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
    Dim = InDim;
    CellSize = InCellSize;
    GridOrigin = InGridOrigin;

    check(GEngine);
    check(GEngine->GameViewport);
    check(GEngine->GameViewport->Viewport);

    auto* Viewport = GEngine->GameViewport->Viewport;
    Viewport->ViewportResizedEvent.AddUObject(this, &ACTGRhythmPawn::OnViewportResized);

#if WITH_EDITOR
    OnViewportResized(Viewport, 0);
#endif
}

float ACTGRhythmPawn::HalfFOVTan(float FOVDegrees)
{
    return FMath::Tan(FMath::DegreesToRadians(FOVDegrees * 0.5f));
}

float ACTGRhythmPawn::VerticalFOV(float HorizontalFOVDegrees, float ViewportAspectHW)
{
    return FMath::RadiansToDegrees(2.0f * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(HorizontalFOVDegrees) * 0.5f) * ViewportAspectHW));
}

void ACTGRhythmPawn::OnViewportResized(FViewport* Viewport, uint32 Val)
{
    if (!Viewport || Viewport->GetSizeXY().Y == 0 || Dim.Height == 0) return;

    const float WorldWidth = Dim.Width * CellSize;
    const float WorldHeight = Dim.Height * CellSize;

    const float ViewportAspect = static_cast<float>(Viewport->GetSizeXY().X) / Viewport->GetSizeXY().Y;
    const float GridAspect = static_cast<float>(Dim.Width) / Dim.Height;

    float LocationZ = 0.0f;
    if (ViewportAspect <= GridAspect)
    {
        LocationZ = WorldWidth / HalfFOVTan(Camera->FieldOfView);
    }
    else
    {
        check(ViewportAspect);
        const float VFOV = VerticalFOV(Camera->FieldOfView, 1.0 / ViewportAspect);
        LocationZ = WorldHeight / HalfFOVTan(VFOV);
    }

    const FVector NewPawnLocation = GridOrigin.GetLocation() + 0.5f * FVector(WorldHeight, WorldWidth, LocationZ);
    SetActorLocation(NewPawnLocation);
}
