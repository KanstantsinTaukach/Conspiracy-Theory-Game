// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTGWorldUserWidget.generated.h"

class USizeBox;
class UVerticalBox;
class UCTGInteractionWidget;

UCLASS()
class CONSPIRACYTHEORYGAME_API UCTGWorldUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    AActor* AttachedActor;

protected:
    UPROPERTY(meta = (Bindwidget))
    TObjectPtr <USizeBox> ParentSizeBox;

    UPROPERTY(meta = (Bindwidget))
    TObjectPtr <UVerticalBox> InteractionInfoBox;

    UPROPERTY(EditAnywhere, Category = "UI")
    FVector WorldOffset;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> InteractionInfoWidgetClass;

    UPROPERTY()
    TObjectPtr <UCTGInteractionWidget> InteractionInfoWidget;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    void UpdateInteractionInfo();
};
