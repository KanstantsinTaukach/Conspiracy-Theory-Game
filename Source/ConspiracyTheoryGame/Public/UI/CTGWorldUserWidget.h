// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTGWorldUserWidget.generated.h"

class USizeBox;

UCLASS()
class CONSPIRACYTHEORYGAME_API UCTGWorldUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    AActor* AttachedActor;

protected:
    UPROPERTY(meta = (Bindwidget))
    USizeBox* ParentSizeBox;

    UPROPERTY(EditAnywhere, Category = "UI")
    FVector WorldOffset;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
