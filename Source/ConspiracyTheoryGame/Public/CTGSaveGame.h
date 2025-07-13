// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CTGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CONSPIRACYTHEORYGAME_API UCTGSaveGame : public USaveGame
{
	GENERATED_BODY()

    public:
    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    int32 SavedPoints = 0;
};
