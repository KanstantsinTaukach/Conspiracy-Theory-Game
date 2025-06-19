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

    private:
    CTGMatchState MatchState = CTGMatchState::WaitingToStart;

    void SetMatchState(CTGMatchState State);
};