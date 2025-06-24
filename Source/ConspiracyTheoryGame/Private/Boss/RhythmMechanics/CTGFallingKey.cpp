// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/RhythmMechanics/CTGFallingKey.h"
#include "Components/StaticMeshComponent.h"

ACTGFallingKey::ACTGFallingKey()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
    RootComponent = MeshComponent;    
}

void ACTGFallingKey::BeginPlay() 
{
    Super::BeginPlay();

    EndKeyLocation = GetActorLocation() - (0.0f, 0.0f, 1000.0f);
}

void ACTGFallingKey::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation();
    NewLocation.Z -= FallSpeed * DeltaTime;
    SetActorLocation(NewLocation);

    if (NewLocation.Z < EndKeyLocation.Z)
    {
        OnMissed();
        Destroy();
    }
}

void ACTGFallingKey::SetKeyType(ECTGKeyType Key) 
{
    KeyType = Key;
}

void ACTGFallingKey::CheckHit() {}

void ACTGFallingKey::OnMissed() {}