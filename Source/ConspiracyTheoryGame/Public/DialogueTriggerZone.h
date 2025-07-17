// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "DialogueTriggerZone.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API ADialogueTriggerZone : public AActor
{
    GENERATED_BODY()

public:
    ADialogueTriggerZone();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerZone;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> DialogueWidgetClass;

    UPROPERTY(EditAnywhere, Category = "Dialogue")
    FText DialogueText;

    UPROPERTY(EditAnywhere, Category = "UI")
    float WidgetDisplayTime = 5.f;

    UUserWidget* ActiveWidget;

    FTimerHandle HideWidgetTimerHandle;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    void HideWidget();
};