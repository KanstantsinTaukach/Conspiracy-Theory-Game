// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CTGCoreTypes.h"
#include "CTGRhythmGameModeBase.generated.h"

class ACTGGrid;
class ACTGFallingKey;
class AExponentialHeightFog;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGRhythmGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ACTGRhythmGameModeBase();

    virtual void StartPlay() override;

    UFUNCTION(BlueprintCallable)
    int32 GetPlayerHealth() const { return PlayerHealth; };
    UFUNCTION(BlueprintCallable)
    void RemovePlayerHealth(int32 Delta);

    UFUNCTION(BlueprintCallable)
    int32 GetBossHealth() const { return BossHealth; };
    UFUNCTION(BlueprintCallable)
    void RemoveBossHealth(int32 Delta);

    UFUNCTION()
    void CheckPlayerInput(ECTGKeyType InputKey);

protected:
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", clampMax = "100"), Category = "RhythmGameSettings")
    FUintPoint GridDims{15, 30};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", clampMax = "100"), Category = "RhythmGameSettings")
    uint32 CellSize{10};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.01", clampMax = "5"), Category = "RhythmGameSettings")
    float GameSpeed{0.15f};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.5", clampMax = "5"), Category = "RhythmGameSettings")
    float SpawnInterval = 2.5f;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ACTGGrid> GridVisualClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ACTGFallingKey> FallingKeyVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    TObjectPtr<UDataTable> ColorsTable;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 PlayerHealth = 2500;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 BossHealth = 2500;

    UPROPERTY()
    TArray<ACTGFallingKey*> ActiveFallingKeys;

    UFUNCTION(BlueprintCallable)
    void SpawnFallingKey(ECTGKeyType Key);

private:
    UPROPERTY()
    TObjectPtr<ACTGGrid> GridVisual;

    UPROPERTY()
    TObjectPtr<ACTGFallingKey> FallingKeyVisual;

    UPROPERTY()
    TObjectPtr<AExponentialHeightFog> Fog;

    FSettings RhythmSettings;

    uint32 ColorTableIndex{0};

    uint32 LastActorPositionX{0};

    FTimerHandle SpawnTimerHandle;

    void FindFog();

    void UpdateColors();

    void SpawnRandomFallingKey();

    UFUNCTION()
    void RemoveFallingKey(AActor* DestroyedActor);
};
