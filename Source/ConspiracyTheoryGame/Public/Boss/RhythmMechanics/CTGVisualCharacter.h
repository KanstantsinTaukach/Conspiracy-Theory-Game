// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CTGVisualCharacter.generated.h"

//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGVisualCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ACTGVisualCharacter();

    //FOnHealthChangedSignature OnHealthChanged;

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetCharacterHealth() const { return CharacterHealth; };

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetHealth(float NewHealth);

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return CharacterHealth / CharacterMaxHealth; };

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float CharacterHealth = 1000;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float CharacterMaxHealth = 1000;

    virtual void BeginPlay() override;
};
