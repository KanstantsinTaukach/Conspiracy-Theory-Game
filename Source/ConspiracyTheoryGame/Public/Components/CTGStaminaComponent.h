// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTGStaminaComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStaminaEmptySignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChangedSignature, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CONSPIRACYTHEORYGAME_API UCTGStaminaComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCTGStaminaComponent();

    float GetStamina() const { return Stamina; };
    bool CanSprinting() const { return Stamina > MinStaminaToSprint; };
    void ChangeSpamina(bool IsSprinting);

    UFUNCTION(BlueprintCallable)
    float GetStaminaPercent() const { return Stamina / MaxStamina; };

    void StopStaminaRegeneration();

    UFUNCTION(BlueprintCallable)
    bool IsStaminaEmpty() const { return FMath::IsNearlyZero(Stamina); };

    FOnStaminaEmptySignature OnStaminaEmpty;
    FOnStaminaChangedSignature OnStaminaChanged;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = "0.0", ClampMax = "20.0"))
    float MaxStamina = 20.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = "0.01", ClampMax = "0.5"))
    float StaminaDrainRate = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = "0.01", ClampMax = "0.5"))
    float StaminaRegenRate = 0.02f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = "5", ClampMax = "10.0"))
    float MinStaminaToSprint = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    float StaminaUpdateTime = 0.01f;

    virtual void BeginPlay() override;

private:
    float Stamina = 0.0f;

    void SetStamina(float NewStamina);
    void DrainStamina();
    void RegenerateStamina();

    FTimerHandle StaminaTimerHandle;
};
