// Fill out your copyright notice in the Description page of Project Settings.


#include "D_Rot.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"


// Sets default values
AD_Rot::AD_Rot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AD_Rot::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AD_Rot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}






