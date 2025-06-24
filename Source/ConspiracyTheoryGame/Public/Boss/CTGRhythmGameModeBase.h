// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CTGCoreTypes.h"
#include "CTGRhythmGameModeBase.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGRhythmGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ACTGRhythmGameModeBase();

    virtual void StartPlay() override;

    UFUNCTION(BlueprintCallable)
    void SpawnFallingKey(ECTGKeyType Key);

private:
    UPROPERTY(EditAnywhere, Category = "KeySpawning")
    float SpawnInterval = 1.0f;

    FTimerHandle SpawnTimerHandle;

    void SpawnRandomFallingKey();
};
