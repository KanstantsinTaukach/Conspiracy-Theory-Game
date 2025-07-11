// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CTGGameplayInterface.h"
#include "CTGLockedBossDoor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGLockedBossDoor : public AActor, public ICTGGameplayInterface
{
    GENERATED_BODY()

public:
    ACTGLockedBossDoor();

    void Interact_Implementation(APawn* InstigatorPawn);
    FText GetInteractName_Implementation() const;
    FText GetInteractDescription_Implementation() const;
    int32 GetPointsValue_Implementation() const;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, Category = "Door")
    FVector OpenLocationOffset = FVector(0.0f, 0.0f, 270.0f);

    UPROPERTY(EditAnywhere, Category = "Door")
    float TimeBetweenOffsets = 0.01f;

    UPROPERTY(EditAnywhere, Category = "Description")
    FText DisplayName;

    UPROPERTY(EditAnywhere, Category = "Description")
    FText Description;

    UPROPERTY(EditAnywhere, Category = "Description")
    int32 PointsToOpenDoor = 2500;

    virtual void BeginPlay() override;

private:
    FVector InitialLocation;

    FTimerHandle DoorOffsetTimerHande;

    FVector CloseLocationOffset = FVector(0.0f, 0.0f, 0.0f);

    void OpenDoor();
};
