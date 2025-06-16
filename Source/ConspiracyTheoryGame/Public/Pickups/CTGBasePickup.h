// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CTGGameplayInterface.h"
#include "CTGBasePickup.generated.h"

class USphereComponent;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGBasePickup : public AActor, public ICTGGameplayInterface
{
    GENERATED_BODY()

public:
    ACTGBasePickup();

    void Interact_Implementation(APawn* InstigatorPawn);

protected:
    UPROPERTY(visibleAnywhere, Category = "Components")
    USphereComponent* CollisionComponent;

    virtual void BeginPlay() override;

    void ShowPickup();

    void HidePickup();

    void SetPickupState(bool bIsActive);
};
