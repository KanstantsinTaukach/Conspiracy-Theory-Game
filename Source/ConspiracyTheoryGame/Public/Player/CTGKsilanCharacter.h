// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CTGKsilanCharacter.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGKsilanCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ACTGKsilanCharacter();

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void SetOwnerActor(AActor* InActor) { OwnerActor = InActor; };

protected:
    UPROPERTY(EditAnywhere, Category = "Follow")
    float FollowDistance = 50.0f;

    UPROPERTY(EditAnywhere, Category = "Follow")
    float FollowHeight = 50.0f;

    UPROPERTY(EditAnywhere, Category = "Follow")
    float FollowSpeed = 5.0f;

    UPROPERTY(EditAnywhere, Category = "Follow")
    AActor* OwnerActor = nullptr;
};
