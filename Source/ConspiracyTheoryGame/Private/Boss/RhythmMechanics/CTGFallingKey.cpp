// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGFallingKey.h"
#include "Components/StaticMeshComponent.h"
#include "CTGRhythmGameModeBase.h"

ACTGFallingKey::ACTGFallingKey()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACTGFallingKey::SetModel(const FSettings& InSettings, uint32 InCellSize)
{
    Settings = InSettings;
    Settings.GameSpeed = FMath::RandRange(Settings.GameSpeed - Delta, Settings.GameSpeed + Delta);
    CellSize = InCellSize;
}

void ACTGFallingKey::BeginPlay()
{
    Super::BeginPlay();

    if (FallingKeyClass)
    {
        const FTransform Transform = FTransform(ActorPositionToVector(Settings.ActorPosition, CellSize, Settings.GridDims));
        KeyMeshActor = GetWorld()->SpawnActor<AActor>(FallingKeyClass, Transform);
    }
}

void ACTGFallingKey::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeSinceLastMove += DeltaTime;

    if (TimeSinceLastMove >= Settings.GameSpeed)
    {
        UpdateActorPosition();
        TimeSinceLastMove = 0.0f;
    }
}

void ACTGFallingKey::UpdateActorPosition()
{
    if (!KeyMeshActor) return;
    Settings.ActorPosition.Y = Settings.ActorPosition.Y + 1;

    KeyMeshActor->SetActorLocation(ActorPositionToVector(Settings.ActorPosition, CellSize, Settings.GridDims));

    if (Settings.ActorPosition.Y == Settings.GridDims.Height - 1)
    {
        OnMissed();
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

void ACTGFallingKey::CheckHit() 
{
    if (KeyMeshActor)
    {
        KeyMeshActor->Destroy();
    }

    OnFallingKeyDestroyed.Broadcast(this);
    Destroy();
}

void ACTGFallingKey::OnMissed() 
{
    if (KeyMeshActor)
    {
        KeyMeshActor->Destroy();
    }

    if (auto* GameMode = Cast<ACTGRhythmGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        GameMode->RemovePlayerHealth(50);
    }

    OnFallingKeyDestroyed.Broadcast(this);
    Destroy();
}