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

    void SetKeyType(ECTGKeyType Key);

    UFUNCTION()
    void CheckHit();

    UFUNCTION()
    void OnMissed();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KeySettings")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeySettings")
    ECTGKeyType KeyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeySettings")
    float FallSpeed = 100.0f;

private:
    FVector EndKeyLocation;
};
