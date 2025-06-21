// Team Development of a Conspiracy Theory Game for GameBOX.

#include "MainMenu/CTGMainMenuGameModeBase.h"
#include "MainMenu/CTGMainMenuPlayerController.h"
#include "MainMenu/UI/CTGManiMenuHUD.h"

ACTGMainMenuGameModeBase::ACTGMainMenuGameModeBase() 
{
    PlayerControllerClass = ACTGMainMenuPlayerController::StaticClass();
    HUDClass = ACTGManiMenuHUD::StaticClass();
}
