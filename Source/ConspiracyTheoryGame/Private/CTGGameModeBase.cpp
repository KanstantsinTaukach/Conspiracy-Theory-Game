// Team Development of a Conspiracy Theory Game for GameBOX.


#include "CTGGameModeBase.h"
#include "Player/CTGCharacter.h"
#include "Player/CTGPlayerController.h"

ACTGGameModeBase::ACTGGameModeBase() 
{
    DefaultPawnClass = ACTGCharacter::StaticClass();
    PlayerControllerClass = ACTGPlayerController::StaticClass();
}
