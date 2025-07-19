// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CTGGameInstance.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API UCTGGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    FName GetIntroLevelName() const { return IntroLevelName; };

    UFUNCTION(BlueprintCallable)
    FName GetStartupLevelName() const { return StartupLevelName; };

    UFUNCTION(BlueprintCallable)
    FName GetMenuLevelName() const { return MenuLevelName; };

    UFUNCTION(BlueprintCallable)
    FName GetBossIntroLevelName() const { return BossIntroLevelName; };

    UFUNCTION(BlueprintCallable)
    FName GetBossLevelName() const { return BossLevelName; };

    UFUNCTION(BlueprintCallable)
    FName GetGoodEndLevelName() const { return GoodEndLevelName; };

    int32 GetPlayerScore() const { return PlayerScore; };
    void SetPlayerScore(int32 NewScore) { PlayerScore = NewScore; };

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName IntroLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName StartupLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MenuLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName BossIntroLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName BossLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName GoodEndLevelName = NAME_None;

private:
    int32 PlayerScore = 0;
};