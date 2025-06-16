// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Components/CTGInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "Interfaces/CTGGameplayInterface.h"

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
}

void UCTGInteractionComponent::PrimaryInteract() 
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

    for (const auto Hit : Hits)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            if (HitActor->Implements<UCTGGameplayInterface>())
            {
                APawn* MyPawn = Cast<APawn>(GetOwner());
                ICTGGameplayInterface::Execute_Interact(HitActor, MyPawn);

                DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 3.0f);

                break;
            }
        }
    }

    DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);

    UE_LOG(CTGInreractionComponentLog, Log, TEXT("Interact success"));
}