// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CTGGameplayInterface.h"
#include "CTGBasePickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGBasePickup : public AActor, public ICTGGameplayInterface
{
    GENERATED_BODY()

public:
    ACTGBasePickup();

    void Interact_Implementation(APawn* InstigatorPawn);
    FText GetInteractName_Implementation() const;
    FText GetInteractDescription_Implementation() const;
    int32 GetPointsValue_Implementation() const;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, Category = "Description")
    FText DisplayName;

    UPROPERTY(EditAnywhere, Category = "Description")
    FText Description;

    virtual void BeginPlay() override;

    void ShowPickup();

    void HidePickup();

    void SetPickupState(bool bIsActive);
};
