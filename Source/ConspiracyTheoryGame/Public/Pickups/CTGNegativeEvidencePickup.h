// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/CTGBasePickup.h"
#include "CTGNegativeEvidencePickup.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGNegativeEvidencePickup : public ACTGBasePickup
{
    GENERATED_BODY()

public:
    ACTGNegativeEvidencePickup();

    void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
    UPROPERTY(EditAnywhere, Category = "Points")
    int32 PointsAmountToRemove;
};
