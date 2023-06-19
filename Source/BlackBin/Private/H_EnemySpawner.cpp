// Fill out your copyright notice in the Description page of Project Settings.


#include "H_EnemySpawner.h"
#include "H_EnemyCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "H_EnemySpawnLocation.h"
#include <Components/BoxComponent.h>
#include "DebugMessages.h"


// Sets default values
AH_EnemySpawner::AH_EnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    RootComponent = BoxComp;

}

// Called when the game starts or when spawned
void AH_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
    Target = Cast<AH_EnemySpawnLocation>(UGameplayStatics::GetActorOfClass(GetWorld(), AH_EnemySpawnLocation::StaticClass()));
    if (!Target)
    {
        printf("Target is invalid.");
        return;
    }

    // 콜리젼 활성화
    SetActorEnableCollision(true);

    // 오버랩 이벤트 등록
    OnActorBeginOverlap.AddDynamic(this, &AH_EnemySpawner::OnOverlapBegin);
}

    


// Called every frame
void AH_EnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  
}

void AH_EnemySpawner::SpawnEnemy()
{
    print("Spawn");
    UWorld* World = GetWorld();
    if (World && EnemyCharacterClass)
    {
        FVector SpawnLocation = Target->GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();
       
        AH_EnemyCharacter* Enemy = World->SpawnActor<AH_EnemyCharacter>(EnemyCharacterClass, SpawnLocation, SpawnRotation);
        if (Enemy)
        {
            Enemy->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}


void AH_EnemySpawner::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    printf("Col");
    SpawnEnemy();
   
}