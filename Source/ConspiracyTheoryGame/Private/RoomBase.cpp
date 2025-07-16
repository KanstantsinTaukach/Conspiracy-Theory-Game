#include "RoomBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BrushComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"


ARoomBase::ARoomBase()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent->SetMobility(EComponentMobility::Movable);

    RoomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomMesh"));
    RoomMesh->SetupAttachment(RootComponent);


    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (MeshAsset.Succeeded())
    {
        RoomMesh->SetStaticMesh(MeshAsset.Object);
        RoomMesh->SetRelativeScale3D(FVector(30.f, 30.f, 0.2f));  
    }
}

void ARoomBase::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    InitializeRoomExits(ExitOffset);
}

void ARoomBase::BeginPlay()
{
    Super::BeginPlay();

    SpawnPatrolTargetPoint();

    RoomMesh->SetCanEverAffectNavigation(true);



    for (const auto& Exit : RoomExits)
    {
        FVector WorldPos = GetActorTransform().TransformPosition(Exit.Location);
        FVector ArrowEnd = WorldPos + Exit.Rotation.Vector() * 100.f;


    }
}

void ARoomBase::InitializeRoomExits(float Offset)
{
    RoomExits.Empty();

    if (bUseNorthExit)
    {
        RoomExits.Add({ERoomDirection::North, FVector(0.f, Offset, 0.f), FRotator(0.f, 0.f, 0.f)});
    }
    if (bUseSouthExit)
    {
        RoomExits.Add({ERoomDirection::South, FVector(0.f, -Offset, 0.f), FRotator(0.f, 180.f, 0.f)});
    }
    if (bUseEastExit)
    {
        RoomExits.Add({ERoomDirection::East, FVector(Offset, 0.f, 0.f), FRotator(0.f, 90.f, 0.f)});
    }
    if (bUseWestExit)
    {
        RoomExits.Add({ERoomDirection::West, FVector(-Offset, 0.f, 0.f), FRotator(0.f, -90.f, 0.f)});
    }
}

FVector ARoomBase::GetExitLocation(ERoomDirection Direction) const
{
    for (const auto& Exit : RoomExits)
    {
        if (Exit.Direction == Direction)
        {
            return GetActorTransform().TransformPosition(Exit.Location);
        }
    }
    return FVector::ZeroVector;
}

FRotator ARoomBase::GetExitRotation(ERoomDirection Direction) const
{
    for (const auto& Exit : RoomExits)
    {
        if (Exit.Direction == Direction)
        {
            return GetActorRotation() + Exit.Rotation;
        }
    }
    return FRotator::ZeroRotator;
}

FTransform ARoomBase::GetWorldExitTransform(const FRoomExit& Exit) const
{
    FVector WorldLocation = GetActorLocation() + GetActorRotation().RotateVector(Exit.Location);
    FRotator WorldRotation = GetActorRotation() + Exit.Rotation;
    return FTransform(WorldRotation, WorldLocation);
}

void ARoomBase::SpawnPatrolTargetPoint()
{
    if (!GetWorld()) return;

    if (!bSpawnPatrolPoint)
    {
        UE_LOG(LogTemp, Warning, TEXT("Спавн точки патруля отключен переменной bSpawnPatrolPoint."));
        return;
    }

    if (!bIsBossRoom && !bIsStartRoom)
    {
        FVector CenterLocation = GetActorLocation() + FVector(0, 0, 100.f);

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        ATargetPoint* PatrolPoint = GetWorld()->SpawnActor<ATargetPoint>(CenterLocation, FRotator::ZeroRotator, SpawnParams);
        if (PatrolPoint)
        {
            PatrolPoint->Tags.Add(FName("patrol"));
            PatrolPoint->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
            UE_LOG(LogTemp, Warning, TEXT("Spawned patrol target point at %s"), *CenterLocation.ToString());
        }
    }
}

FVector ARoomBase::GetLocalExitLocation(ERoomDirection Direction) const
{
    for (const auto& Exit : RoomExits)
    {
        if (Exit.Direction == Direction)
        {
            return Exit.Location;
        }
    }
    return FVector::ZeroVector;
}