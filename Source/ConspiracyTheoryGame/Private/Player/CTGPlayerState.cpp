// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Player/CTGPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "CTGGameInstance.h"

void ACTGPlayerState::BeginPlay()
{
    Super::BeginPlay();

    auto* CTGGameInstance = GetWorld()->GetGameInstance<UCTGGameInstance>();
    if (CTGGameInstance)
    {
        AddPoints(CTGGameInstance->GetPlayerScore());
    }
}

void ACTGPlayerState::AddPoints(int32 Delta)
{
    if (!ensure(Delta >= 0)) return;

    PreviousPoints = Points;
    Points += Delta;

    if (Points >= PointsToUnlockBoss)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), DoorUnlockedSound);
    }

    OnPointsChanged.Broadcast(this, Points, Delta);
}

bool ACTGPlayerState::RemovePoints(int32 Delta)
{
    if (!ensure(Delta > 0)) return false;

    PreviousPoints = Points;
    Points -= Delta;
    if (Points < 0)
    {
        Points = 0;
    }

    OnPointsChanged.Broadcast(this, Points, Delta);

    return true;
}