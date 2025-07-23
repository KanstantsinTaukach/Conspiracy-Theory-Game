// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTGInteractionWidget.generated.h"

class UTextBlock;

UCLASS()
class CONSPIRACYTHEORYGAME_API UCTGInteractionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetNameText(const FText& Text);
    void SetDescriptionText(const FText& Text);
    void SetPointsText(const FText& Text);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> NameText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr <UTextBlock> DescriptionText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr <UTextBlock> PointsText;
};
