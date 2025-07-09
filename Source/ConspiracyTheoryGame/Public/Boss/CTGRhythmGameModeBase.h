// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "CTGGameModeBase.h"
#include "CTGCoreTypes.h"
#include "CTGRhythmGameModeBase.generated.h"

class ACTGGrid;
class ACTGFallingKey;
class AExponentialHeightFog;
class ACTGBossHUD;
class ACTGVisualCharacter;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGRhythmGameModeBase : public ACTGGameModeBase
{
    GENERATED_BODY()

public:
    ACTGRhythmGameModeBase();

    virtual void StartPlay() override;

    UFUNCTION()
    void CheckPlayerInput(ECTGKeyType InputKey);

    UFUNCTION(BlueprintCallable)
    ACTGVisualCharacter* GetVisualPlayerCharacter() const { return PlayerCharacter; };

    UFUNCTION(BlueprintCallable)
    ACTGVisualCharacter* GetVisualBossCharacter() const { return BossCharacter; };

    virtual bool ClearPause() override;

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

    UPROPERTY()
    TObjectPtr<ACTGBossHUD> HUD;

    UPROPERTY(EditDefaultsOnly, Category = "Characters")
    TSubclassOf<ACTGVisualCharacter> PlayerCharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "Characters")
    TSubclassOf<ACTGVisualCharacter> BossCharacterClass;

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", clampMax = "20"), Category = "Characters")
    int32 VisualCharacterOffset = 3;

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

    UPROPERTY()
    TObjectPtr<ACTGVisualCharacter> PlayerCharacter;

    UPROPERTY()
    TObjectPtr<ACTGVisualCharacter> BossCharacter;

    FSettings RhythmSettings;

    uint32 ColorTableIndex{0};

    uint32 LastActorPositionX{0};

    FTimerHandle SpawnTimerHandle;

    void FindFog();

    void UpdateColors();

    void SpawnRandomFallingKey();

    void OnPlayerCharacterDeath();
    void OnBossCharacterDeath();

    UFUNCTION()
    void RemoveFallingKey(AActor* DestroyedActor);
};
