// Fill out your copyright notice in the Description page of Project Settings.


#include "H_EnemySpawnLocation.h"
#include <Components/BoxComponent.h>


// Sets default values
AH_EnemySpawnLocation::AH_EnemySpawnLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = BoxComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(BoxComp);
}

// Called when the game starts or when spawned
void AH_EnemySpawnLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AH_EnemySpawnLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

