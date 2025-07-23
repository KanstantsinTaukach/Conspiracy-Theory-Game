// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CTGVisualCharacter.generated.h"

class USoundCue;
class UAudioComponent;

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

    UFUNCTION()
    float GetMaxHealth() const { return CharacterMaxHealth; };

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return CharacterHealth <= 0.0f; };

    UFUNCTION()
    void StopAllCharacterAnimations();

    UFUNCTION()
    UAudioComponent* GetVoiceComponent() const { return VoiceComponent; };

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    float CharacterMaxHealth = 1500.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TArray<UAnimMontage*> DanceAnimations;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TObjectPtr<UAnimMontage> DamageAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    TArray<USoundCue*> DanceSounds;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    TArray<USoundCue*> DamageSounds;

    virtual void BeginPlay() override;

private:
    float CharacterHealth = 0.0f;

    int8 LastAnimationIndex = 0;

    UPROPERTY()
    TObjectPtr<UAnimMontage> CurrentDanceAnimMontage;

    UPROPERTY()
    TObjectPtr<UAudioComponent> VoiceComponent;

    void PlayDanceAnimation();

    UFUNCTION()
    void OnAnimationEnded(UAnimMontage* Montage, bool bInterrupted);
};
