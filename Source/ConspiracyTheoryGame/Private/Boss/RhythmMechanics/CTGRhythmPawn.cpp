// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGRhythmPawn.h"
#include "Camera/CameraComponent.h"

constexpr double GridMargin = 0.0;

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

void ACTGRhythmPawn::UpdateLocation(const FDim& InDim, uint32 InCellSize, const FTransform& InGridOrigin)
{
    Dim = InDim;
    CellSize = InCellSize;
    GridOrigin = InGridOrigin;

    check(GEngine);
    check(GEngine->GameViewport);
    check(GEngine->GameViewport->Viewport);

    auto* Viewport = GEngine->GameViewport->Viewport;
    Viewport->ViewportResizedEvent.Remove(ResizeHandle);
    ResizeHandle = Viewport->ViewportResizedEvent.AddUObject(this, &ACTGRhythmPawn::OnViewportResized);

//#if WITH_EDITOR
    OnViewportResized(Viewport, 0);
//#endif
}

double ACTGRhythmPawn::HalfFOVTan(double FOVDegrees)
{
    return FMath::Tan(FMath::DegreesToRadians(FOVDegrees * 0.5));
}

double ACTGRhythmPawn::VerticalFOV(double HorizontalFOVDegrees, double ViewportAspectHW)
{
    return FMath::RadiansToDegrees(2.0 * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(HorizontalFOVDegrees) * 0.5) * ViewportAspectHW));
}

void ACTGRhythmPawn::OnViewportResized(FViewport* Viewport, uint32 Val)
{
    if (!Viewport || Viewport->GetSizeXY().Y == 0 || Dim.Height == 0) return;

    const double WorldWidth = Dim.Width * CellSize;
    const double WorldHeight = Dim.Height * CellSize;

    const double ViewportAspect = static_cast<double>(Viewport->GetSizeXY().X) / Viewport->GetSizeXY().Y;
    const double GridAspect = static_cast<double>(Dim.Width) / Dim.Height;

    double LocationZ = 0.0;
    if (ViewportAspect <= GridAspect)
    {
        const double MarginWidth = (Dim.Width + GridMargin) * CellSize;
        LocationZ = MarginWidth / HalfFOVTan(Camera->FieldOfView);
    }
    else
    {
        check(ViewportAspect);
        const double VFOV = VerticalFOV(Camera->FieldOfView, 1.0 / ViewportAspect);
        const double MarginHeight = (Dim.Height + GridMargin) * CellSize;
        LocationZ = MarginHeight / HalfFOVTan(VFOV);
    }

    const FVector NewPawnLocation = GridOrigin.GetLocation() + 0.5 * FVector(WorldHeight, WorldWidth, LocationZ);
    SetActorLocation(NewPawnLocation);
}

AActor* ACTGRhythmPawn::SpawnVisualCharacter(TSubclassOf<AActor> ActorClass, int32 ActorOffset, const FRotator& TargetRotation)
{
    if (!GetWorld()) return nullptr;

    double GridWidth = Dim.Width * CellSize;

    const auto SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator);
    PositionCharacter(SpawnedActor, ActorOffset, TargetRotation);

    return SpawnedActor;
}

void ACTGRhythmPawn::PositionCharacter(AActor* TargetActor, int32 YOffset, const FRotator& TargetRotation)
{
    if (!TargetActor) return;

    const FVector GridLocation = GridOrigin.GetLocation();
    double WorldHeight = Dim.Height * CellSize;
    double WorldWidth = Dim.Width * CellSize;

    TargetActor->SetActorLocation(FVector(WorldHeight * 0.5, WorldWidth * 0.5 + YOffset, GridLocation.Z * 0.5));
    TargetActor->SetActorRotation(TargetRotation);
}
