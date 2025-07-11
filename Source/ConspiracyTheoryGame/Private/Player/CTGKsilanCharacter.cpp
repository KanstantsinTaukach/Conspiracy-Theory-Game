// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Player/CTGKsilanCharacter.h"

ACTGKsilanCharacter::ACTGKsilanCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACTGKsilanCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (OwnerActor)
    {
        const FVector TargetLocation = OwnerActor->GetActorLocation() + FVector(0.0f, FollowDistance, FollowHeight);
        const FRotator TargetRotation = OwnerActor->GetActorRotation();

        SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, FollowSpeed));
        SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, FollowSpeed));
    }
}
