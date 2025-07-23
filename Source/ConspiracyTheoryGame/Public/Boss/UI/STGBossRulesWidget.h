// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STGBossRulesWidget.generated.h"

class UButton;

UCLASS()
class CONSPIRACYTHEORYGAME_API USTGBossRulesWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> StartBattleButton;

    void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnStartBattle();
};
