// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Player/CTGKsilanCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ACTGKsilanCharacter::ACTGKsilanCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACTGKsilanCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!OwnerActor)
    {
        return;
    }

    FVector TargetLocation = OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * FollowDistance + FVector(0.0f, 0.0f, FollowHeight);
    FVector CurrentLocation = GetActorLocation();

    float DistanceToTarget = FVector::Distance(CurrentLocation, TargetLocation);

    FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 100.0f);

    GetCharacterMovement()->MoveSmooth(NewLocation - CurrentLocation, DeltaTime);

    FRotator TargetRotation = OwnerActor->GetActorRotation() - FRotator(0.0f, 90.0f, 0.0f);
    SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 20.0f));
}
