// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CTGPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPointsChangedSignature, ACTGPlayerState*, PlayerState, int32, NewPoints, int32, Delta);

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGPlayerState : public APlayerState
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Points")
    int32 Points;

public:
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPointsChangedSignature OnPointsChanged;

    UFUNCTION(BlueprintCallable, Category = "Points")
    int32 GetPoints() const { return Points; };

    UFUNCTION(BlueprintCallable, Category = "Points")
    void AddPoints(int32 Delta);

    UFUNCTION(BlueprintCallable, Category = "Points")
    bool RemovePoints(int32 Delta);
};
