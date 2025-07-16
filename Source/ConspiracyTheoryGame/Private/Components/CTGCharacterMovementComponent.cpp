// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Components/CTGCharacterMovementComponent.h"
#include "Player/CTGCharacter.h"

float UCTGCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();

    const auto* Player = Cast<ACTGCharacter>(GetPawnOwner());
    return Player && Player->IsCharacterRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}