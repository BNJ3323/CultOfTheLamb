#include "DungeonGenerator.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

UDungeonGenerator::UDungeonGenerator()
{
}

void UDungeonGenerator::GenerateDungeon(int32 Width, int32 Height, int32 NumRooms, int32 MinRoomSize, int32 MaxRoomSize, int32 Seed)
{
    GridW = Width;
    GridH = Height;
    Grid.SetNumZeroed(GridW * GridH);
    Rooms.Empty();

    if (Seed != 0)
        FMath::RandInit(Seed);

    for (int i = 0; i < NumRooms; ++i)
    {
        int w = FMath::RandRange(MinRoomSize, MaxRoomSize);
        int h = FMath::RandRange(MinRoomSize, MaxRoomSize);
        int x = FMath::RandRange(1, GridW - w - 2);
        int y = FMath::RandRange(1, GridH - h - 2);

        FDungeonRoom R;
        R.X = x; R.Y = y; R.Width = w; R.Height = h;

        // naive overlap check
        bool bOverlap = false;
        for (auto& Existing : Rooms)
        {
            if (!(x + w < Existing.X || Existing.X + Existing.Width < x || y + h < Existing.Y || Existing.Y + Existing.Height < y))
            {
                bOverlap = true;
                break;
            }
        }

        if (!bOverlap)
        {
            Rooms.Add(R);
            CarveRoom(R);
        }
    }

    // optionally carve corridors (connect room centers)
    for (int i = 1; i < Rooms.Num(); ++i)
    {
        FDungeonRoom& A = Rooms[i-1];
        FDungeonRoom& B = Rooms[i];
        int ax = A.X + A.Width/2;
        int ay = A.Y + A.Height/2;
        int bx = B.X + B.Width/2;
        int by = B.Y + B.Height/2;

        // horizontal
        int x0 = FMath::Min(ax, bx);
        int x1 = FMath::Max(ax, bx);
        for (int x = x0; x <= x1; ++x)
            Grid[ay * GridW + x] = 1;

        // vertical
        int y0 = FMath::Min(ay, by);
        int y1 = FMath::Max(ay, by);
        for (int y = y0; y <= y1; ++y)
            Grid[y * GridW + bx] = 1;
    }
}

void UDungeonGenerator::CarveRoom(const FDungeonRoom& R)
{
    for (int y = R.Y; y < R.Y + R.Height; ++y)
    {
        for (int x = R.X; x < R.X + R.Width; ++x)
        {
            Grid[y * GridW + x] = 1;
        }
    }
}

void UDungeonGenerator::SpawnIntoWorld(UWorld* World, FVector Origin, float TileSize)
{
    if (!World) return;
    for (int y = 0; y < GridH; ++y)
    {
        for (int x = 0; x < GridW; ++x)
        {
            int8 Cell = Grid[y * GridW + x];
            FVector Loc = Origin + FVector(x * TileSize, y * TileSize, 0.f);

            if (Cell == 1 && FloorActor)
            {
                World->SpawnActor<AActor>(FloorActor, Loc, FRotator::ZeroRotator);
            }
            else if (Cell == 0 && WallActor)
            {
                World->SpawnActor<AActor>(WallActor, Loc, FRotator::ZeroRotator);
            }
        }
    }
}
