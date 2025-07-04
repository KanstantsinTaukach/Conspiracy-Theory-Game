// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CTGVisualCharacter.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGVisualCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACTGVisualCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
