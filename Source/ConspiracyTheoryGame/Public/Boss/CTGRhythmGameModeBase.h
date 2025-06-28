// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CTGCoreTypes.h"
#include "CTGRhythmGameModeBase.generated.h"

class ACTGGrid;
class AExponentialHeightFog;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGRhythmGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ACTGRhythmGameModeBase();

    virtual void StartPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", clampMax = "100"))
    FUintPoint GridDims{10, 10};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", clampMax = "100"))
    uint32 CellSize{10};

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ACTGGrid> GridVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    UDataTable* ColorsTable;

    UFUNCTION(BlueprintCallable)
    void SpawnFallingKey(ECTGKeyType Key);

private:
    UPROPERTY()
    ACTGGrid* GridVisual;

    UPROPERTY()
    AExponentialHeightFog* Fog;

    uint32 ColorTableIndex{0};

    void FindFog();

    void UpdateColors();

    UPROPERTY(EditAnywhere, Category = "KeySpawning")
    float SpawnInterval = 1.0f;

    FTimerHandle SpawnTimerHandle;

    void SpawnRandomFallingKey();
};
