// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/CTGRhythmGameModeBase.h"
#include "Boss/RhythmMechanics/CTGFallingKey.h"
#include "Boss/RhythmMechanics/CTGGrid.h"
#include "Boss/RhythmMechanics/CTGRhythmPawn.h"
#include "CTGCoreTypes.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"

ACTGRhythmGameModeBase::ACTGRhythmGameModeBase() {}

void ACTGRhythmGameModeBase::StartPlay()
{
    Super::StartPlay();

    // FSettings RhythmSettings;
    RhythmSettings.GridDims = FDim{GridDims.X, GridDims.Y};
    RhythmSettings.StartPosition = FPosition{0, 0};
    RhythmSettings.GameSpeed = GameSpeed;

    // Init CTGGrid
    const FTransform GridOrigin = FTransform::Identity;
    check(GetWorld());
    GridVisual = GetWorld()->SpawnActorDeferred<ACTGGrid>(GridVisualClass, GridOrigin);
    check(GridVisual);
    GridVisual->SetModel(RhythmSettings, CellSize);
    GridVisual->FinishSpawning(GridOrigin);

    // Set pawn location fitting grid in viewport
    auto* PC = GetWorld()->GetFirstPlayerController();
    check(PC);
    auto* MyPawn = Cast<ACTGRhythmPawn>(PC->GetPawn());
    check(MyPawn);
    MyPawn->UpdateLocation(RhythmSettings.GridDims, CellSize, GridOrigin);

    //
    FindFog();

    // Update colors
    check(ColorsTable);
    const auto RowsCount = ColorsTable->GetRowNames().Num();
    check(RowsCount >= 1);
    ColorTableIndex = FMath::RandRange(0, RowsCount - 1);
    UpdateColors();

    GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ACTGRhythmGameModeBase::SpawnRandomFallingKey, SpawnInterval, true);
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
        uint32 RandPositionX = FMath::RandRange(1, GridDims.X - 2);
        RhythmSettings.StartPosition = FPosition{RandPositionX, 1};

        const FTransform GridOrigin = FTransform::Identity;

        // Init falling actors
        FallingKeyVisual = GetWorld()->SpawnActorDeferred<ACTGFallingKey>(FallingKeyVisualClass, GridOrigin);
        check(FallingKeyVisual);
        FallingKeyVisual->SetModel(RhythmSettings, CellSize);
        FallingKeyVisual->SetKeyType(Key);
        FallingKeyVisual->FinishSpawning(GridOrigin);
    }
}