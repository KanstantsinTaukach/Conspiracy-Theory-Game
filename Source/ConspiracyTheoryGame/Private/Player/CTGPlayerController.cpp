// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Player/CTGPlayerController.h"
#include "EnhancedInputComponent.h"
#include "CTGGameModeBase.h"

void ACTGPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        const auto GameMode = Cast<ACTGGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ACTGPlayerController::OnMatchStateChanged);
        }
    }
}

void ACTGPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Triggered, this, &ACTGPlayerController::OnPauseGame);
    }
}

void ACTGPlayerController::OnPauseGame()
{
    SetPause(!IsPaused());
}

void ACTGPlayerController::OnMatchStateChanged(ECTGMatchState State)
{
    if (State == ECTGMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
        bShowMouseCursor = true;
    }
}
