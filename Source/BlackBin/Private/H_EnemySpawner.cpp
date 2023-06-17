// Fill out your copyright notice in the Description page of Project Settings.


#include "H_EnemySpawner.h"
#include "H_EnemyCharacter.h"

// Sets default values
AH_EnemySpawner::AH_EnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AH_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AH_EnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AH_EnemySpawner::SpawnEnemy()
{
    UWorld* World = GetWorld();
    if (World && EnemyCharacterClass)
    {
        FVector SpawnLocation = GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();

        AH_EnemyCharacter* Enemy = World->SpawnActor<AH_EnemyCharacter>(EnemyCharacterClass, SpawnLocation, SpawnRotation);
        if (Enemy)
        {
            Enemy->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}