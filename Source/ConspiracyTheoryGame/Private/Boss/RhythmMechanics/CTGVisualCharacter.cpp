// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGVisualCharacter.h"

ACTGVisualCharacter::ACTGVisualCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACTGVisualCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ACTGVisualCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACTGVisualCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
