// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/CTGRhythmPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CTGRhythmGameModeBase.h"

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

void ACTGRhythmPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (const auto InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        InputSystem->AddMappingContext(MappingContext, 0);
    }

    if (GetWorld() && GetWorld()->GetAuthGameMode())
    {
        RhythmGameMode = Cast<ACTGRhythmGameModeBase>(GetWorld()->GetAuthGameMode());
    }
}

void ACTGRhythmPlayerController::OnUpPressed()
{
    if (RhythmGameMode)
    {
        RhythmGameMode->CheckPlayerInput(ECTGKeyType::ArrowUp);
    }
}

void ACTGRhythmPlayerController::OnDownPressed()
{
    if (RhythmGameMode)
    {
        RhythmGameMode->CheckPlayerInput(ECTGKeyType::ArrowDown);
    }
}

void ACTGRhythmPlayerController::OnLeftPressed()
{
    if (RhythmGameMode)
    {
        RhythmGameMode->CheckPlayerInput(ECTGKeyType::ArrowLeft);
    }
}

void ACTGRhythmPlayerController::OnRightPressed()
{
    if (RhythmGameMode)
    {
        RhythmGameMode->CheckPlayerInput(ECTGKeyType::ArrowRight);
    }
}
