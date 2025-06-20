// Team Development of a Conspiracy Theory Game for GameBOX.

#include "UI/CTGInteractionWidget.h"
#include "Components/TextBlock.h"

void UCTGInteractionWidget::SetNameText(const FText& Text) 
{
    if (NameText)
    {
        NameText->SetText(Text);
    }
}

void UCTGInteractionWidget::SetDescriptionText(const FText& Text) 
{
    if (DescriptionText)
    {
        DescriptionText->SetText(Text);
    }
}

void UCTGInteractionWidget::SetPointsText(const FText& Text) 
{
    if (PointsText)
    {
        PointsText->SetText(Text);
    }
}