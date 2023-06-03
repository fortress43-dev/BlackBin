// Fill out your copyright notice in the Description page of Project Settings.


#include "D_LightBarrior.h"

// Sets default values
AD_LightBarrior::AD_LightBarrior()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AD_LightBarrior::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AD_LightBarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector scale = GetActorScale3D();
	SetActorScale3D(scale + speed*DeltaTime);

	if (scale.X > destroySize)Destroy();
}

