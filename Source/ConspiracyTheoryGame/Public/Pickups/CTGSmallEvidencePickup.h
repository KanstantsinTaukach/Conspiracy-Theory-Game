// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/CTGBasePickup.h"
#include "CTGSmallEvidencePickup.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGSmallEvidencePickup : public ACTGBasePickup
{
    GENERATED_BODY()

public:
    ACTGSmallEvidencePickup();

    void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
    UPROPERTY(EditAnywhere, Category = "Points")
    int32 PointsAmount;
};