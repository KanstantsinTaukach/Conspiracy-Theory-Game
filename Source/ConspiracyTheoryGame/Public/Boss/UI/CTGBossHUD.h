// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CTGBossHUD.generated.h"

class UCTGBossGameplayWidget;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGBossHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UCTGBossGameplayWidget> BossGameplayWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TObjectPtr<UCTGBossGameplayWidget> BossGameplayWidget;
};
