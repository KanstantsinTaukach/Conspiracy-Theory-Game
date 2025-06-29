// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGFallingKey.h"
#include "Components/StaticMeshComponent.h"

ACTGFallingKey::ACTGFallingKey()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACTGFallingKey::SetModel(const FSettings& InSettings, uint32 InCellSize) 
{
    Settings = InSettings;
    CellSize = InCellSize;
}

void ACTGFallingKey::BeginPlay() 
{
    Super::BeginPlay();

    const FTransform Transform = FTransform(ActorPositionToVector(Settings.StartPosition, CellSize, Settings.GridDims));
    auto* FallingKeyActor = GetWorld()->SpawnActor<AActor>(FallingKeyClass, Transform);
}

void ACTGFallingKey::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    for (auto& FallingActor : FallingActors)
    {
        FallingActor->SetActorLocation(ActorPositionToVector(Settings.StartPosition, CellSize, Settings.GridDims));
    }
}

FVector ACTGFallingKey::ActorPositionToVector(FPosition& InPosition, uint32 InCellSize, FDim& InDim) 
{
    return FVector((Settings.GridDims.Height - 1 - InPosition.Y) * InCellSize, InPosition.X * InCellSize, 0.0) + FVector(CellSize * 0.5);
}

void ACTGFallingKey::SetKeyType(ECTGKeyType Key) 
{
    KeyType = Key;
}

void ACTGFallingKey::CheckHit() {}

void ACTGFallingKey::OnMissed() {}