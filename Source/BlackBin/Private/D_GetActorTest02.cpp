// Fill out your copyright notice in the Description page of Project Settings.


#include "D_GetActorTest02.h"

// Sets default values
AD_GetActorTest02::AD_GetActorTest02()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AD_GetActorTest02::BeginPlay()
{
	Super::BeginPlay();
	FVector now = GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Testpos with d = %d"), now.X);
	UE_LOG(LogTemp, Warning, TEXT("Testpos with f = %f"), now.X);
}

// Called every frame
void AD_GetActorTest02::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

