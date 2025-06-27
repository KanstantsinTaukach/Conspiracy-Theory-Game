// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/CTGRhythmGameModeBase.h"
#include "Boss/RhythmMechanics/CTGFallingKey.h"
#include "CTGCoreTypes.h"
#include "RhythmMechanics/CTGGrid.h"
#include "RhythmMechanics/CTGRhythmPawn.h"

ACTGRhythmGameModeBase::ACTGRhythmGameModeBase() {}

void ACTGRhythmGameModeBase::StartPlay()
{
    Super::StartPlay();

    // Init CTGGrid
    const FTransform GridOrigin = FTransform::Identity;
    check(GetWorld());
    GridVisual = GetWorld()->SpawnActorDeferred<ACTGGrid>(GridVisualClass, GridOrigin);

    check(GridVisual);
    const FSettings MySettings{GridSize.X, GridSize.Y};
    GridVisual->SetModel(MySettings, CellSize);
    GridVisual->FinishSpawning(GridOrigin);

    // Set pawn location fitting grid in viewport
    const auto PC = GetWorld()->GetFirstPlayerController();
    check(PC);

    const auto MyPawn = Cast<ACTGRhythmPawn>(PC->GetPawn());
    check(MyPawn);
    MyPawn->UpdateLocation(MySettings.GridSize, CellSize, GridOrigin);

    // GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ACTGRhythmGameModeBase::SpawnRandomFallingKey, SpawnInterval, true);
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
        ACTGFallingKey* NewKey = World->SpawnActor<ACTGFallingKey>(
            ACTGFallingKey::StaticClass(), FVector(0.0f, 0.0f, 1100.0f), FRotator::ZeroRotator, SpawnParams);
        if (NewKey)
        {
            NewKey->SetKeyType(Key);
        }
    }
}