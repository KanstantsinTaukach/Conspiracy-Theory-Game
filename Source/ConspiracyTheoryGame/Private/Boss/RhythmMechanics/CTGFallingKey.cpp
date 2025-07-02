// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGFallingKey.h"
#include "Components/StaticMeshComponent.h"
#include "CTGRhythmGameModeBase.h"

ACTGFallingKey::ACTGFallingKey()
{
    PrimaryActorTick.bCanEverTick = true;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    FallingKeyMesh = CreateDefaultSubobject<UStaticMeshComponent>("FallingKeyMesh");
    check(FallingKeyMesh);
    FallingKeyMesh->SetupAttachment(Origin);
}

void ACTGFallingKey::SetModel(const FSettings& InSettings, uint32 InCellSize)
{
    Settings = InSettings;
    Delta = Settings.GameSpeed / 10;
    Settings.GameSpeed = FMath::RandRange(Settings.GameSpeed - Delta, Settings.GameSpeed + Delta);
    CellSize = InCellSize;
}

void ACTGFallingKey::UpdateColors(const FGridColors& Colors)
{
    if (auto* MeshMaterial = FallingKeyMesh->CreateAndSetMaterialInstanceDynamic(0))
    {
        MeshMaterial->SetVectorParameterValue("FallingKeyColor", Colors.FallingKeyColor);
    }
}

void ACTGFallingKey::UpdateScale(uint32 InCellSize) 
{
    check(FallingKeyMesh->GetStaticMesh());
    const FBox Box = FallingKeyMesh->GetStaticMesh()->GetBoundingBox();
    const FVector Size = Box.GetSize();

    check(Size.X);
    check(Size.Y);
    FallingKeyMesh->SetRelativeScale3D(FVector(InCellSize / Size.X, InCellSize / Size.Y, InCellSize / Size.Z));
}

void ACTGFallingKey::ACTGFallingKey::BeginPlay()
{
    Super::BeginPlay();

    UpdateActorPosition();
}

void ACTGFallingKey::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeSinceLastMove += DeltaTime;

    if (TimeSinceLastMove >= Settings.GameSpeed)
    {
        UpdateActorPosition();
        TimeSinceLastMove = 0.0f;
    }
}

void ACTGFallingKey::UpdateActorPosition()
{
    if (!FallingKeyMesh) return;
    Settings.ActorPosition.Y = Settings.ActorPosition.Y + 1;

    FVector NewLocation = ActorPositionToVector(Settings.ActorPosition, CellSize, Settings.GridDims);
    Origin->SetWorldLocation(NewLocation);

    if (Settings.ActorPosition.Y == Settings.GridDims.Height - 1)
    {
        OnMissed();
    }
}

FVector ACTGFallingKey::ActorPositionToVector(FPosition& InPosition, uint32 InCellSize, FDim& InDim)
{
    return FVector((Settings.GridDims.Height - 1 - InPosition.Y) * InCellSize, InPosition.X * InCellSize, 0.0) + FVector(CellSize * 0.5);
}

void ACTGFallingKey::OnMissed()
{
    if (auto* GameMode = Cast<ACTGRhythmGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        GameMode->RemovePlayerHealth(200);
    }

    OnFallingKeyDestroyed.Broadcast(this);
    DestroyFallingKey();
}

void ACTGFallingKey::DestroyFallingKey()
{
    Destroy();
}