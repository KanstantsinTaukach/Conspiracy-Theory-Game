// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Pickups/CTGSmallEvidencePickup.h"
#include "Player/CTGPlayerState.h"

ACTGSmallEvidencePickup::ACTGSmallEvidencePickup() 
{
    PointsAmountToAdd = 50;
}

void ACTGSmallEvidencePickup::Interact_Implementation(APawn* InstigatorPawn) 
{
    if (!InstigatorPawn) return;

    if (const auto PS = InstigatorPawn->GetPlayerState<ACTGPlayerState>())
    {
        PS->AddPoints(PointsAmountToAdd);
        HidePickup();
    }
}

int32 ACTGSmallEvidencePickup::GetPointsValue_Implementation() const
{
    return PointsAmountToAdd;
}
