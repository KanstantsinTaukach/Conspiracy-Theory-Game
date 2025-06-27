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

USTRUCT(BlueprintType)
struct FDim
{
    GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GridSettings")
    int32 Width;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GridSettings")
    int32 Height;
};

UENUM(BlueprintType)
enum class ECTGCellType : uint8
{
    Empty = 0,
    Wall,
    FallingKey
};

USTRUCT(BlueprintType)
struct FSettings
{
    GENERATED_USTRUCT_BODY()

    FDim GridSize;
};