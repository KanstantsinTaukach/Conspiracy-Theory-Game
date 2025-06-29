#include "RoomSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "PickUpSpawner.h"
#include "Pickups/CTGBasePickup.h"
#include "Engine/StaticMeshActor.h"
#include "NavMesh/RecastNavMesh.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

// Sets default values
ARoomSpawner::ARoomSpawner()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ARoomSpawner::BeginPlay()
{
    Super::BeginPlay();
    GenerateDungeon();

}

bool ARoomSpawner::IsLocationFree(FVector Location, float Radius)
{
    for (auto Room : SpawnedRooms)
    {
        if (FVector::Dist(Room->GetActorLocation(), Location) < Radius)
        {
            return false;
        }
    }
    return true;
}



void ARoomSpawner::RebuildNavigation()
{
    if (UWorld* World = GetWorld())
    {
        if (UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World))
        {
            ARecastNavMesh* RecastNavMesh = Cast<ARecastNavMesh>(NavSystem->GetMainNavData());
            if (RecastNavMesh)
            {
                RecastNavMesh->RebuildAll();
                UE_LOG(LogTemp, Warning, TEXT("NavMesh rebuilt after dungeon generation."));
            }
            else
            {
                NavSystem->Build();
                UE_LOG(LogTemp, Warning, TEXT("NavSystem Build called after dungeon generation."));
            }
        }
    }
}

void ARoomSpawner::GenerateDungeon()
{
    if (!StartRoomClass) return;

    FActorSpawnParameters Params;
    ARoomBase* StartRoom = GetWorld()->SpawnActor<ARoomBase>(StartRoomClass, GetActorLocation(), FRotator::ZeroRotator, Params);
    if (!StartRoom) return;

    SpawnedRooms.Add(StartRoom);
    UE_LOG(LogTemp, Warning, TEXT("Start room has %d exits"), StartRoom->RoomExits.Num());
    TArray<ARoomBase*> Frontier;
    Frontier.Add(StartRoom);

    while (SpawnedRooms.Num() < TotalRooms && Frontier.Num() > 0)
    {
        ARoomBase* CurrentRoom = Frontier.Pop();
        for (FRoomExit& Exit : CurrentRoom->RoomExits)
        {
            if (Exit.bIsConnected) continue;
            FVector SpawnLocation = CurrentRoom->GetExitLocation(Exit.Direction);
            FRotator SpawnRotation = CurrentRoom->GetExitRotation(Exit.Direction);

            if (!IsLocationFree(SpawnLocation)) continue;

            TSubclassOf<ARoomBase> RoomToSpawn =
                (SpawnedRooms.Num() == TotalRooms - 1) ? BossRoomClass : RoomTypes[FMath::RandRange(0, RoomTypes.Num() - 1)];

            ARoomBase* NewRoom = GetWorld()->SpawnActor<ARoomBase>(RoomToSpawn, SpawnLocation, SpawnRotation, Params);
            if (NewRoom)
            {
                Exit.bIsConnected = true;
                SpawnedRooms.Add(NewRoom);
                Frontier.Add(NewRoom);
            }
        }
    }
    CloseUnconnectedExits();
    SpawnPickups();
    GetWorldTimerManager().SetTimerForNextTick(this, &ARoomSpawner::RebuildNavigation);
}

void ARoomSpawner::CloseUnconnectedExits()
{
    if (!ExitBlockerMesh) return;

    for (ARoomBase* Room : SpawnedRooms)
    {

        FBox RoomBounds = Room->GetComponentsBoundingBox(true);
        FVector RoomSize = RoomBounds.IsValid ? RoomBounds.GetSize() : FVector(1000, 1000, 0);
        float SphereRadius = RoomSize.GetMax() / 4.0f;

        FVector RoomCenter = Room->GetActorLocation();

        for (const FRoomExit& Exit : Room->RoomExits)
        {

            FVector ExitWorldLocation = Room->GetActorTransform().TransformPosition(Exit.Location);
            FRotator ExitWorldRotation = Room->GetActorRotation() + Exit.Rotation;


            FVector CenterToExitDir = (ExitWorldLocation - RoomCenter).GetSafeNormal();


            float DesiredOffset = RoomSize.GetMax() * 0.5f;  
            FVector BlockerPosition = ExitWorldLocation - CenterToExitDir * DesiredOffset;


            bool bShouldSpawn = true;
            for (ARoomBase* OtherRoom : SpawnedRooms)
            {
                if (OtherRoom == Room) continue;
                if (FVector::DistSquared(ExitWorldLocation, OtherRoom->GetActorLocation()) < FMath::Square(SphereRadius))
                {
                    bShouldSpawn = false;
                    break;
                }
            }




            if (bShouldSpawn)
            {

                AStaticMeshActor* Blocker =
                    GetWorld()->SpawnActor<AStaticMeshActor>(BlockerPosition, ExitWorldRotation, FActorSpawnParameters());

                if (Blocker)
                {
                    UStaticMeshComponent* MeshComp = Blocker->GetStaticMeshComponent();
                    MeshComp->SetStaticMesh(ExitBlockerMesh);
                    MeshComp->SetMobility(EComponentMobility::Movable);


                    Blocker->AttachToComponent(Room->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);



                }
            }
        }
    }
}

void ARoomSpawner::SpawnPickups()
{
    TArray<AActor*> FoundSpawners;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickUpSpawner::StaticClass(), FoundSpawners);

    int32 CurrentIndex = 0;
    int32 SpawnedGuaranteed = 0;

    for (int32 i = 0; i < FoundSpawners.Num(); ++i)
    {
        APickUpSpawner* Spawner = Cast<APickUpSpawner>(FoundSpawners[i]);
        if (!Spawner || !Spawner->PickupClass) continue;

        bool bShouldSpawn = false;

        if (SpawnedGuaranteed < GuaranteedPickupCount)
        {
            bShouldSpawn = true;
            ++SpawnedGuaranteed;
        }
        else if (FMath::FRandRange(0.f, 100.f) <= RandomPickupChance)
        {
            bShouldSpawn = true;
        }

        if (bShouldSpawn && PickupSequence.IsValidIndex(CurrentIndex))
        {
            FActorSpawnParameters Params;
            Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            GetWorld()->SpawnActor<ACTGBasePickup>(
                PickupSequence[CurrentIndex], Spawner->GetActorLocation(), Spawner->GetActorRotation(), Params);
            ++CurrentIndex;
        }
    }
}