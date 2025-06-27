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
    WorldWidth = GridDim.Width * CellSize;
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
        const FVector EndLocation = StartLocation + GetActorRightVector() * WorldWidth;
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0, 0, 2);
    }

    for (int32 j = 0; j < GridDim.Width + 1; ++j)
    {
        const FVector StartLocation = GetActorLocation() + GetActorRightVector() * CellSize * j;
        const FVector EndLocation = StartLocation + GetActorForwardVector() * WorldHeight;
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0, 0, 2);
    }
}