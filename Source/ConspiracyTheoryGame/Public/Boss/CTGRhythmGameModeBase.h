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

protected:
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", clampMax = "100"), Category = "RhythmGameSettings")
    FUintPoint GridDims{15, 30};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", clampMax = "100"), Category = "RhythmGameSettings")
    uint32 CellSize{10};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.01", clampMax = "5"), Category = "RhythmGameSettings")
    float GameSpeed{0.1f};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.5", clampMax = "5"), Category = "RhythmGameSettings")
    float SpawnInterval = 2.5f;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ACTGGrid> GridVisualClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ACTGFallingKey> FallingKeyVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    UDataTable* ColorsTable;

    UFUNCTION(BlueprintCallable)
    void SpawnFallingKey(ECTGKeyType Key);

private:
    UPROPERTY()
    ACTGGrid* GridVisual;

    UPROPERTY()
    ACTGFallingKey* FallingKeyVisual;

    UPROPERTY()
    AExponentialHeightFog* Fog;

    FSettings RhythmSettings;

    uint32 ColorTableIndex{0};

    FTimerHandle SpawnTimerHandle;

    void FindFog();

    void UpdateColors();

    void SpawnRandomFallingKey();
};
