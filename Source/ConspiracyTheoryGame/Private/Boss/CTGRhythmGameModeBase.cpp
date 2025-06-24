// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/CTGRhythmGameModeBase.h"
#include "Boss/RhythmMechanics/CTGFallingKey.h"

ACTGRhythmGameModeBase::ACTGRhythmGameModeBase() {}

void ACTGRhythmGameModeBase::StartPlay() 
{
    Super::StartPlay();

    GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ACTGRhythmGameModeBase::SpawnRandomFallingKey, SpawnInterval, true);
}

void ACTGRhythmGameModeBase::SpawnRandomFallingKey() 
{
    ECTGKeyType RandomKeyType = static_cast<ECTGKeyType>(FMath::RandRange(0, 3));
    SpawnFallingKey(RandomKeyType);
}

void ACTGRhythmGameModeBase::SpawnFallingKey(ECTGKeyType Key) 
{
    if (UWorld* World = GetWorld())
    {
        FActorSpawnParameters SpawnParams;
        ACTGFallingKey* NewKey =
            World->SpawnActor<ACTGFallingKey>(ACTGFallingKey::StaticClass(), FVector(0.0f, 0.0f, 1100.0f), FRotator::ZeroRotator, SpawnParams);
        if (NewKey)
        {
            NewKey->SetKeyType(Key);
        }
    }
}