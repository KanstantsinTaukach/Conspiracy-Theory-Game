// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGVisualCharacter.h"

ACTGVisualCharacter::ACTGVisualCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    CharacterHealth = CharacterMaxHealth;
}

void ACTGVisualCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CharacterMaxHealth > 0);
}

void ACTGVisualCharacter::SetHealth(float NewHealth)
{
    const auto NextHealt = FMath::Clamp(NewHealth, 0.0f, CharacterMaxHealth);
    const auto HealthDelta = NextHealt - CharacterHealth;
    CharacterHealth = NextHealt;

    //OnHealthChanged.Broadcast(CharacterHealth, HealthDelta);
}