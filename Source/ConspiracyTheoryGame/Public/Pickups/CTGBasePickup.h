// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CTGGameplayInterface.h"
#include "CTGBasePickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USoundCue;

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
    TObjectPtr<USphereComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
    TObjectPtr<USoundCue> PickupSound;

    UPROPERTY(EditAnywhere, Category = "Description")
    FText DisplayName;

    UPROPERTY(EditAnywhere, Category = "Description")
    FText Description;

    virtual void BeginPlay() override;

    void ShowPickup();

    void HidePickup();

    void SetPickupState(bool bIsActive);
};
