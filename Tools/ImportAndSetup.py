# Place this file into the project's Content/Python or run from Editor Scripting Console
import unreal

@unreal.uclass()
class AssetToolsHelper(unreal.GlobalEditorUtilityBase):
    pass

def import_mesh(path_on_disk, dest_path, filename):
    task = unreal.AssetImportTask()
    task.filename = path_on_disk
    task.destination_path = dest_path
    task.automated = True
    task.save = True
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    assets = task.get_editor_property('imported_object_paths')
    return assets

def create_material(name, color):
    factory = unreal.MaterialFactoryNew()
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    mat = asset_tools.create_asset(name, "/Game/Materials", unreal.Material, factory)
    # create simple color node
    expression = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionConstant3Vector, -400, 0)
    expression.constant = unreal.LinearColor(color[0], color[1], color[2], 1.0)
    unreal.MaterialEditingLibrary.connect_material_property(expression, "R", unreal.MaterialProperty.MP_BASE_COLOR)
    unreal.MaterialEditingLibrary.update_material(mat)
    return mat

def create_bp_from_staticmesh(bp_name, mesh_asset_path, dest_folder="/Game/Blueprints"):
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    factory = unreal.BlueprintFactory()
    factory.set_editor_property('ParentClass', unreal.Actor)
    bp = asset_tools.create_asset(bp_name, dest_folder, None, factory)
    # add StaticMeshComponent and assign mesh
    unreal.EditorAssetLibrary.save_loaded_asset(bp)
    # NOTE: creating components via script is more involved; instruct to edit BP and add StaticMeshComponent and assign mesh manually if required.
    return bp

def main():
    unreal.log("Starting import script: import OBJ placeholders and create materials.")
    # Adjust these paths before running
    disk_folder = unreal.SystemLibrary.get_project_directory() + "ImportMeshes/"
    # Example files: Player.obj, Enemy.obj, Floor.obj, Wall.obj
    imports = ["Player.obj", "Enemy.obj", "Floor.obj", "Wall.obj"]
    for f in imports:
        path = disk_folder + f
        unreal.log("Importing " + path)
        try:
            imported = import_mesh(path, "/Game/Meshes", f)
            unreal.log("Imported: " + str(imported))
        except Exception as e:
            unreal.log_warning("Import failed: " + str(e))

    # Create simple materials
    create_material("M_Player", (0.8, 0.2, 0.2))
    create_material("M_Enemy", (0.2, 0.2, 0.8))
    create_material("M_Floor", (0.6, 0.5, 0.4))
    create_material("M_Wall", (0.2, 0.2, 0.2))

    unreal.log("Import script finished. Create Blueprints manually or extend the script to auto-create Actors.")
