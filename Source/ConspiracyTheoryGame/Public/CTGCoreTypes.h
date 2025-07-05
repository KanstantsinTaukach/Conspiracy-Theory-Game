// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "Engine/DataTable.h"
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
    ArrowRight UMETA(DispayName = "Right")
};

USTRUCT(BlueprintType)
struct FDim
{
    GENERATED_USTRUCT_BODY()

    uint32 Width;
    uint32 Height;
};

UENUM(BlueprintType)
enum class ECTGCellType : uint8
{
    Empty = 0,
    Wall,
    FallingKey
};

USTRUCT(BlueprintType)
struct FPosition
{
    GENERATED_USTRUCT_BODY()

    uint32 X;
    uint32 Y;
};

USTRUCT(BlueprintType)
struct FSettings
{
    GENERATED_USTRUCT_BODY()

    FDim GridDims{25, 40};

    FPosition ActorPosition{0, 0};

    float GameSpeed{1.0f};
};

USTRUCT(BlueprintType)
struct FGridColors : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridBackgroundColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridWallColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridLineColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor SkyAtmosphereColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor FallingKeyColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridFailColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridPerfectColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridGreatColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridGoodColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridRiskColor;
};