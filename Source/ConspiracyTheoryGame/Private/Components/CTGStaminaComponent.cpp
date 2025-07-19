// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Components/CTGStaminaComponent.h"

UCTGStaminaComponent::UCTGStaminaComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCTGStaminaComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxStamina > 0.0f);

    SetStamina(MaxStamina);
}

void UCTGStaminaComponent::ChangeSpamina(bool IsSprinting) 
{
    if (IsSprinting)
    {
        GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &UCTGStaminaComponent::DrainStamina, StaminaUpdateTime, true);
    }
    else
    {
        GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &UCTGStaminaComponent::RegenerateStamina, StaminaUpdateTime, true);
    }
}

void UCTGStaminaComponent::DrainStamina() 
{
    if (GetWorld())
    {
        if (Stamina > 0.0f)
        {
            SetStamina(Stamina - StaminaDrainRate);
        }
        else if (IsStaminaEmpty())
        {
            GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
            OnStaminaEmpty.Broadcast();
        }
    }
}

void UCTGStaminaComponent::RegenerateStamina() 
{
    if (GetWorld())
    {
        if (Stamina < MaxStamina)
        {
            SetStamina(Stamina + StaminaRegenRate);
        }
        else if (FMath::IsNearlyEqual(Stamina, MaxStamina))
        {
            GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
        }
    }
}

void UCTGStaminaComponent::SetStamina(float NewStamina) 
{
    Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
    OnStaminaChanged.Broadcast(Stamina);
}

void UCTGStaminaComponent::StopStaminaRegeneration() 
{
    GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
}