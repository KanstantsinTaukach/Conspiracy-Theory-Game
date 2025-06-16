// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Components/CTGInteractionComponent.h"
#include "Interfaces/CTGGameplayInterface.h"
#include "UI/CTGWorldUserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(CTGInreractionComponentLog, All, All);

UCTGInteractionComponent::UCTGInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    TraceDistance = 1000.0f;
    TraceRadius = 30.0f;
    CollisionChannel = ECC_WorldDynamic;
}

void UCTGInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FindBestInteractable();
}

void UCTGInteractionComponent::FindBestInteractable() 
{
    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

    const AActor* MyOwner = GetOwner();

    FVector EyeLocation;
    FRotator EyeRotation;

    MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

    FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

    TArray<FHitResult> Hits;

    FCollisionShape Shape;
    Shape.SetSphere(TraceRadius);

    bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

    FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

    FocusedActor = nullptr;

    for (const auto Hit : Hits)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            if (HitActor->Implements<UCTGGameplayInterface>())
            {
                FocusedActor = HitActor;
                break;
            }
        }
    }

    if(FocusedActor)
    {
        if(DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
        {
            DefaultWidgetInstance = CreateWidget<UCTGWorldUserWidget>(GetWorld(), DefaultWidgetClass);
        }

        if(DefaultWidgetInstance)
        {
            DefaultWidgetInstance->AttachedActor = FocusedActor;

            if(!DefaultWidgetInstance->IsInViewport())
            {
                DefaultWidgetInstance->AddToViewport();
            }
        }
    }
    else
    {
        if(DefaultWidgetInstance)
        {
            DefaultWidgetInstance->RemoveFromParent();
        }
    }
}

void UCTGInteractionComponent::PrimaryInteract()
{
    if (FocusedActor == nullptr) return;

    APawn* MyPawn = Cast<APawn>(GetOwner());
    ICTGGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
}