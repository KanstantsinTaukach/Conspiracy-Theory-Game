// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGGrid.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"

ACTGGrid::ACTGGrid()
{
    PrimaryActorTick.bCanEverTick = true;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    GridMesh = CreateDefaultSubobject<UStaticMeshComponent>("GridMesh");
    check(GridMesh);
    GridMesh->SetupAttachment(Origin);
}

void ACTGGrid::BeginPlay()
{
    Super::BeginPlay();
}

void ACTGGrid::SetModel(const FSettings& InSettings, uint32 InCellSize)
{
    GridDim = InSettings.GridSize;
    CellSize = InCellSize;
    WorldWidth = GridDim.Width * CellSize;
    WorldHeight = GridDim.Height * CellSize;

    check(GridMesh->GetStaticMesh());
    const FBox Box = GridMesh->GetStaticMesh()->GetBoundingBox();
    const FVector Size = Box.GetSize();

    check(Size.X);
    check(Size.Y);
    GridMesh->SetRelativeScale3D(FVector(WorldHeight / Size.X, WorldWidth / Size.Y, 1.0));
    GridMesh->SetRelativeLocation(0.5 * FVector(WorldHeight, WorldWidth, -Size.Z));
}

void ACTGGrid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawGrid();
}

void ACTGGrid::DrawGrid()
{
    for (uint32 i = 0; i < GridDim.Height + 1; ++i)
    {
        const FVector StartLocation = GetActorLocation() + GetActorForwardVector() * CellSize * i;
        const FVector EndLocation = StartLocation + GetActorRightVector() * WorldWidth;
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0, 0, 2);
    }

    for (uint32 j = 0; j < GridDim.Width + 1; ++j)
    {
        const FVector StartLocation = GetActorLocation() + GetActorRightVector() * CellSize * j;
        const FVector EndLocation = StartLocation + GetActorForwardVector() * WorldHeight;
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0, 0, 2);
    }
}