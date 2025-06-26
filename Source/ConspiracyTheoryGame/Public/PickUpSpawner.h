// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "CTGBasePickup.h"
#include "GameFramework/Actor.h"
#include "PickUpSpawner.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API APickUpSpawner : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    APickUpSpawner();

	   UPROPERTY(EditAnywhere, Category = "Pickup")
    float SpawnChance = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Pickup")
    TSubclassOf<ACTGBasePickup> PickupClass;

protected:


public:	


};
