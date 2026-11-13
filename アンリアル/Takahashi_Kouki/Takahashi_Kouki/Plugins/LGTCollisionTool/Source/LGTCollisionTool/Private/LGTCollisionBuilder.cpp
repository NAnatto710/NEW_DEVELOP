#include "LGTCollisionBuilder.h"

#include "Algo/Sort.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Editor.h"
#include "Engine/CollisionProfile.h"
#include "Engine/Level.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "InstanceDataSceneProxy.h"
#include "InstancedStaticMesh/ISMInstanceDataSceneProxy.h"
#include "LandscapeProxy.h"
#include "Misc/ScopedSlowTask.h"
#include "PhysicsEngine/BodyInstance.h"
#include "PhysicsEngine/BodySetup.h"
#include "ScopedTransaction.h"

#define LOCTEXT_NAMESPACE "LGTCollisionBuilder"

const FName FLGTCollisionBuilder::GeneratedActorTag(TEXT("LGTCollisionTool.Generated"));
const TCHAR* FLGTCollisionBuilder::GeneratedActorLabel = TEXT("LGT_GeneratedCollision");

namespace LGTCollisionBuilderInternal
{
    struct FCollectedMeshData
    {
        UStaticMesh* Mesh = nullptr;
        TArray<FTransform> WorldTransforms;

        bool bHasSimpleCollision = false;
        bool bUsesComplexAsSimple = false;

        // Static Mesh アセット側で Collision が有効か
        bool bAssetCollisionEnabled = false;
        FName CollisionProfileName = NAME_None;
    };

    bool GetCollisionState(
        UStaticMesh* Mesh,
        bool& bOutHasSimpleCollision,
        bool& bOutUsesComplexAsSimple,
        bool& bOutAssetCollisionEnabled,
        FName& OutCollisionProfileName)
    {
        bOutHasSimpleCollision = false;
        bOutUsesComplexAsSimple = false;
        bOutAssetCollisionEnabled = false;
        OutCollisionProfileName = NAME_None;

        if (!Mesh)
        {
            return false;
        }

        UBodySetup* BodySetup = Mesh->GetBodySetup();
        if (!BodySetup)
        {
            return false;
        }

        bOutHasSimpleCollision = BodySetup->AggGeom.GetElementCount() > 0;
        bOutUsesComplexAsSimple = BodySetup->CollisionTraceFlag == CTF_UseComplexAsSimple;

        // Static Mesh Editor の Collision Presets / Collision Enabled に相当する設定。
        // BodySetup::DefaultInstance はアセットから生成される BodyInstance の既定値。
        const FBodyInstance& DefaultInstance = BodySetup->DefaultInstance;
        const ECollisionEnabled::Type CollisionEnabled = DefaultInstance.GetCollisionEnabled(false);

        bOutAssetCollisionEnabled = CollisionEnabled != ECollisionEnabled::NoCollision;
        OutCollisionProfileName = DefaultInstance.GetCollisionProfileName();

        return true;
    }

    void ForcePopulateLandscapeGrass(UWorld* World, int32& OutLandscapeCount, TArray<FString>& OutWarnings)
    {
        OutLandscapeCount = 0;
        if (!World)
        {
            OutWarnings.Add(TEXT("World が無効です。"));
            return;
        }

        TArray<FVector> NoCullCameras;

        for (TActorIterator<ALandscapeProxy> It(World); It; ++It)
        {
            ALandscapeProxy* Landscape = *It;
            if (!IsValid(Landscape))
            {
                continue;
            }

            ++OutLandscapeCount;

            // Cameras が空なら距離カリングなし。bForceSync=true で生成完了を待つ。
            Landscape->UpdateGrass(NoCullCameras, true);
        }

        if (OutLandscapeCount == 0)
        {
            OutWarnings.Add(TEXT("現在の Editor World に Landscape が見つかりません。"));
        }
    }

