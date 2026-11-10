# ひっぱれ、とばせ — UE5.7 Blueprint組み立てガイド

**このZIPにあるもの**: UE 5.7 Blueprint専用プロジェクト設定、入力定義、ステージ自動生成用の**エディタ専用**Python、シンプルなマップとマテリアルと空のBlueprintアセットをエディタで生成する仕組み。

**まだ入っていないもの**: 実行可能なイベントグラフの接続済み `.uasset`。本環境ではUnreal Editorを起動できないため、ノードをUE上で組み立ててください。Pythonはエディタの配置作業だけに使用し、実際のゲームプレイ処理はBlueprintだけで行います。

## 0. プロジェクトを開く

1. `HippareTobase.uproject` を UE5.7 で開く。
2. Tools > Debug > Output Log を開き、入力欄のモードを **Python** に変更する。Python Editor Script Pluginが有効であることを確認。
3. 1行で `exec(open(r'C:\PATH\TO\HippareTobase_UE57\Tools\generate_stage.py', encoding='utf-8').read())` を実行（実際のフルパスに変更）。
4. Content Browser の `/Game/Maps/L_Prototype` をダブルクリックし、ステージ、マテリアル、空のBPができたことを確認。
5. 生成失敗の場合は Output Log の最初の Python 例外を確認。UE5.7のAPIは環境やプラグイン有効化状況で差があるため、スクリプトの動作は未実機検証です。

## 1. BP_Player (Parent: Character)

### Components

- `CapsuleComponent` はCharacterに標準で付属。Radius 38 / Half Height 88 程度。
- `Mesh` (Character標準SkeletalMesh) は非表示にする（Hidden in Game）。
- `Body` をStaticMeshComponentで追加。Mesh=`/Engine/BasicShapes/Sphere`。Relative Location=(0,0,0), Relative Scale=(0.75,0.75,0.75)、Material=`M_Player`。
- `SpringArm` を追加。Target Arm Length=950, Rotation=(-60,-0,0)（角度の順番はPitch/Yaw/Roll）。`Do Collision Test`=false。
- `Camera` をSpringArmに子付け。
- CharacterMovement `Max Walk Speed`=400、`Gravity Scale`=1.8、`Air Control`=0.25。
- Class Defaults: Auto Possess Player = Player 0（GameModeを設定するなら不要）。

### Variables

| 名前 | 型 | 初期値 |
|---|---|---|
| bCharging | Boolean | false |
| PressMouse | Vector2D | (0,0) |
| CurrentMouse | Vector2D | (0,0) |
| MaxDragPixels | Float | 300 |
| LaunchStrength | Float | 1800 |
| UpwardStrength | Float | 560 |
| bCanLaunch | Boolean | true |
| Lives | Integer | 3 |
| TimeLeft | Float | 180 |
| CollectCount | Integer | 0 |
| SpawnPoint | Vector | (0,0,0) |

### Event Graph: WASD

- `InputAxis MoveForward` → `Branch (NOT bCharging)` → `Add Movement Input`。World Direction=(1,0,0)、Scale Value=Axis Value。
- `InputAxis MoveRight` → `Branch (NOT bCharging)` → `Add Movement Input`。World Direction=(0,1,0)、Scale Value=Axis Value。
- 入力が反応しない場合は `Auto Receive Input=Player 0` または PawnのPossess/GameModeを確認。

### Event Graph: 引っ張って発射

ここでは**画面上をマウスでドラッグした距離**を引っ張り量とみなし、キャラの背面方向に引いた分だけ前に飛ばす。初期段階では照準は世界のX+方向を前、Yを横とする。引っ張り中にプレイヤーの位置は動かさない（プロトタイプの簡略化）。

**InputAction Pull (Pressed)**

`InputAction Pull.Pressed` → `Branch bCanLaunch` → `Set bCharging = true` → `Get Player Controller` → `Set Show Mouse Cursor(true)` → `Get Mouse Position on Viewport`（もしくは `Get Mouse Position Scaled by DPI`; 関数の出力/スケールを統一） → `Set PressMouse`。

**InputAction Pull (Released)**

`InputAction Pull.Released` → `Branch bCharging` → `Set bCharging=false` → `Get Mouse Position on Viewport` → `Set CurrentMouse` → `Vector2D - Vector2D` (`PressMouse - CurrentMouse`) → `Vector Length` → `Clamp (Float, 0, MaxDragPixels)` → ` / MaxDragPixels` → `Power(x, 1.2)` → `* LaunchStrength`。

方向は、ドラッグ量の各成分を使って `Make Vector` = `(Clamp(PressMouse.Y-CurrentMouse.Y,-300,300), Clamp(PressMouse.X-CurrentMouse.X,-300,300), 0)` → `Get Safe Normal` → `LaunchCharacter` のXYへ変換。高さは `UpwardStrength * (0.45 + 0.55 * ChargeRate)`。最初は次の簡易バージョンで動作確認することを推奨：

