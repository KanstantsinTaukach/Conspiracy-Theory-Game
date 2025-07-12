#include "RoomSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "CollisionQueryParams.h"
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

void ARoomSpawner::SpawnEnemies()
{
    if (!EnemyClass) return;
    if (SpawnedRooms.Num() <= 1) return;  

    TArray<ARoomBase*> RoomsToSpawnIn = SpawnedRooms;


    RoomsToSpawnIn.RemoveAt(0);


    RoomsToSpawnIn.Sort([](const ARoomBase& A, const ARoomBase& B) { return FMath::RandBool(); });

    int32 NumToSpawn = FMath::Min(EnemiesToSpawn, RoomsToSpawnIn.Num());

    for (int32 i = 0; i < NumToSpawn; ++i)
    {
        ARoomBase* Room = RoomsToSpawnIn[i];
        if (!Room) continue;

        FVector RoomLocation = Room->GetActorLocation();
        FVector SpawnLocation = RoomLocation + FVector(150, 0, 500);
        

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, Params);
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

TArray<TSubclassOf<ARoomBase>> ShuffledRoomTypes = RoomTypes;

    // Самодельный перемешивающий цикл
    for (int32 i = 0; i < ShuffledRoomTypes.Num(); ++i)
    {
        int32 SwapIndex = FMath::RandRange(i, ShuffledRoomTypes.Num() - 1);
        if (i != SwapIndex)
        {
            ShuffledRoomTypes.Swap(i, SwapIndex);
        }
    }

    TArray<ARoomBase*> Frontier;
    Frontier.Add(StartRoom);

    int32 RoomTypeIndex = 0;

    // Спавним все обычные комнаты
    while (SpawnedRooms.Num() < TotalRooms - 1 && Frontier.Num() > 0)
    {
        ARoomBase* CurrentRoom = Frontier.Pop();
        for (FRoomExit& Exit : CurrentRoom->RoomExits)
        {
            if (Exit.bIsConnected) continue;
            FVector SpawnLocation = CurrentRoom->GetExitLocation(Exit.Direction);
            FRotator SpawnRotation = CurrentRoom->GetExitRotation(Exit.Direction);

            if (!IsLocationFree(SpawnLocation)) continue;

            TSubclassOf<ARoomBase> RoomToSpawn;
            if (ShuffledRoomTypes.IsValidIndex(RoomTypeIndex))
            {
                RoomToSpawn = ShuffledRoomTypes[RoomTypeIndex++];
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Ran out of room types"));
                continue;
            }

            ARoomBase* NewRoom = GetWorld()->SpawnActor<ARoomBase>(RoomToSpawn, SpawnLocation, SpawnRotation, Params);
            if (NewRoom)
            {
                Exit.bIsConnected = true;
                SpawnedRooms.Add(NewRoom);
                Frontier.Add(NewRoom);

                if (SpawnedRooms.Num() >= TotalRooms - 1) break;
            }
        }
    }


    bool bBossSpawned = false;

    if (SpawnedRooms.Num() > 0)
    {
        ARoomBase* LastRoom = SpawnedRooms.Last();

        for (FRoomExit& Exit : LastRoom->RoomExits)
        {
            if (Exit.bIsConnected) continue;

            FVector SpawnLocation = LastRoom->GetExitLocation(Exit.Direction);
            FRotator SpawnRotation = LastRoom->GetExitRotation(Exit.Direction);

            if (!IsLocationFree(SpawnLocation)) continue;

            ARoomBase* BossRoom = GetWorld()->SpawnActor<ARoomBase>(BossRoomClass, SpawnLocation, SpawnRotation, Params);
            if (BossRoom)
            {
                Exit.bIsConnected = true;
                SpawnedRooms.Add(BossRoom);
                bBossSpawned = true;
                UE_LOG(LogTemp, Warning, TEXT("Boss room spawned successfully on last room."));
                break;
            }
        }
    }

    if (!bBossSpawned)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn boss room - no valid exits on last room."));
    }

    CloseUnconnectedExits();
    SpawnPickups();
    SpawnEnemies();
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

            FVector ExitWorldLocation = Room->GetActorTransform().TransformPosition(Exit.Location) + (0, 0, 20);
            FRotator ExitWorldRotation = Room->GetActorRotation() + Exit.Rotation;


            FVector CenterToExitDir = (ExitWorldLocation - RoomCenter).GetSafeNormal();


            float DesiredOffset = RoomSize.GetMax() * 0.5f + 80.0;  
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