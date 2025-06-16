// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Pickups/CTGBasePickup.h"
#include "Components/SphereComponent.h"

ACTGBasePickup::ACTGBasePickup()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->SetCollisionProfileName("Pickup");
    RootComponent = CollisionComponent;
}

void ACTGBasePickup::BeginPlay()
{
    Super::BeginPlay();

    check(CollisionComponent);
}

void ACTGBasePickup::ShowPickup() 
{
    SetPickupState(true);
}

void ACTGBasePickup::HidePickup() 
{
    SetPickupState(false);
}

void ACTGBasePickup::SetPickupState(bool bIsActive) 
{
    SetActorEnableCollision(bIsActive);

    RootComponent->SetVisibility(bIsActive, true);
}

void ACTGBasePickup::Interact_Implementation(APawn* InstigatorPawn) {}