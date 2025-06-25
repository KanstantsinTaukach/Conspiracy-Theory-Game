// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/CTGRhythmPlayerController.h"
#include "EnhancedInputComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogTGRhythmPlayerController, All, All);

void ACTGRhythmPlayerController::SetupInputComponent() 
{
    Super::SetupInputComponent();

    if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(PressUp, ETriggerEvent::Triggered, this, &ACTGRhythmPlayerController::OnUpPressed);
        EnhancedInputComponent->BindAction(PressDown, ETriggerEvent::Triggered, this, &ACTGRhythmPlayerController::OnDownPressed);
        EnhancedInputComponent->BindAction(PressLeft, ETriggerEvent::Triggered, this, &ACTGRhythmPlayerController::OnLeftPressed);
        EnhancedInputComponent->BindAction(PressRight, ETriggerEvent::Triggered, this, &ACTGRhythmPlayerController::OnRightPressed);
    }
}

void ACTGRhythmPlayerController::OnUpPressed() 
{
    UE_LOG(LogTGRhythmPlayerController, Display, TEXT("UP"));
}

void ACTGRhythmPlayerController::OnDownPressed() 
{
    UE_LOG(LogTGRhythmPlayerController, Display, TEXT("DOWN"));
}

void ACTGRhythmPlayerController::OnLeftPressed() 
{
    UE_LOG(LogTGRhythmPlayerController, Display, TEXT("LEFT"));
}

void ACTGRhythmPlayerController::OnRightPressed() 
{
    UE_LOG(LogTGRhythmPlayerController, Display, TEXT("RIGHT"));
}