    bool AppendRenderInstanceWorldTransforms(
        UHierarchicalInstancedStaticMeshComponent* SourceComponent,
        UStaticMesh* Mesh,
        TArray<FTransform>& OutWorldTransforms,
        TArray<FString>& OutWarnings)
    {
        if (!IsValid(SourceComponent))
        {
            return false;
        }

        const int32 RenderInstanceCount = SourceComponent->GetNumRenderInstances();
        if (RenderInstanceCount <= 0)
        {
            return true;
        }

        TSharedPtr<FISMCInstanceDataSceneProxy, ESPMode::ThreadSafe> InstanceDataProxy =
            SourceComponent->GetOrCreateInstanceDataSceneProxy();

        if (!InstanceDataProxy.IsValid())
        {
            OutWarnings.Add(FString::Printf(
                TEXT("%s の Instance Data Scene Proxy を取得できませんでした。"),
                Mesh ? *Mesh->GetName() : TEXT("<Invalid Mesh>")));
            return false;
        }

        // Instance Data の更新が非同期で走っている場合は、Buffer を読む前に完了を待つ。
        if (FInstanceDataUpdateTaskInfo* UpdateTaskInfo = InstanceDataProxy->GetUpdateTaskInfo())
        {
            UpdateTaskInfo->WaitForUpdateCompletion();
        }

        const FInstanceSceneDataBuffers& InstanceData = InstanceDataProxy->GetData();
        FInstanceSceneDataBuffers::FAccessTag AccessTag;

        const int32 BufferInstanceCount = InstanceData.GetNumInstances(AccessTag);
        if (BufferInstanceCount <= 0)
        {
            OutWarnings.Add(FString::Printf(
                TEXT("%s は Render Instance を %d 個持っていますが、Instance Data Buffer から Transform を取得できませんでした。"),
                Mesh ? *Mesh->GetName() : TEXT("<Invalid Mesh>"),
                RenderInstanceCount));
            return false;
        }

        OutWorldTransforms.Reserve(OutWorldTransforms.Num() + BufferInstanceCount);

        for (int32 InstanceIndex = 0; InstanceIndex < BufferInstanceCount; ++InstanceIndex)
        {
            const FMatrix InstanceToWorld = InstanceData.GetInstanceToWorld(InstanceIndex, AccessTag);
            OutWorldTransforms.Emplace(InstanceToWorld);
        }

        if (BufferInstanceCount != RenderInstanceCount)
        {
            OutWarnings.Add(FString::Printf(
                TEXT("%s : Render Instance 数 (%d) と Instance Data Buffer 数 (%d) が一致しません。"),
                Mesh ? *Mesh->GetName() : TEXT("<Invalid Mesh>"),
                RenderInstanceCount,
                BufferInstanceCount));
        }

        return true;
    }

    TMap<UStaticMesh*, FCollectedMeshData> Collect(
        UWorld* World,
        bool bForcePopulateGrass,
        int32& OutLandscapeCount,
        int32& OutSourceComponentCount,
        TArray<FString>& OutWarnings)
    {
        TMap<UStaticMesh*, FCollectedMeshData> Collected;
        OutLandscapeCount = 0;
        OutSourceComponentCount = 0;

        if (!World)
        {
            OutWarnings.Add(TEXT("World が無効です。"));
            return Collected;
        }

        if (bForcePopulateGrass)
        {
            ForcePopulateLandscapeGrass(World, OutLandscapeCount, OutWarnings);
        }

        if (!bForcePopulateGrass)
        {
            for (TActorIterator<ALandscapeProxy> It(World); It; ++It)
            {
                if (IsValid(*It))
                {
                    ++OutLandscapeCount;
                }
            }
        }

        for (TActorIterator<ALandscapeProxy> It(World); It; ++It)
        {
            ALandscapeProxy* Landscape = *It;
            if (!IsValid(Landscape))
            {
                continue;
            }

            for (UHierarchicalInstancedStaticMeshComponent* SourceComponent : Landscape->FoliageComponents)
            {
                if (!IsValid(SourceComponent) || SourceComponent->GetWorld() != World)
                {
                    continue;
                }

                UStaticMesh* Mesh = SourceComponent->GetStaticMesh();
                if (!IsValid(Mesh))
                {
                    OutWarnings.Add(FString::Printf(
                        TEXT("%s に Static Mesh が設定されていない LGT Component があります。"),
                        *Landscape->GetActorLabel()));
                    continue;
                }

                ++OutSourceComponentCount;

                FCollectedMeshData& MeshData = Collected.FindOrAdd(Mesh);
                MeshData.Mesh = Mesh;

                GetCollisionState(
                    Mesh,
                    MeshData.bHasSimpleCollision,
                    MeshData.bUsesComplexAsSimple,
                    MeshData.bAssetCollisionEnabled,
                    MeshData.CollisionProfileName);

                AppendRenderInstanceWorldTransforms(
                    SourceComponent,
                    Mesh,
                    MeshData.WorldTransforms,
                    OutWarnings);
            }
        }

        if (OutSourceComponentCount == 0 && OutLandscapeCount > 0)
        {
            OutWarnings.Add(TEXT("Landscape は見つかりましたが、生成済みの LGT Foliage Component がありません。Landscape Material の Grass Output を確認してください。"));
        }

        return Collected;
    }

