// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGVisualCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"

ACTGVisualCharacter::ACTGVisualCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    CharacterHealth = CharacterMaxHealth;
}

void ACTGVisualCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CharacterMaxHealth > 0);

    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        AnimInstance->OnMontageEnded.AddDynamic(this, &ACTGVisualCharacter::OnAnimationEnded);
    }

    PlayDanceAnimation();
}

void ACTGVisualCharacter::SetHealth(float NewHealth)
{
    if (IsDead()) return;

    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, CharacterMaxHealth);
    const auto HealthDelta = NextHealth - CharacterHealth;
    CharacterHealth = NextHealth;

    OnHealthChanged.Broadcast(CharacterHealth, HealthDelta);

    if (HealthDelta < 0.0f && DamageAnimMontage)
    {
        if (CurrentDanceAnimMontage)
        {
            StopAnimMontage(CurrentDanceAnimMontage);
        }

        PlayAnimMontage(DamageAnimMontage);
    }

    if (IsDead())
    {
        StopAllCharacterAnimations();

        OnDeath.Broadcast();
    }
}

void ACTGVisualCharacter::PlayDanceAnimation()
{
    if (IsDead()) return;

    if (DanceAnimations.Num() > 0)
    {
        int8 CurrentIndex = FMath::RandRange(0, DanceAnimations.Num() - 1);
        CurrentDanceAnimMontage = DanceAnimations[CurrentIndex];
    }

    if (CurrentDanceAnimMontage)
    {
        PlayAnimMontage(CurrentDanceAnimMontage);
    }
}

void ACTGVisualCharacter::OnAnimationEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (IsDead()) return;

    if (Montage == DamageAnimMontage)
    {
        PlayDanceAnimation();
    }
    else if (Montage == CurrentDanceAnimMontage && !bInterrupted)
    {
        PlayDanceAnimation();
    }
}

void ACTGVisualCharacter::StopAllCharacterAnimations() 
{
    StopAnimMontage(CurrentDanceAnimMontage);
    StopAnimMontage(DamageAnimMontage);
    CurrentDanceAnimMontage = nullptr;
}