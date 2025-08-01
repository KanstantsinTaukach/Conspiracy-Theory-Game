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

    SetMatchState(ECTGMatchState::InProgress);
}

bool ACTGGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    if (MatchState == ECTGMatchState::PlayerWin || MatchState == ECTGMatchState::GameOver || MatchState == ECTGMatchState::WaitingToStart) return false;

    const bool PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSet)
    {
        SetPreviousMatchState(MatchState);
        SetMatchState(ECTGMatchState::Pause);
    }

    return PauseSet;
}

bool ACTGGameModeBase::ClearPause()
{
    const bool PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetMatchState(GetPreviousMatchState());
    }

    return PauseCleared;
}

void ACTGGameModeBase::SetMatchState(ECTGMatchState State)
{
    if (MatchState == State) return;

    MatchState = State;
    OnMatchStateChanged.Broadcast(MatchState);
}