    bool IsUsableCollision(const FCollectedMeshData& MeshData, const FLGTBuildOptions& Options)
    {
        // Static Mesh アセット側が NoCollision なら、Collision Geometry が存在していても生成対象にしない。
        if (!MeshData.bAssetCollisionEnabled)
        {
            return false;
        }

        if (MeshData.bHasSimpleCollision)
        {
            return true;
        }

        return Options.bTreatComplexAsSimpleAsCollision && MeshData.bUsesComplexAsSimple;
    }

    FString MakeComponentBaseName(const UStaticMesh* Mesh)
    {
        FString Name = Mesh ? Mesh->GetName() : TEXT("UnknownMesh");
        for (TCHAR& Character : Name)
        {
            if (!FChar::IsAlnum(Character) && Character != TEXT('_'))
            {
                Character = TEXT('_');
            }
        }
        return FString::Printf(TEXT("HISM_%s"), *Name);
    }
}

FString FLGTScanReport::ToText() const
{
    FString Result;
    Result += FString::Printf(TEXT("Landscape : %d\n"), LandscapeCount);
    Result += FString::Printf(TEXT("LGT Component : %d\n"), SourceComponentCount);
    Result += FString::Printf(TEXT("総 Instance 数 : %lld\n"), TotalInstanceCount);
    Result += FString::Printf(TEXT("Static Mesh 種類 : %d\n\n"), Meshes.Num());

    if (Meshes.IsEmpty())
    {
        Result += TEXT("対象となる LGT Instance がありません。\n");
    }
    else
    {
        Result += TEXT("本数       Geometry           Asset    Preset             Static Mesh\n");
        Result += TEXT("--------------------------------------------------------------------------------\n");

        for (const FLGTMeshScanResult& Item : Meshes)
        {
            const UStaticMesh* Mesh = Item.Mesh.Get();

            const FString GeometryText = Item.bHasSimpleCollision
                ? TEXT("Simple")
                : (Item.bUsesComplexAsSimple ? TEXT("ComplexAsSimple") : TEXT("なし"));

            const FString AssetCollisionText = Item.bAssetCollisionEnabled
                ? TEXT("ON")
                : TEXT("OFF");

            const FString ProfileText = Item.CollisionProfileName.IsNone()
                ? TEXT("-")
                : Item.CollisionProfileName.ToString();

            Result += FString::Printf(
                TEXT("%8d   %-18s %-8s %-18s %s\n"),
                Item.InstanceCount,
                *GeometryText,
                *AssetCollisionText,
                *ProfileText,
                Mesh ? *Mesh->GetPathName() : TEXT("<Invalid Mesh>"));
        }
    }

    if (!Warnings.IsEmpty())
    {
        Result += TEXT("\n警告:\n");
        for (const FString& Warning : Warnings)
        {
            Result += TEXT("- ") + Warning + TEXT("\n");
        }
    }

    return Result;
}

