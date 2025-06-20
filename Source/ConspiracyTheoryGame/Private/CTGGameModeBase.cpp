// Team Development of a Conspiracy Theory Game for GameBOX.


#include "CTGGameModeBase.h"
#include "Player/CTGCharacter.h"
#include "Player/CTGPlayerController.h"
#include "Player/CTGPlayerState.h"
#include "UI/CTGGameHUD.h"

ACTGGameModeBase::ACTGGameModeBase() 
{
    DefaultPawnClass = ACTGCharacter::StaticClass();
    PlayerControllerClass = ACTGPlayerController::StaticClass();
    HUDClass = ACTGGameHUD::StaticClass();
    PlayerStateClass = ACTGPlayerState::StaticClass();
}

void ACTGGameModeBase::StartPlay() 
{
    Super::StartPlay();

    SetMatchState(CTGMatchState::InProgress);
}

bool ACTGGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const bool PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSet)
    {
        SetMatchState(CTGMatchState::Pause);
    }

    return PauseSet;
}

bool ACTGGameModeBase::ClearPause()
{
    const bool PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetMatchState(CTGMatchState::InProgress);
    }

    return PauseCleared;
}

void ACTGGameModeBase::SetMatchState(CTGMatchState State)
{
    if (MatchState == State) return;

    MatchState = State;
    OnMatchStateChanged.Broadcast(MatchState);
}
