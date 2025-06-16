// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTGInteractionComponent.generated.h"

class UCTGWorldUserWidget;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CONSPIRACYTHEORYGAME_API UCTGInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCTGInteractionComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void PrimaryInteract();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    float TraceDistance;

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    float TraceRadius;

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    TEnumAsByte<ECollisionChannel> CollisionChannel;

    UPROPERTY()
    AActor* FocusedActor;

    UPROPERTY()
    UCTGWorldUserWidget* DefaultWidgetInstance;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UCTGWorldUserWidget> DefaultWidgetClass;

    void FindBestInteractable();
};
