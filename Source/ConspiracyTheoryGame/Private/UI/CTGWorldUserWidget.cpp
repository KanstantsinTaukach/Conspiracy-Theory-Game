// Team Development of a Conspiracy Theory Game for GameBOX.

#include "UI/CTGWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void UCTGWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) 
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if(!IsValid(AttachedActor))
    {
        RemoveFromParent();
        return;
    }

    FVector2D ScreenPosition;
    if(UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
    {
        float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

        ScreenPosition /= Scale;

        if(ParentSizeBox)
        {
            ParentSizeBox->SetRenderTranslation(ScreenPosition);
        }
    }
}
