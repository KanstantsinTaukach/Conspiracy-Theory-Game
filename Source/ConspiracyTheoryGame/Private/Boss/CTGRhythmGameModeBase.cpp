// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/CTGRhythmGameModeBase.h"
#include "Boss/RhythmMechanics/CTGFallingKey.h"
#include "CTGCoreTypes.h"
#include "RhythmMechanics/CTGGrid.h"
#include "RhythmMechanics/CTGRhythmPawn.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"

ACTGRhythmGameModeBase::ACTGRhythmGameModeBase() {}

void ACTGRhythmGameModeBase::StartPlay()
{
    Super::StartPlay();

    // Init CTGGrid
    const FTransform GridOrigin = FTransform::Identity;
    check(GetWorld());
    GridVisual = GetWorld()->SpawnActorDeferred<ACTGGrid>(GridVisualClass, GridOrigin);

    check(GridVisual);
    const FSettings MySettings{GridDims.X, GridDims.Y};
    GridVisual->SetModel(MySettings, CellSize);
    GridVisual->FinishSpawning(GridOrigin);

    // Set pawn location fitting grid in viewport
    auto* PC = GetWorld()->GetFirstPlayerController();
    check(PC);

    auto* MyPawn = Cast<ACTGRhythmPawn>(PC->GetPawn());
    check(MyPawn);
    MyPawn->UpdateLocation(MySettings.GridSize, CellSize, GridOrigin);

    //
    FindFog();

    // Update colors
    check(ColorsTable);
    const auto RowsCount = ColorsTable->GetRowNames().Num();
    check(RowsCount >= 1);
    ColorTableIndex = FMath::RandRange(0, RowsCount - 1);
    UpdateColors();

    // GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ACTGRhythmGameModeBase::SpawnRandomFallingKey, SpawnInterval, true);
}

void ACTGRhythmGameModeBase::FindFog() 
{
    TArray<AActor*> Fogs;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExponentialHeightFog::StaticClass(), Fogs);
    if (Fogs.Num() > 0)
    {
        Fog = Cast<AExponentialHeightFog>(Fogs[0]);
    }
}

void ACTGRhythmGameModeBase::UpdateColors()
{
    const auto RowName = ColorsTable->GetRowNames()[ColorTableIndex];
    const auto* ColorSet = ColorsTable->FindRow<FGridColors>(RowName, {});
    if (ColorSet)
    {
        // Update grid
        GridVisual->UpdateColors(*ColorSet);

        // Update scene ambient color via fog
        if (Fog && Fog->GetComponent())
        {
            Fog->GetComponent()->SkyAtmosphereAmbientContributionColorScale = ColorSet->SkyAtmosphereColor;
            Fog->MarkComponentsRenderStateDirty();
        }
    }
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