// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTGBasePickup.h"
#include "PickupSpawner.h"
#include "RoomBase.h"
#include "RoomSpawner.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API ARoomSpawner : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ARoomSpawner();
    UPROPERTY(EditAnywhere)
    TSubclassOf<ARoomBase> StartRoomClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
    float BlockerSpawnOffset = 0.5f;

    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<ARoomBase>> RoomTypes;

    UPROPERTY(EditAnywhere)
    TSubclassOf<ARoomBase> BossRoomClass;
    UPROPERTY(EditAnywhere, Category = "Dungeon Generation")
    float RoomSpacing = 1000.0f;
    UPROPERTY(EditAnywhere)
    int32 TotalRooms = 10;

    UFUNCTION(BlueprintCallable, Category = "Pickup")
    void SpawnPickups();

    UPROPERTY(EditAnywhere, Category = "Pickup")
    TArray<TSubclassOf<ACTGBasePickup>> PickupSequence;

    UPROPERTY(EditAnywhere, Category = "Pickup")
    int32 GuaranteedPickupCount = 10;

    UPROPERTY(EditAnywhere, Category = "Pickup")
    float RandomPickupChance = 10.f;

    UPROPERTY(EditAnywhere, Category = "Dungeon")
    UStaticMesh* ExitBlockerMesh;
    void CloseUnconnectedExits();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    TArray<ARoomBase*> SpawnedRooms;

    bool IsLocationFree(FVector Location, float Radius = 500.0f);

    void GenerateDungeon();
    void RebuildNavigation();
};
