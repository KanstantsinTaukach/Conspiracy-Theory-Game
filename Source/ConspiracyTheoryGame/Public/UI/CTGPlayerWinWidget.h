// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTGPlayerWinWidget.generated.h"

class UButton;

UCLASS()
class CONSPIRACYTHEORYGAME_API UCTGPlayerWinWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr <UButton> RestartLevelButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr <UButton> RestartGameButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnRestartLevel();

    UFUNCTION()
    void OnRestartGame();
};
