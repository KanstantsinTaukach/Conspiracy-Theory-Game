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

    //UPROPERTY(Transient, meta = (BindWidgetAnim))
    //TObjectPtr<UWidgetAnimation> DamageAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float PercentColorThreshold = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor GoodColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor BadColor = FLinearColor::Red;

    virtual void NativeOnInitialized() override;

private:
    void OnHealthChanged(float Health, float HealthDelta);

    void UpdatePlayerHealthBar();
    void UpdateBossHealthBar();
};
