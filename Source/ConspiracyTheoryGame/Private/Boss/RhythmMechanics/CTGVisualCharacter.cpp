// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGVisualCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

ACTGVisualCharacter::ACTGVisualCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    CharacterHealth = CharacterMaxHealth;

    VoiceComponent = CreateDefaultSubobject<UAudioComponent>("VoiceComponent");
    VoiceComponent->SetupAttachment(RootComponent);
    VoiceComponent->bAutoActivate = false;
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

        if (DamageSounds.Num() > 0)
        {
            USoundCue* RandomSound = DamageSounds[FMath::RandRange(0, DamageSounds.Num() - 1)];
            UGameplayStatics::PlaySound2D(GetWorld(), RandomSound);
        }
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

    int8 CurrentIndex = 0;

    if (DanceAnimations.Num() > 0)
    {
        do
        {
            CurrentIndex = FMath::RandRange(0, DanceAnimations.Num() - 1);
        } while (CurrentIndex == LastAnimationIndex);

        CurrentDanceAnimMontage = DanceAnimations[CurrentIndex];
        LastAnimationIndex = CurrentIndex;
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