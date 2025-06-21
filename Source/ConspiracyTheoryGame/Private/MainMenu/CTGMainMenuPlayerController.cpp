// Team Development of a Conspiracy Theory Game for GameBOX.


#include "MainMenu/CTGMainMenuPlayerController.h"

void ACTGMainMenuPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
}
