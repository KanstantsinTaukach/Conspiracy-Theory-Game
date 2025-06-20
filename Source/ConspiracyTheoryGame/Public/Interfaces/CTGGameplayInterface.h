// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CTGGameplayInterface.generated.h"


UINTERFACE(MinimalAPI)
class UCTGGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

class CONSPIRACYTHEORYGAME_API ICTGGameplayInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Interact(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    FText GetInteractName() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    FText GetInteractDescription() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    int32 GetPointsValue() const;
};
