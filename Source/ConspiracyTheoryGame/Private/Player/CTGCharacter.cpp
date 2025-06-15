// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Player/CTGCharacter.h"
#include "Camera/CameraComponent.h"

ACTGCharacter::ACTGCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Cameracomponent");
    CameraComponent->SetupAttachment(GetRootComponent());
}

void ACTGCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ACTGCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACTGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
