// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CTGCharacterMovementComponent.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API UCTGCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "1.1", ClampMax = "5"), Category = "Movement")
    float RunModifier = 2.0f;

    virtual float GetMaxSpeed() const override;
};