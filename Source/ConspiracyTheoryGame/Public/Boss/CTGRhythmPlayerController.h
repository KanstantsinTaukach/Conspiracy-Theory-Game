// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CTGRhythmPlayerController.generated.h"

class UInputAction;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGRhythmPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void SetupInputComponent() override;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* PressUp;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* PressDown;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* PressLeft;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* PressRight;

    UFUNCTION()
    void OnUpPressed();

    UFUNCTION()
    void OnDownPressed();

    UFUNCTION()
    void OnLeftPressed();

    UFUNCTION()
    void OnRightPressed();
};
