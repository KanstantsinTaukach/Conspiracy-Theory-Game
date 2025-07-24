// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CTGSoundFuncLib.generated.h"

class USoundClass;

UCLASS()
class CONSPIRACYTHEORYGAME_API UCTGSoundFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    static void SetSoundClassVolume(USoundClass* SoundClass, float InVolume);

    UFUNCTION(BlueprintCallable)
    static float GetSoundClassVolume(USoundClass* SoundClass);
};
