// Team Development of a Conspiracy Theory Game for GameBOX.


#include "CTGGameModeBase.h"
#include "Player/CTGCharacter.h"
#include "Player/CTGPlayerController.h"
#include "UI/CTGGameHUD.h"

ACTGGameModeBase::ACTGGameModeBase() 
{
    DefaultPawnClass = ACTGCharacter::StaticClass();
    PlayerControllerClass = ACTGPlayerController::StaticClass();
    HUDClass = ACTGGameHUD::StaticClass();
}

void ACTGGameModeBase::StartPlay() 
{
    Super::StartPlay();

    SetMatchState(CTGMatchState::InProgress);
}

void ACTGGameModeBase::SetMatchState(CTGMatchState State)
{
    if (MatchState == State) return;

    MatchState = State;
    OnMatchStateChange.Broadcast(MatchState);
}
