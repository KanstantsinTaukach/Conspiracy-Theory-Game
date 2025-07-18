// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CTGPlayerState.generated.h"

class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPointsChangedSignature, ACTGPlayerState*, PlayerState, int32, NewPoints, int32, Delta);

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGPlayerState : public APlayerState
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Points")
    int32 Points;

    UPROPERTY(EditDefaultsOnly, Category = "Points")
    int32 PointsToUnlockBoss = 2000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> DoorUnlockedSound;

public:
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPointsChangedSignature OnPointsChanged;

    UFUNCTION(BlueprintCallable, Category = "Points")
    int32 GetPoints() const { return Points; };

    UFUNCTION(BlueprintCallable, Category = "Points")
    int32 GetPointsToUnlockBoss() const { return PointsToUnlockBoss; };

    UFUNCTION(BlueprintCallable, Category = "Points")
    int32 GetPreviousPoints() const { return PreviousPoints; };

    UFUNCTION(BlueprintCallable, Category = "Points")
    void AddPoints(int32 Delta);

    UFUNCTION(BlueprintCallable, Category = "Points")
    bool RemovePoints(int32 Delta);

    virtual void BeginPlay() override;

private:
    int32 PreviousPoints;
};
