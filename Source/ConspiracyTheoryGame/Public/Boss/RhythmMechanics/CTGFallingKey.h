// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTGCoreTypes.h"
#include "CTGFallingKey.generated.h"

class UStaticMeshComponent;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGFallingKey : public AActor
{
    GENERATED_BODY()

public:
    ACTGFallingKey();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void SetModel(const FSettings& InSettings, uint32 InCellSize);

    void SetKeyType(ECTGKeyType Key);

    UFUNCTION()
    void CheckHit();

    UFUNCTION()
    void OnMissed();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AActor> FallingKeyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeySettings")
    ECTGKeyType KeyType;

private:
    FSettings Settings;
    uint32 CellSize;
    float Delta = 0.1f;
    float TimeSinceLastMove = 0.0f;

    UPROPERTY()
    AActor* KeyMeshActor;

    FVector ActorPositionToVector(FPosition& InPosition, uint32 InCellSize, FDim& InDim);

    void UpdateActorPosition();
};
