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

    void UpdateLocation(const FDim& InDim, uint32 InCellSize, const FTransform& InGridOrigin);

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> Origin;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> Camera;

private:
    FDim Dim;
    uint32 CellSize;
    FTransform GridOrigin;
    FDelegateHandle ResizeHandle;

    double HalfFOVTan(double FOVDegrees);
    double VerticalFOV(double HorizontalFOVDegrees, double ViewportAspectHW);

    void OnViewportResized(FViewport* Viewport, uint32 Val);
};
