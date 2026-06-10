#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonGenerator.generated.h"

USTRUCT()
struct FDungeonRoom
{
    GENERATED_BODY()
    int32 X, Y, Width, Height;
};

UCLASS(Blueprintable)
class UDungeonGenerator : public UObject
{
    GENERATED_BODY()

public:
    UDungeonGenerator();

    UFUNCTION(BlueprintCallable, Category="Dungeon")
    void GenerateDungeon(int32 Width, int32 Height, int32 NumRooms, int32 MinRoomSize = 4, int32 MaxRoomSize = 12, int32 Seed = 0);

    UPROPERTY(BlueprintReadOnly, Category="Dungeon")
    TArray<FDungeonRoom> Rooms;

    // Expose spawnable actors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dungeon")
    TSubclassOf<AActor> FloorActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dungeon")
    TSubclassOf<AActor> WallActor;

    // Spawn actors in world
    UFUNCTION(BlueprintCallable, Category="Dungeon")
    void SpawnIntoWorld(UWorld* World, FVector Origin = FVector::ZeroVector, float TileSize = 200.f);
private:
    TArray<int8> Grid;
    int32 GridW, GridH;
    void CarveRoom(const FDungeonRoom& R);
};
