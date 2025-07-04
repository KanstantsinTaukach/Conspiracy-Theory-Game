// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CTGVisualCharacter.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGVisualCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ACTGVisualCharacter();

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetCharacterHealth() const { return CharacterHealth; };

    UFUNCTION(BlueprintCallable, Category = "Health")
    void RemoveCharacterHealth(float Delta);

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return CharacterHealth / CharacterMaxHealth; };

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float CharacterHealth = 1000;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float CharacterMaxHealth = 1000;

    virtual void BeginPlay() override;
};