FLGTScanReport FLGTCollisionBuilder::Scan(UWorld* World, bool bForcePopulateGrass)
{
    using namespace LGTCollisionBuilderInternal;

    FLGTScanReport Report;
    TMap<UStaticMesh*, FCollectedMeshData> Collected = Collect(
        World,
        bForcePopulateGrass,
        Report.LandscapeCount,
        Report.SourceComponentCount,
        Report.Warnings);

    for (const TPair<UStaticMesh*, FCollectedMeshData>& Pair : Collected)
    {
        const FCollectedMeshData& MeshData = Pair.Value;
        FLGTMeshScanResult& Item = Report.Meshes.AddDefaulted_GetRef();
        Item.Mesh = MeshData.Mesh;
        Item.InstanceCount = MeshData.WorldTransforms.Num();
        Item.bHasSimpleCollision = MeshData.bHasSimpleCollision;
        Item.bUsesComplexAsSimple = MeshData.bUsesComplexAsSimple;
        Item.bAssetCollisionEnabled = MeshData.bAssetCollisionEnabled;
        Item.CollisionProfileName = MeshData.CollisionProfileName;
        Report.TotalInstanceCount += Item.InstanceCount;
    }

    Report.Meshes.Sort([](const FLGTMeshScanResult& A, const FLGTMeshScanResult& B)
    {
        const UStaticMesh* MeshA = A.Mesh.Get();
        const UStaticMesh* MeshB = B.Mesh.Get();
        const FString PathA = MeshA ? MeshA->GetPathName() : FString();
        const FString PathB = MeshB ? MeshB->GetPathName() : FString();
        return PathA < PathB;
    });

    return Report;
}

