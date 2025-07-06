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

    PlayDanceAnimation();
}

void ACTGVisualCharacter::SetHealth(float NewHealth)
{
    if (IsDead()) return;

    const auto NextHealt = FMath::Clamp(NewHealth, 0.0f, CharacterMaxHealth);
    const auto HealthDelta = NextHealt - CharacterHealth;
    CharacterHealth = NextHealt;

    OnHealthChanged.Broadcast(CharacterHealth, HealthDelta);

    if (HealthDelta < 0.0f && DamageAnimMontage)
    {
        StopAnimMontage(DanceAnimMontage);

        float Duration = PlayAnimMontage(DamageAnimMontage);
        if (Duration > 0.0f)
        {
            GetWorld()->GetTimerManager().ClearTimer(DanceRestartTimerHandle);
            GetWorld()->GetTimerManager().SetTimer(DanceRestartTimerHandle, this, &ACTGVisualCharacter::PlayDanceAnimation, Duration, false);            
        }
        else
        {
            PlayDanceAnimation();
        }
    }

    if (IsDead())
    {
        StopAnimMontage(DanceAnimMontage);
        StopAnimMontage(DamageAnimMontage);
        GetWorld()->GetTimerManager().ClearTimer(DanceRestartTimerHandle);

        OnDeath.Broadcast();
    }
}

void ACTGVisualCharacter::PlayDanceAnimation()
{
    if (IsDead()) return;

    if (DanceAnimMontage)
    {
        PlayAnimMontage(DanceAnimMontage);
    }
}