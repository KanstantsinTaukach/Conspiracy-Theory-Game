// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CTGCoreTypes.h"
#include "CTGRhythmGameModeBase.generated.h"

class ACTGGrid;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGRhythmGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", clampMax = "100"))
    FUintPoint GridDims{10, 10};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", clampMax = "100"))
    uint32 CellSize{10};

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ACTGGrid> GridVisualClass;

    ACTGRhythmGameModeBase();

    virtual void StartPlay() override;

    UFUNCTION(BlueprintCallable)
    void SpawnFallingKey(ECTGKeyType Key);

private:
    UPROPERTY(EditAnywhere, Category = "KeySpawning")
    float SpawnInterval = 1.0f;

    UPROPERTY()
    ACTGGrid* GridVisual;

    FTimerHandle SpawnTimerHandle;

    void SpawnRandomFallingKey();
};