FLGTBuildReport FLGTCollisionBuilder::Rebuild(UWorld* World, const FLGTBuildOptions& Options)
{
    using namespace LGTCollisionBuilderInternal;

    FLGTBuildReport BuildReport;
    if (!World)
    {
        BuildReport.Message = TEXT("Editor World を取得できませんでした。");
        return BuildReport;
    }

    int32 LandscapeCount = 0;
    int32 SourceComponentCount = 0;
    TArray<FString> Warnings;

    TMap<UStaticMesh*, FCollectedMeshData> Collected = Collect(
        World,
        true,
        LandscapeCount,
        SourceComponentCount,
        Warnings);

    if (Collected.IsEmpty())
    {
        BuildReport.Message = TEXT("生成対象の LGT Instance がありません。\n");
        for (const FString& Warning : Warnings)
        {
            BuildReport.Message += TEXT("- ") + Warning + TEXT("\n");
        }
        return BuildReport;
    }

    const FScopedTransaction Transaction(LOCTEXT("RebuildTransaction", "Rebuild LGT Collision"));

    DeleteGeneratedActors(World);

    ULevel* TargetLevel = World->GetCurrentLevel();
    if (!TargetLevel)
    {
        TargetLevel = World->PersistentLevel;
    }

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.OverrideLevel = TargetLevel;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParameters.ObjectFlags |= RF_Transactional;

    AActor* GeneratedActor = World->SpawnActor<AActor>(
        AActor::StaticClass(),
        FTransform::Identity,
        SpawnParameters);

    if (!GeneratedActor)
    {
        BuildReport.Message = TEXT("生成 Actor を作成できませんでした。");
        return BuildReport;
    }

    GeneratedActor->Modify();
    GeneratedActor->SetActorLabel(GeneratedActorLabel);
    GeneratedActor->Tags.AddUnique(GeneratedActorTag);

    USceneComponent* RootComponent = NewObject<USceneComponent>(
        GeneratedActor,
        TEXT("Root"),
        RF_Transactional);
    RootComponent->SetMobility(EComponentMobility::Static);
    GeneratedActor->AddInstanceComponent(RootComponent);
    GeneratedActor->SetRootComponent(RootComponent);
    RootComponent->RegisterComponent();

    FScopedSlowTask SlowTask(
        static_cast<float>(Collected.Num()),
        LOCTEXT("BuildingCollision", "LGT Collision HISM を作成しています..."));
    SlowTask.MakeDialog(false);

    for (const TPair<UStaticMesh*, FCollectedMeshData>& Pair : Collected)
    {
        SlowTask.EnterProgressFrame(1.0f);

        const FCollectedMeshData& MeshData = Pair.Value;
        if (!MeshData.Mesh || MeshData.WorldTransforms.IsEmpty())
        {
            continue;
        }

        const bool bHasUsableCollision = IsUsableCollision(MeshData, Options);
        if (Options.bExcludeMeshesWithoutCollision && !bHasUsableCollision)
        {
            ++BuildReport.SkippedMeshCount;
            BuildReport.SkippedInstanceCount += MeshData.WorldTransforms.Num();
            continue;
        }

        const FName ComponentName = MakeUniqueObjectName(
            GeneratedActor,
            UHierarchicalInstancedStaticMeshComponent::StaticClass(),
            *MakeComponentBaseName(MeshData.Mesh));

        UHierarchicalInstancedStaticMeshComponent* HISM =
            NewObject<UHierarchicalInstancedStaticMeshComponent>(
                GeneratedActor,
                ComponentName,
                RF_Transactional);

        HISM->SetMobility(EComponentMobility::Static);
        HISM->SetStaticMesh(MeshData.Mesh);
        HISM->SetupAttachment(RootComponent);
        HISM->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName, false);
        HISM->SetCollisionEnabled(
            Options.bUseQueryAndPhysics
                ? ECollisionEnabled::QueryAndPhysics
                : ECollisionEnabled::QueryOnly);
        HISM->SetCollisionObjectType(ECC_WorldStatic);
        HISM->SetCollisionResponseToAllChannels(ECR_Block);
        HISM->SetGenerateOverlapEvents(false);
        HISM->SetCanEverAffectNavigation(true);
        HISM->SetVisibility(false, true);
        HISM->SetHiddenInGame(true, true);
        HISM->SetCastShadow(false);

        GeneratedActor->AddInstanceComponent(HISM);
        HISM->RegisterComponent();

        for (const FTransform& WorldTransform : MeshData.WorldTransforms)
        {
            HISM->AddInstance(WorldTransform, true);
        }

        ++BuildReport.GeneratedComponentCount;
        BuildReport.GeneratedInstanceCount += MeshData.WorldTransforms.Num();
    }

    if (BuildReport.GeneratedComponentCount == 0)
    {
        World->EditorDestroyActor(GeneratedActor, true);
        BuildReport.Message = TEXT("Collision を持つ生成対象がありませんでした。生成 Actor は作成していません。\n");
    }
    else
    {
        GeneratedActor->MarkPackageDirty();
        if (TargetLevel)
        {
            TargetLevel->MarkPackageDirty();
        }

        if (GEditor)
        {
            GEditor->SelectNone(false, true, false);
            GEditor->SelectActor(GeneratedActor, true, true, true);
        }

        BuildReport.bSucceeded = true;
        BuildReport.Message = FString::Printf(
            TEXT("LGT Collision の再構築が完了しました。\n")
            TEXT("Landscape : %d\n")
            TEXT("LGT Component : %d\n")
            TEXT("生成 HISM : %d\n")
            TEXT("生成 Instance : %lld\n")
            TEXT("除外 Mesh : %d\n")
            TEXT("除外 Instance : %lld\n")
            TEXT("Collision Enabled : %s\n\n")
            TEXT("アウトライナーの %s を確認し、レベルを保存してください。\n"),
            LandscapeCount,
            SourceComponentCount,
            BuildReport.GeneratedComponentCount,
            BuildReport.GeneratedInstanceCount,
            BuildReport.SkippedMeshCount,
            BuildReport.SkippedInstanceCount,
            Options.bUseQueryAndPhysics ? TEXT("Query And Physics") : TEXT("Query Only"),
            GeneratedActorLabel);
    }

    if (!Warnings.IsEmpty())
    {
        BuildReport.Message += TEXT("\n警告:\n");
        for (const FString& Warning : Warnings)
        {
            BuildReport.Message += TEXT("- ") + Warning + TEXT("\n");
        }
    }

    return BuildReport;
}

int32 FLGTCollisionBuilder::DeleteGeneratedActors(UWorld* World)
{
    if (!World)
    {
        return 0;
    }

    TArray<AActor*> ActorsToDelete;
    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (IsValid(Actor) && Actor->Tags.Contains(GeneratedActorTag))
        {
            ActorsToDelete.Add(Actor);
        }
    }

    for (AActor* Actor : ActorsToDelete)
    {
        World->EditorDestroyActor(Actor, true);
    }

    return ActorsToDelete.Num();
}

#undef LOCTEXT_NAMESPACE
