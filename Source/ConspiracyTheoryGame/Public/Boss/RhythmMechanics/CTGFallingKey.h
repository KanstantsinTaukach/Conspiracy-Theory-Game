// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTGCoreTypes.h"
#include "CTGFallingKey.generated.h"

class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFallingKeyDestroyedSignature, AActor*, DestroyedActor);

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGFallingKey : public AActor
{
    GENERATED_BODY()

public:
    ACTGFallingKey();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintAssignable)
    FOnFallingKeyDestroyedSignature OnFallingKeyDestroyed;

    void SetModel(const FSettings& InSettings, uint32 InCellSize);

    void SetKeyType(ECTGKeyType Key) { KeyType = Key; };
    ECTGKeyType GetKeyType() const { return KeyType; };

    FPosition GetCurrentPosition() const { return Settings.ActorPosition; };

    UFUNCTION()
    void OnMissed();

    UFUNCTION()
    void DestroyFallingKey();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AActor> FallingKeyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeySettings")
    ECTGKeyType KeyType;

    UPROPERTY()
    AActor* KeyMeshActor;

private:
    FSettings Settings;
    uint32 CellSize;
    float Delta = 0.05f;
    float TimeSinceLastMove = 0.0f;

    FVector ActorPositionToVector(FPosition& InPosition, uint32 InCellSize, FDim& InDim);

    void UpdateActorPosition();
};
