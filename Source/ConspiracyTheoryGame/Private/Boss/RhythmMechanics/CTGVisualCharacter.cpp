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

void ACTGVisualCharacter::RemoveCharacterHealth(float Delta)
{
    CharacterHealth = FMath::Max(0, CharacterHealth - Delta);
}