`LaunchCharacter (Launch Velocity = Make Vector(ChargeRate*LaunchStrength, 0, UpwardStrength), XYOverride=true, ZOverride=true)`。

前方X固定の簡易版が動いたら、XYの方向入力版へ拡張する。`LaunchCharacter` は CharacterMovement を使用する（Simulate PhysicsはBody/Capsule共にOFF）。

**着地で再使用**

`Event OnLanded` → `Set bCanLaunch=true`。発射時 `Set bCanLaunch=false`。入力した直後に空中から再度発射しないようにする。発射時に `CharacterMovement -> Set Movement Mode(Falling)` は不要（LaunchCharacterが適切に処理）。

### カウントダウン / 落下

`Event BeginPlay` → `Get Actor Location` → `Set SpawnPoint` → `Set Timer by Event` (Time=1.0, Looping=true) → CustomEvent `Countdown`。

`Countdown` → `Set TimeLeft = TimeLeft - 1` → `Branch TimeLeft <= 0` → `Print String('TIME UP / GAME OVER')` → `Set Game Paused(true)`。

`Event Tick` → `Get Actor Location` → `Break Vector Z` → `Branch Z < -500` → `Set Lives = Lives-1` → `Branch Lives <=0` → `Print String('GAME OVER')`, `Set Game Paused(true)`; else `SetActorLocation(SpawnPoint, Sweep=false, Teleport=true)` and `CharacterMovement -> Stop Movement Immediately`。

**注意**: Tick中のLives減算はリスポーン前に1回だけ行う。ブランチ内で即時リスポーンすると通常は1回だが、複雑化する場合は `bRespawning` で多重実行を防ぐ。

## 2. BP_Goal (Parent: Actor)

- Components: Scene Root、BoxCollision（100,250,200程度のExtent）、StaticMesh（緑のCube / M_Goal）。
- BoxCollision: Generate Overlap Events ON、Collision Preset=`Trigger`。
- Event: `OnComponentBeginOverlap(BoxCollision)` → `Cast to BP_Player (Other Actor)` → `Print String('CLEAR!')` → `Set Game Paused(true)`。
- レベルの緑プレースホルダー `Goal_Green_Placeholder` を削除して `BP_Goal` に置き換える。

## 3. BP_Trap (Parent: Actor)

- Components: BoxCollision (Trigger)、StaticMesh(Cube / M_Trap)。
- Event: `OnComponentBeginOverlap` → `Cast to BP_Player` → `Get Lives` → `Integer - 1` → `Set Lives` → `Branch Lives<=0`。trueなら `GAME OVER` + Pause、falseなら `SetActorLocation(SpawnPoint)` + `Stop Movement Immediately`。
- レベルの `Trap_Red_01`, `Trap_Red_02` を削除し、同じ位置にBP_Trapを配置する。色・寸法は見た目のCubeで調整。

## 4. BP_Collectible (Parent: Actor)

- Components: SphereCollision (Trigger)、StaticMesh(Sphere / M_Collectible)。
- `OnComponentBeginOverlap` → `Cast to BP_Player` → `Get CollectCount` → `+1` → `Set CollectCount` → `Destroy Actor(Self)`。
- 黄のプレースホルダーをBP_Collectibleに置き換える。

## 5. BP_PrototypeGameMode (Parent: GameModeBase)

- Class Defaults の `Default Pawn Class` を `BP_Player` に設定。
- レベルの `World Settings > GameMode Override` を `BP_PrototypeGameMode` に指定。
- `Player_Blue_Placeholder` を削除する。`PlayerStart` は残す。
- GameMode BPはこの段階では空でも問題なし。

## 6. テスト手順

1. UE Editorで `L_Prototype` を開き、`Play` → WASDで青い球体が動く。
2. 左クリックを押してドラッグして離すと、ドラッグ距離に応じて飛ぶ。着地後は再発射可能。
3. 穴に落ちるか赤い罠に接触すると残機が減る。黄色い球に触れると取得、緑ゴールでCLEAR表示。
4. 残機0または180秒経過でGAME OVER。

## よくある問題

- 開けたのに何もない: ステージはファイルに含まれず、最初のPython実行で作成する方式。`L_Prototype`を開く。
- Blueprintが空: 仕様通り。ゲーム用イベントグラフはこの手順に従ってUE上で接続が必要。
- カメラが動かない: SpringArm/Camera の親子構成とPawnへのPossessを確認。
- 引っ張れない: InputAction Pull が `DefaultInput.ini` に存在するか確認。UE5.7でもLegacy Inputは利用できる構成を想定。プロジェクト設定のInputを確認。
- Generate Scriptがエラー: Output Logの最初のPython例外を確認。API変更・未有効のエディタプラグインは要修正。
