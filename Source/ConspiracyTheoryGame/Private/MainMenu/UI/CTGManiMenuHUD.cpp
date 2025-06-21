// Team Development of a Conspiracy Theory Game for GameBOX.

#include "MainMenu/UI/CTGManiMenuHUD.h"
#include "Blueprint/UserWidget.h"

void ACTGManiMenuHUD::BeginPlay() 
{
    Super::BeginPlay();

    if (MenuWidgetClass)
    {
        const auto MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidget)
        {
            MenuWidget->AddToViewport();
        }
    }
}
