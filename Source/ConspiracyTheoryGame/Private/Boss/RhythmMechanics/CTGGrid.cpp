// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGGrid.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"

ACTGGrid::ACTGGrid()
{
    PrimaryActorTick.bCanEverTick = false;

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

    // Scale mesh
    check(GridMesh->GetStaticMesh());
    const FBox Box = GridMesh->GetStaticMesh()->GetBoundingBox();
    const FVector Size = Box.GetSize();

    check(Size.X);
    check(Size.Y);
    GridMesh->SetRelativeScale3D(FVector(WorldHeight / Size.X, WorldWidth / Size.Y, 1.0));
    GridMesh->SetRelativeLocation(0.5 * FVector(WorldHeight, WorldWidth, -Size.Z));

    // Setup material
    GridMaterial = GridMesh->CreateAndSetMaterialInstanceDynamic(0);
    if (GridMaterial)
    {
        GridMaterial->SetVectorParameterValue("Division", FVector(GridDim.Height, GridDim.Width, 0.0));
    }
}

void ACTGGrid::UpdateColors(const FGridColors& Colors) 
{
    if (GridMaterial)
    {
        GridMaterial->SetVectorParameterValue("BackgroundColor", Colors.GridBackgroundColor);
        GridMaterial->SetVectorParameterValue("LineColor", Colors.GridLineColor);
        GridMaterial->SetVectorParameterValue("WallColor", Colors.GridWallColor);
    }
}

void ACTGGrid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //DrawGrid();
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