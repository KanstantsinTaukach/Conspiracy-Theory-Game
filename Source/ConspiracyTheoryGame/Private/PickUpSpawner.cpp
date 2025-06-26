// Team Development of a Conspiracy Theory Game for GameBOX.


#include "PickUpSpawner.h"

// Sets default values
APickUpSpawner::APickUpSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FClassFinder<ACTGBasePickup> DefaultPickupClass(TEXT("/Script/ConspiracyTheoryGame.CTGBasePickup"));
    if (DefaultPickupClass.Succeeded())
    {
        PickupClass = DefaultPickupClass.Class;
    }
}



