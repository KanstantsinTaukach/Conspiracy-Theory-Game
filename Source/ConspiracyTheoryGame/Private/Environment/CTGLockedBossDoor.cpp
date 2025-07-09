// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Environment/CTGLockedBossDoor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/CTGPlayerState.h"

ACTGLockedBossDoor::ACTGLockedBossDoor()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->SetCollisionProfileName("Pickup");
    RootComponent = CollisionComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComponent->SetupAttachment(RootComponent);
}

void ACTGLockedBossDoor::BeginPlay()
{
    Super::BeginPlay();

    check(CollisionComponent);
    check(MeshComponent);

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (const auto PS = Cast<ACTGPlayerState>(PC->PlayerState))
        {
            PointsToOpenDoor = PS->GetPointsToUnlockBoss();
        }
    }
}

void ACTGLockedBossDoor::Interact_Implementation(APawn* InstigatorPawn)
{
    if (!InstigatorPawn) return;

    if (const auto PS = InstigatorPawn->GetPlayerState<ACTGPlayerState>())
    {
        if (PS->GetPoints() >= PS->GetPointsToUnlockBoss())
        {
            FVector NewLocation = InitialLocation + OpenLocationOffset;
            MeshComponent->SetWorldLocation(NewLocation);

            SetActorEnableCollision(false);
            RootComponent->SetVisibility(false, true);
        }
    }
}

FText ACTGLockedBossDoor::GetInteractName_Implementation() const
{
    return DisplayName;
}

FText ACTGLockedBossDoor::GetInteractDescription_Implementation() const
{
    return Description;
}

int32 ACTGLockedBossDoor::GetPointsValue_Implementation() const
{
    return PointsToOpenDoor;
}