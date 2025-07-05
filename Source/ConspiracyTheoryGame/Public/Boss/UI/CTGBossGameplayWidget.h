// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTGBossGameplayWidget.generated.h"

class UProgressBar;

UCLASS()
class CONSPIRACYTHEORYGAME_API UCTGBossGameplayWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetPlayerHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetBossHealthPercent() const;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> PlayerHealthProgressBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> BossHealthProgressBar;
};
