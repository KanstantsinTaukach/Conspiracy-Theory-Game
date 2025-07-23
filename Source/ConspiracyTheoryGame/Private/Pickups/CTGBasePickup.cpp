// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Pickups/CTGBasePickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/Gameplaystatics.h"
#include "Sound/SoundCue.h"

ACTGBasePickup::ACTGBasePickup()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->SetCollisionProfileName("Pickup");
    RootComponent = CollisionComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComponent->SetupAttachment(RootComponent);
}

void ACTGBasePickup::BeginPlay()
{
    Super::BeginPlay();

    check(CollisionComponent);
    check(MeshComponent);
}

void ACTGBasePickup::ShowPickup()
{
    SetPickupState(true);
}

void ACTGBasePickup::HidePickup()
{
    if (PickupSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
    }

    SetPickupState(false);
}

void ACTGBasePickup::SetPickupState(bool bIsActive)
{
    SetActorEnableCollision(bIsActive);

    RootComponent->SetVisibility(bIsActive, true);
}

void ACTGBasePickup::Interact_Implementation(APawn* InstigatorPawn) {}

FText ACTGBasePickup::GetInteractName_Implementation() const
{
    return DisplayName;
}

FText ACTGBasePickup::GetInteractDescription_Implementation() const
{
    return Description;
}

int32 ACTGBasePickup::GetPointsValue_Implementation() const
{
    return 0;
}
