# CultOfTheLamb - Prototype skeleton

This repository contains the initial Unreal Engine 5.7 prototype skeleton for a Cult-like game (low-poly, top-down/dungeon loop).

Contents:
- CultLike.uproject
- Source/CultLike/ (C++ module with GameMode, Player, Enemy, DungeonGenerator)
- Tools/ImportAndSetup.py (UE Python script to import OBJ placeholders and create simple materials)
- Tools/ImportMeshes/ (small low-poly OBJ placeholders)

Quick start
1. Clone the repo.
2. In the project root run "GenerateProjectFiles.bat" (Windows) or the corresponding script for your OS.
3. Open the generated .sln in Visual Studio (or Xcode) and build the project, or open the .uproject in Unreal Editor and let it compile.
4. Put the OBJ files in <ProjectFolder>/ImportMeshes/ and run the Python script from the Editor scripting window to import meshes and create materials.
5. Create Blueprints for Player/Enemy/Floor/Wall by adding StaticMeshComponents and assigning the imported assets. Set collisions appropriately.
6. Create a Test level and use the DungeonGenerator (exposed to Blueprint) to generate a tile layout and spawn Floor/Wall actors.

Notes
- This is a starting skeleton: you'll need to create Blueprints and assign default pawn/GameMode in the editor.
- The project uses .gitattributes configured for Git LFS for common asset types. Run `git lfs install` locally if you plan to add large assets.
