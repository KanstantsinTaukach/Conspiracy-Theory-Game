// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "CTGGameModeBase.h"
#include "CTGCoreTypes.h"
#include "CTGRhythmGameModeBase.generated.h"

class ACTGGrid;
class ACTGFallingKey;
class ACTGBossHUD;
class ACTGVisualCharacter;
class USoundCue;
class UAudioComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrepareForNextStageSignature, float, SecondsToPrepare);

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGRhythmGameModeBase : public ACTGGameModeBase
{
    GENERATED_BODY()

public:
    ACTGRhythmGameModeBase();

    virtual void StartPlay() override;

    UPROPERTY(BlueprintAssignable)
    FOnPrepareForNextStageSignature OnPrepareForNextStage;

    UFUNCTION()
    void CheckPlayerInput(ECTGKeyType InputKey);

    UFUNCTION(BlueprintCallable)
    ACTGVisualCharacter* GetVisualPlayerCharacter() const { return PlayerCharacter; };

    UFUNCTION(BlueprintCallable)
    ACTGVisualCharacter* GetVisualBossCharacter() const { return BossCharacter; };

    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    void StartBattleWithBoss();

protected:
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "5", cCampMax = "100"), Category = "RhythmGameSettings")
    FUintPoint GridDims{15, 30};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"), Category = "RhythmGameSettings")
    uint32 CellSize{10};

    UPROPERTY(VisibleAnywhere, Category = "RhythmGameSettings")
    float GameSpeed{0.5f};

    UPROPERTY(VisibleAnywhere, Category = "RhythmGameSettings")
    float SpawnInterval = 2.0;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> StartGameSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> FirstDialogSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> SecondDialogSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundCue> ThirdDialogSound;

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
    TObjectPtr<ACTGVisualCharacter> PlayerCharacter;

    UPROPERTY()
    TObjectPtr<ACTGVisualCharacter> BossCharacter;

    UPROPERTY()
    TObjectPtr<UAudioComponent> GameMusicComponent;

    UPROPERTY()
    TObjectPtr<USoundCue> CurrentDialogSound;

    FSettings RhythmSettings;

    uint32 ColorTableIndex{0};

    uint32 LastActorPositionX{0};

    FTimerHandle SpawnTimerHandle;
    FTimerHandle PrepareTimerHanlde;

    bool IsMiddleStage = false;
    bool IsFinalStage = false;

    void UpdateColors();

    void SpawnRandomFallingKey();

    void OnPlayerCharacterDeath();
    void OnBossCharacterDeath();

    UFUNCTION()
    void RemoveFallingKey(AActor* DestroyedActor);

    void DestroyAllFallingKeys(bool bIsPlayerWin);

    void GetBattleStageLevel(float Health, float HealthDelta);

    void PrepareForTheNextStage();
    void StartNewStage();
};
