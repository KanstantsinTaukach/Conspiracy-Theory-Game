// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CTGCharacter.generated.h"

class UCameraComponent;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACTGCharacter();

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

	virtual void BeginPlay() override;
};
