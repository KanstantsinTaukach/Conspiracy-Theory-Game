// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Pickups/CTGNegativeEvidencePickup.h"
#include "Player/CTGPlayerState.h"

ACTGNegativeEvidencePickup::ACTGNegativeEvidencePickup() 
{
    PointsAmountToRemove = 20;
}

void ACTGNegativeEvidencePickup::Interact_Implementation(APawn* InstigatorPawn) 
{
    if (!InstigatorPawn) return;

    if (const auto PS = InstigatorPawn->GetPlayerState<ACTGPlayerState>())
    {
        if (PS->RemovePoints(PointsAmountToRemove))
        {
            HidePickup();
        }
    }
}

int32 ACTGNegativeEvidencePickup::GetPointsValue_Implementation() const
{
    return PointsAmountToRemove;
}
