// Fill out your copyright notice in the Description page of Project Settings.


#include "D_Rot.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AD_Rot::AD_Rot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	//MyBoxComponent->InitBoxExtent(FVector(50, 50, 50));
	RootComponent = MyBoxComponent;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	
	rot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rot"));
	rot->SetupAttachment(RootComponent);
	if (tempMesh.Succeeded()) {
		rot->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void AD_Rot::BeginPlay()
{
	Super::BeginPlay();
	if (stActor && enActor) {
		st = stActor->GetActorLocation();
		en = enActor->GetActorLocation();
	}
	a1 = FMath::Lerp(st, en, 0.5f);
}

// Called every frame
void AD_Rot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

