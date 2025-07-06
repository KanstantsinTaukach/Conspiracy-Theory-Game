// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGFallingKey.h"
#include "Components/StaticMeshComponent.h"
#include "CTGRhythmGameModeBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Boss/RhythmMechanics/CTGVisualCharacter.h"

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

void ACTGFallingKey::SetKeyType(ECTGKeyType Key)
{
    KeyType = Key;

    if (FallingKeyMesh)
    {
        FallingKeyMesh->SetRelativeRotation(GetRotationForDirection(KeyType));
    }
}

FRotator ACTGFallingKey::GetRotationForDirection(ECTGKeyType Direction) const
{
    switch (Direction)
    {
        case ECTGKeyType::ArrowUp: return FRotator(0.0f, 180.0f, 0.0f);
        case ECTGKeyType::ArrowDown: return FRotator(0.0f, 0.0f, 0.0f);
        case ECTGKeyType::ArrowLeft: return FRotator(0.0f, 90.0f, 0.0f);
        case ECTGKeyType::ArrowRight: return FRotator(0.0f, -90.0f, 0.0f);
        default: return FRotator::ZeroRotator;
    }
}

void ACTGFallingKey::UpdateColors(const FGridColors& Colors)
{
    if (auto* MeshMaterial = FallingKeyMesh->CreateAndSetMaterialInstanceDynamic(0))
    {
        MeshMaterial->SetVectorParameterValue("FallingKeyColor", Colors.FallingKeyColor);
    }

    ExplodeColor = Colors.FallingKeyColor;
}

void ACTGFallingKey::UpdateScale(uint32 InCellSize)
{
    check(FallingKeyMesh->GetStaticMesh());
    const FBox Box = FallingKeyMesh->GetStaticMesh()->GetBoundingBox();
    const FVector Size = Box.GetSize();

    check(Size.X);
    check(Size.Y);
    FallingKeyMesh->SetRelativeScale3D(FVector(InCellSize / Size.X, InCellSize / Size.Y, InCellSize / Size.Z / 2.0));
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

    FVector NewLocation = ActorPositionToVector(Settings.ActorPosition, CellSize);
    Origin->SetWorldLocation(NewLocation);

    if (Settings.ActorPosition.Y == Settings.GridDims.Height - 1)
    {
        OnMissed();
    }
}

FVector ACTGFallingKey::ActorPositionToVector(FPosition& InPosition, uint32 InCellSize)
{
    return FVector((Settings.GridDims.Height - 1 - InPosition.Y) * InCellSize, InPosition.X * InCellSize, 0.0) + FVector(CellSize * 0.5);
}

void ACTGFallingKey::OnMissed()
{
    if (!GetWorld()) return;

    if (auto* GameMode = Cast<ACTGRhythmGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        auto* VisualPlayer = GameMode->GetVisualPlayerCharacter();
        if (VisualPlayer)
        {
            float Damage;
            GetZoneDamage(Damage);
            VisualPlayer->SetHealth(VisualPlayer->GetCharacterHealth() - Damage);
        }
    }

    DestroyFallingKey();
}

void ACTGFallingKey::DestroyFallingKey()
{
    Explode();

    Destroy();
}

void ACTGFallingKey::Explode()
{
    if (UNiagaraComponent* NS = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, Origin->GetComponentLocation()))
    {
        NS->SetNiagaraVariableLinearColor("ExplodeColor", ExplodeColor);
    }
}

bool ACTGFallingKey::GetZoneDamage(float& Damage)
{
    bool IsDamageToBoss = false;
    const int32 CurrentY = Settings.ActorPosition.Y;
    const int32 GridHeigth = Settings.GridDims.Height;

    if (CurrentY == GridHeigth - 1)
    {
        Damage = FailZoneDamage;
        IsDamageToBoss = false;
    }
    else if (CurrentY == GridHeigth - 2)
    {
        Damage = PerfectZoneDamage;
        IsDamageToBoss = true;
    }
    else if (CurrentY == GridHeigth - 3 || CurrentY == GridHeigth - 4)
    {
        Damage = GreatZoneDamage;
        IsDamageToBoss = true;
    }
    else if (CurrentY == GridHeigth - 5 || CurrentY == GridHeigth - 6)
    {
        Damage = GoodZoneDamage;
        IsDamageToBoss = true;
    }
    else
    {
        Damage = MissZoneDamage;
        IsDamageToBoss = false;
    }

    OnGetZoneDamage.Broadcast(IsDamageToBoss, Damage);

    return IsDamageToBoss;
}