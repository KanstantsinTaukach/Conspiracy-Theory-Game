// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CTGCoreTypes.h"
#include "CTGGameModeBase.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ACTGGameModeBase();

    FOnMatchStateChangeSignature OnMatchStateChanged;

    virtual void StartPlay() override;

    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

protected:
    ECTGMatchState MatchState = ECTGMatchState::WaitingToStart;
    ECTGMatchState PreviousMatchState = ECTGMatchState::WaitingToStart;

    void SetMatchState(ECTGMatchState State);

    void SetPreviousMatchState(ECTGMatchState State) { PreviousMatchState = State; };
    ECTGMatchState GetPreviousMatchState() const { return PreviousMatchState; };
};