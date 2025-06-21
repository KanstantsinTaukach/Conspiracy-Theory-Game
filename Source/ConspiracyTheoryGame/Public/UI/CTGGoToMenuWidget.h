// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTGGoToMenuWidget.generated.h"

class UButton;

UCLASS()
class CONSPIRACYTHEORYGAME_API UCTGGoToMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* GoToMenuButton;

    void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnGoToMenu();
};
