// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTGGameOverWidget.generated.h"

class UButton;

UCLASS()
class CONSPIRACYTHEORYGAME_API UCTGGameOverWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* RestartLevelButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnRestartLevel();
};
