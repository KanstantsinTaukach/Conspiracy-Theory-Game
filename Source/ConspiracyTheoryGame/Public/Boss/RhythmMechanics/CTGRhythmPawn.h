// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CTGCoreTypes.h"
#include "CTGRhythmPawn.generated.h"

class UCameraComponent;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGRhythmPawn : public APawn
{
    GENERATED_BODY()

public:
    ACTGRhythmPawn();

    void UpdateLocation(const FDim& InDim, int32 InCellSize, const FTransform& InGridOrigin);

protected:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* Camera;
};
