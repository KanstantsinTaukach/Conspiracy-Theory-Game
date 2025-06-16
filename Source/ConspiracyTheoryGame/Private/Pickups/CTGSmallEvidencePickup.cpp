// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Pickups/CTGSmallEvidencePickup.h"
#include "Components/StaticMeshComponent.h"

ACTGSmallEvidencePickup::ACTGSmallEvidencePickup() 
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComponent->SetupAttachment(RootComponent);
}

void ACTGSmallEvidencePickup::Interact_Implementation(APawn* InstigatorPawn) 
{
    if (!InstigatorPawn) return;

    HidePickup();
}
