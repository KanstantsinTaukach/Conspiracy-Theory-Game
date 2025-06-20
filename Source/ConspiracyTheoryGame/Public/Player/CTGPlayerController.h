// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CTGCoreTypes.h"
#include "CTGPlayerController.generated.h"

class UInputAction;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* PauseGameAction;

    void OnPauseGame();

    void OnMatchStateChanged(ECTGMatchState State);
};
