// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CTGKsilanCharacter.generated.h"

class USoundCue;
class UNiagaraSystem;

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
    UPROPERTY(EditAnywhere, Category = "Location")
    float FollowDistance = -20.0f;

    UPROPERTY(EditAnywhere, Category = "Location")
    float FollowHeight = 20.0f;

    UPROPERTY(EditAnywhere, Category = "Location")
    TObjectPtr<AActor> OwnerActor = nullptr;

    UPROPERTY(EditAnywhere, Category = "Location")
    FName FollowSocketName;

    UPROPERTY(EditAnywhere, Category = "Location")
    float TeleportDistance = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "Sound")
    TObjectPtr<USoundCue> TeleportSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    TObjectPtr<UNiagaraSystem> TeleportEffect;
};
