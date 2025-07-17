// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CTGVisualCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGVisualCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ACTGVisualCharacter();

    FOnHealthChangedSignature OnHealthChanged;
    FOnDeathSignature OnDeath;

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetCharacterHealth() const { return CharacterHealth; };

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetHealth(float NewHealth);

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return CharacterHealth / CharacterMaxHealth; };

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return CharacterHealth <= 0.0f; };

    UFUNCTION()
    void StopAllCharacterAnimations();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float CharacterHealth = 1000;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float CharacterMaxHealth = 1000;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TArray<UAnimMontage*> DanceAnimations;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TObjectPtr<UAnimMontage> DamageAnimMontage;

    virtual void BeginPlay() override;

private:
    TObjectPtr<UAnimMontage> CurrentDanceAnimMontage;

    void PlayDanceAnimation();

    UFUNCTION()
    void OnAnimationEnded(UAnimMontage* Montage, bool bInterrupted);
};
