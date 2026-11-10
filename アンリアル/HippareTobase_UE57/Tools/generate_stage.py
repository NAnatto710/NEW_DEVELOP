"""Run inside Unreal Engine 5.7 Python console with:
exec(open(r'FULL_PATH/Tools/generate_stage.py', encoding='utf-8').read())

Editor-time stage/asset generation only. All gameplay logic belongs to Blueprints.
"""
import unreal

ASSETS = unreal.EditorAssetLibrary
LEVELS = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTORS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

for folder in ['Maps','Blueprints','Materials','Props','UI']:
    ASSETS.make_directory('/Game/' + folder)

level_path = '/Game/Maps/L_Prototype'
if not LEVELS.new_level(level_path):
    if not LEVELS.load_level(level_path):
        raise RuntimeError('Cannot create or load ' + level_path)

cube = unreal.load_asset('/Engine/BasicShapes/Cube.Cube')
sphere = unreal.load_asset('/Engine/BasicShapes/Sphere.Sphere')
colors = {
    'Floor': (0.55, 0.74, 0.91),
    'Player': (0.05, 0.45, 0.98),
    'Trap': (0.92, 0.14, 0.20),
    'Goal': (0.11, 0.85, 0.43),
    'Collectible': (1.0, 0.72, 0.08),
    'Wall': (0.74, 0.77, 0.83),
}

def material(name, rgb):
    path = '/Game/Materials/M_' + name
    old = ASSETS.load_asset(path)
    if old:
        return old
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    mat = asset_tools.create_asset('M_' + name, '/Game/Materials', unreal.Material, unreal.MaterialFactoryNew())
    # Material Editing Library is editor-only; resulting material is a normal .uasset.
    node = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -300, 0)
    node.set_editor_property('parameter_name', 'Tint')
    node.set_editor_property('default_value', unreal.LinearColor(*rgb, 1.0))
    unreal.MaterialEditingLibrary.connect_material_property(node, '', unreal.MaterialProperty.MP_BASE_COLOR)
    unreal.MaterialEditingLibrary.recompile_material(mat)
    ASSETS.save_loaded_asset(mat)
    return mat

mats = {key: material(key, value) for key,value in colors.items()}

def static_actor(label, mesh, loc, scale, mat, rot=None):
    actor = ACTORS.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(*loc), rot or unreal.Rotator(0,0,0))
    if not actor:
        raise RuntimeError('Failed to spawn '+label)
    actor.set_actor_label(label)
    comp = actor.static_mesh_component
    comp.set_static_mesh(mesh)
    comp.set_world_scale3d(unreal.Vector(*scale))
    comp.set_material(0, mats[mat])
    return actor

# Units: starter Cube = 100 cm edge; spheres also ~100cm.
static_actor('Floor_Start', cube, (0,0,-50), (9,9,1), 'Floor')
static_actor('Floor_Mid', cube, (1150,0,-50), (8,7,1), 'Floor')
static_actor('Floor_End', cube, (2240,0,-50), (7,9,1), 'Floor')
static_actor('LaunchGapMarker_Left', cube, (550,-380,30), (0.35,1.4,0.6), 'Wall')
static_actor('LaunchGapMarker_Right', cube, (550,380,30), (0.35,1.4,0.6), 'Wall')
static_actor('SideWallA', cube, (1250,-365,45), (6,0.25,1.9), 'Wall')
static_actor('SideWallB', cube, (1250,365,45), (6,0.25,1.9), 'Wall')
static_actor('Trap_Red_01', cube, (1150,0,20), (0.8,2.4,0.4), 'Trap')
static_actor('Trap_Red_02', cube, (1700,120,20), (0.6,2.2,0.4), 'Trap')
static_actor('Collectible_Yellow_01', sphere, (1300,-240,110), (0.45,0.45,0.45), 'Collectible')
static_actor('Collectible_Yellow_02', sphere, (1830,-220,110), (0.45,0.45,0.45), 'Collectible')
static_actor('Goal_Green_Placeholder', cube, (2440,0,140), (0.45,3.0,2.8), 'Goal')
static_actor('Player_Blue_Placeholder', sphere, (-250,0,85), (0.75,0.75,0.75), 'Player')

light = ACTORS.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0,0,700), unreal.Rotator(-45,-20,0))
light.set_actor_label('SunLight')
sky = ACTORS.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0,0,400))
sky.set_actor_label('SkyLight')
start = ACTORS.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(-250,0,150))
start.set_actor_label('PlayerStart')

# Generate Blueprint asset shells. BP graphs intentionally remain editor-editable and unconnected.
# Do not confuse asset creation with implemented gameplay.
bp_parents = {
  'BP_Player': unreal.Character,
  'BP_Goal': unreal.Actor,
  'BP_Trap': unreal.Actor,
  'BP_Collectible': unreal.Actor,
  'BP_PrototypeGameMode': unreal.GameModeBase,
}
for name,parent in bp_parents.items():
    path = '/Game/Blueprints/' + name
    bp = ASSETS.load_asset(path)
    if not bp:
        bp = unreal.BlueprintEditorLibrary.create_blueprint_asset_with_parent(path, parent)
    if not bp:
        unreal.log_warning('Could not create blueprint: ' + name)
    else:
        unreal.BlueprintEditorLibrary.compile_blueprint(bp)
        ASSETS.save_loaded_asset(bp)

LEVELS.save_current_level()
ASSETS.save_directory('/Game', recursive=True)
unreal.log('HippareTobase: stage + materials + empty Blueprint shells generated. Follow Docs/BP_Assembly_Guide.md to wire logic and replace placeholders.')
