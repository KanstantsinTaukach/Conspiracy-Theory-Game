// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CTGCoreTypes.generated.h"

UENUM(BlueprintType)
enum class ECTGMatchState : uint8
{
	WaitingToStart = 0,
	InProgress,
	FightingWithBoss,
	Pause,
	GameOver
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangeSignature, ECTGMatchState);

UENUM(BlueprintType)
enum class ECTGKeyType : uint8
{
    ArrowUp = 0 UMETA(DispayName = "Up"),
    ArrowDown UMETA(DispayName = "Down"),
    ArrowLeft UMETA(DispayName = "Left"),
    AroowRight UMETA(DispayName = "Right")
};