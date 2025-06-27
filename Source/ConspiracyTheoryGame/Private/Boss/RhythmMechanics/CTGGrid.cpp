// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGGrid.h"
#include "DrawDebugHelpers.h"

ACTGGrid::ACTGGrid()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACTGGrid::BeginPlay()
{
    Super::BeginPlay();
}

void ACTGGrid::SetModel(const FSettings& InSettings, int32 InCellSize)
{
    GridDim = InSettings.GridSize;
    CellSize = InCellSize;
    WorldWidht = GridDim.Width * CellSize;
    WorldHeight = GridDim.Height * CellSize;
}

void ACTGGrid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawGrid();
}

void ACTGGrid::DrawGrid()
{
    for (int32 i = 0; i < GridDim.Height + 1; ++i)
    {
        const FVector StartLocation = GetActorLocation() + GetActorForwardVector() * CellSize * i;
        DrawDebugLine(GetWorld(), StartLocation, StartLocation + GetActorRightVector() * WorldWidht, FColor::Red, false, -1.0, 0, 1);
    }

    for (int32 i = 0; i < GridDim.Width + 1; ++i)
    {
        const FVector StartLocation = GetActorLocation() + GetActorRightVector() * CellSize * i;
        DrawDebugLine(GetWorld(), StartLocation, StartLocation + GetActorForwardVector() * WorldHeight, FColor::Red, false, -1.0, 0, 1);
    }
}