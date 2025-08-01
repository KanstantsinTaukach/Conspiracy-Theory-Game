// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTGCoreTypes.h"
#include "CTGFallingKey.generated.h"

class UStaticMeshComponent;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGetZoneDamageSignature, bool, bIsBoss, float, Damage);

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGFallingKey : public AActor
{
    GENERATED_BODY()

public:
    ACTGFallingKey();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintAssignable)
    FOnGetZoneDamageSignature OnGetZoneDamage;

    void SetModel(const FSettings& InSettings, uint32 InCellSize);

    void SetKeyType(ECTGKeyType Key);
    UFUNCTION(BlueprintCallable)
    ECTGKeyType GetKeyType() const { return KeyType; };

    FPosition GetCurrentPosition() const { return Settings.ActorPosition; };

    void UpdateColors(const FGridColors& Colors);
    void UpdateScale(uint32 InCellSize);

    UFUNCTION()
    void OnMissed();

    UFUNCTION()
    void DestroyFallingKey(bool bIsPlayerHit);

    UFUNCTION()
    bool GetZoneDamage(float& Damage);

    void Explode(bool bIsPlayerHit);

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> Origin;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> FallingKeyMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeySettings")
    ECTGKeyType KeyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    TObjectPtr<UNiagaraSystem> ExplosionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float PerfectZoneDamage = 15.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float GreatZoneDamage = 9.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float GoodZoneDamage = 4.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float FailZoneDamage = 20.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float MissZoneDamage = 15.0f;

private:
    FSettings Settings;
    uint32 CellSize;
    float Delta = 0.0f;
    float TimeSinceLastMove = 0.0f;

    FLinearColor GoodExplodeColor;
    FLinearColor BadExplodeColor;

    FVector ActorPositionToVector(FPosition& InPosition, uint32 InCellSize);

    void UpdateActorPosition();

    FRotator GetRotationForDirection(ECTGKeyType Direction) const;

    void RotateKey();
};
