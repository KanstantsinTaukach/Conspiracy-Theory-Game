// Team Development of a Conspiracy Theory Game for GameBOX.

#include "UI/CTGWorldUserWidget.h"
#include "UI/CTGInteractionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Pickups/CTGBasePickup.h"

void UCTGWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!IsValid(AttachedActor))
    {
        RemoveFromParent();
        return;
    }

    FVector2D ScreenPosition;
    if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
    {
        float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

        ScreenPosition /= Scale;

        if (ParentSizeBox)
        {
            ParentSizeBox->SetRenderTranslation(ScreenPosition);
        }

        UpdateInteractionInfo();
    }
}

void UCTGWorldUserWidget::UpdateInteractionInfo()
{
    if (!GetWorld() || !InteractionInfoBox) return;

    if (!InteractionInfoWidget && InteractionInfoWidgetClass)
    {
        InteractionInfoWidget = CreateWidget<UCTGInteractionWidget>(GetWorld(), InteractionInfoWidgetClass);
        if (!InteractionInfoWidget) return;
    }

    AActor* TargetActor = Cast<AActor>(AttachedActor);
    if (!IsValid(TargetActor) || !TargetActor->Implements<UCTGGameplayInterface>())
    {
        InteractionInfoBox->ClearChildren();
        return;
    }

    const FText Name = ICTGGameplayInterface::Execute_GetInteractName(TargetActor);
    const FText Description = ICTGGameplayInterface::Execute_GetInteractDescription(TargetActor);
    const int32 Points = ICTGGameplayInterface::Execute_GetPointsValue(TargetActor);
    const FText PointsText = FText::FromString(FString::FromInt(Points) + TEXT(" points"));

    InteractionInfoWidget->SetNameText(Name);
    InteractionInfoWidget->SetDescriptionText(Description);
    InteractionInfoWidget->SetPointsText(PointsText);

    InteractionInfoBox->AddChild(InteractionInfoWidget);
}
