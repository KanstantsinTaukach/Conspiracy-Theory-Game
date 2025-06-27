// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGRhythmPawn.h"
#include "Camera/CameraComponent.h"

ACTGRhythmPawn::ACTGRhythmPawn()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    check(Camera);
    Camera->SetupAttachment(Origin);
}

void ACTGRhythmPawn::UpdateLocation(const FDim& InDim, int32 InCellSize, const FTransform& InGridOrigin)
{
    const float WorldWidth = InDim.Width * InCellSize;
    const float WorldHeight = InDim.Height * InCellSize;

    const float HalfAngleRad = FMath::DegreesToRadians(Camera->FieldOfView * 0.5f);

    const float LocationZ = WorldWidth * 0.5f / FMath::Tan(HalfAngleRad);
    const FVector NewPawnLocation = InGridOrigin.GetLocation() + FVector(0.5f * WorldHeight, 0.5f * WorldWidth, LocationZ);
    SetActorLocation(NewPawnLocation);
}