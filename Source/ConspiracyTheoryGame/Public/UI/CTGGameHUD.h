// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CTGCoreTypes.h"
#include "CTGGameHUD.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGGameHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PauseWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<ECTGMatchState, UUserWidget*> GameWidgets;

    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentWidget = nullptr;

    void OnMatchStateChanged(ECTGMatchState State);
};
