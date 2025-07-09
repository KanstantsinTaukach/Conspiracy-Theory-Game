// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Player/CTGPlayerController.h"
#include "CTGRhythmPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class ACTGRhythmGameModeBase;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGRhythmPlayerController : public ACTGPlayerController
{
    GENERATED_BODY()

protected:
    virtual void SetupInputComponent() override;

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* PressUp;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* PressDown;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* PressLeft;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* PressRight;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* MappingContext;

    UFUNCTION()
    void OnUpPressed();

    UFUNCTION()
    void OnDownPressed();

    UFUNCTION()
    void OnLeftPressed();

    UFUNCTION()
    void OnRightPressed();

    private:
    ACTGRhythmGameModeBase* RhythmGameMode;
};
