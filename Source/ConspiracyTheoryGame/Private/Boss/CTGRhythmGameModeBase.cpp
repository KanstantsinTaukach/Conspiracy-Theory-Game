// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/CTGRhythmGameModeBase.h"
#include "Boss/RhythmMechanics/CTGFallingKey.h"
#include "Boss/RhythmMechanics/CTGGrid.h"
#include "Boss/RhythmMechanics/CTGRhythmPawn.h"
#include "CTGCoreTypes.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/CTGRhythmPlayerController.h"
#include "Boss/UI/CTGBossHUD.h"
#include "Boss/RhythmMechanics/CTGVisualCharacter.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogCTGRhythmGameModeBase, All, All);

ACTGRhythmGameModeBase::ACTGRhythmGameModeBase()
{
    DefaultPawnClass = ACTGRhythmPawn::StaticClass();
    PlayerControllerClass = ACTGRhythmPlayerController::StaticClass();
    HUDClass = ACTGBossHUD::StaticClass();
}

void ACTGRhythmGameModeBase::StartPlay()
{
    Super::StartPlay();

    SetMatchState(ECTGMatchState::FightingWithBoss);

    // FSettings RhythmSettings;
    RhythmSettings.GridDims = FDim{GridDims.X, GridDims.Y};
    RhythmSettings.ActorPosition = FPosition{0, 0};
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
    auto* RhythmPawn = Cast<ACTGRhythmPawn>(PC->GetPawn());
    check(RhythmPawn);
    RhythmPawn->UpdateLocation(RhythmSettings.GridDims, CellSize, GridOrigin);

    //
    FindFog();

    // Update colors
    check(ColorsTable);
    const auto RowsCount = ColorsTable->GetRowNames().Num();
    check(RowsCount >= 1);
    ColorTableIndex = FMath::RandRange(0, RowsCount - 1);
    UpdateColors();

    // Spawn Player and Boss
    int32 TargetOffset = (RhythmSettings.GridDims.Width * 0.5 + VisualCharacterOffset) * CellSize;

    PlayerCharacter =
        Cast<ACTGVisualCharacter>(RhythmPawn->SpawnVisualCharacter(PlayerCharacterClass, -TargetOffset, FRotator(33.0, 90.0f, -90.0)));
    BossCharacter =
        Cast<ACTGVisualCharacter>(RhythmPawn->SpawnVisualCharacter(BossCharacterClass, TargetOffset, FRotator(33.0, -90.0f, 90.0)));

    PlayerCharacter->OnDeath.AddUObject(this, &ACTGRhythmGameModeBase::OnPlayerCharacterDeath);
    BossCharacter->OnDeath.AddUObject(this, &ACTGRhythmGameModeBase::OnBossCharacterDeath);

    if (StartGameSound && !GameMusicComponent)
    {
        GameMusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), StartGameSound);
    }
    // Spawn Falling Keys
    GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ACTGRhythmGameModeBase::SpawnRandomFallingKey, SpawnInterval, true, TimerDelay);
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
        uint32 RandPositionX;
        do
        {
            RandPositionX = FMath::RandRange(0, GridDims.X - 1);
        } while (RandPositionX == LastActorPositionX);

        RhythmSettings.ActorPosition = FPosition{RandPositionX, 0};
        LastActorPositionX = RhythmSettings.ActorPosition.X;

        const FTransform GridOrigin = FTransform::Identity;

        const auto RowName = ColorsTable->GetRowNames()[ColorTableIndex];
        const auto* ColorSet = ColorsTable->FindRow<FGridColors>(RowName, {});

        // Init falling actors
        FallingKeyVisual = GetWorld()->SpawnActorDeferred<ACTGFallingKey>(FallingKeyVisualClass, GridOrigin);
        check(FallingKeyVisual);
        FallingKeyVisual->SetModel(RhythmSettings, CellSize);
        FallingKeyVisual->SetKeyType(Key);
        FallingKeyVisual->UpdateScale(CellSize);
        FallingKeyVisual->UpdateColors(*ColorSet);
        FallingKeyVisual->FinishSpawning(GridOrigin);

        ActiveFallingKeys.Add(FallingKeyVisual);

        FallingKeyVisual->OnDestroyed.AddDynamic(this, &ACTGRhythmGameModeBase::RemoveFallingKey);
    }
}

void ACTGRhythmGameModeBase::CheckPlayerInput(ECTGKeyType InputKey)
{
    if (ActiveFallingKeys.Num() == 0) return;

    ACTGFallingKey* LowestKey = nullptr;
    uint32 MinPositionY = 0;

    for (auto* Key : ActiveFallingKeys)
    {
        if (Key->GetCurrentPosition().Y > MinPositionY)
        {
            MinPositionY = Key->GetCurrentPosition().Y;
            LowestKey = Key;
        }
    }

    if (LowestKey)
    {
        float Damage;
        bool bIsDamageToBoss = LowestKey->GetZoneDamage(Damage);

        if (LowestKey->GetKeyType() == InputKey && bIsDamageToBoss)
        {
            bIsDamageToBoss = true;

            if (BossCharacter)
            {
                BossCharacter->SetHealth(BossCharacter->GetCharacterHealth() - Damage);
            }
        }
        else
        {
            bIsDamageToBoss = false;

            if (PlayerCharacter)
            {
                PlayerCharacter->SetHealth(PlayerCharacter->GetCharacterHealth() - Damage);
            }
        }

        LowestKey->OnGetZoneDamage.Broadcast(bIsDamageToBoss, Damage);
        RemoveFallingKey(LowestKey);
        LowestKey->DestroyFallingKey();
    }
}

void ACTGRhythmGameModeBase::RemoveFallingKey(AActor* DestroyedActor)
{
    if (auto* FallingKey = Cast<ACTGFallingKey>(DestroyedActor))
    {
        ActiveFallingKeys.Remove(FallingKey);
    }
}

void ACTGRhythmGameModeBase::OnPlayerCharacterDeath()
{
    if (PlayerCharacter && PlayerCharacter->IsDead())
    {
        SetMatchState(ECTGMatchState::GameOver);

        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
    }
}

void ACTGRhythmGameModeBase::OnBossCharacterDeath()
{
    if (BossCharacter && BossCharacter->IsDead())
    {
        SetMatchState(ECTGMatchState::PlayerWin);

        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
    }
}

bool ACTGRhythmGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const bool PauseSet = Super::SetPause(PC, CanUnpauseDelegate);

    if (PauseSet && GameMusicComponent)
    {
        GameMusicComponent->SetPaused(true);
    }

    return PauseSet;
}

bool ACTGRhythmGameModeBase::ClearPause()
{
    const bool PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetMatchState(ECTGMatchState::FightingWithBoss);

        if (GameMusicComponent)
        {
            GameMusicComponent->SetPaused(false);
        }
    }

    return PauseCleared;
}