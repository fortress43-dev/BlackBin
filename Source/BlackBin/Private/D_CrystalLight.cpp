// Fill out your copyright notice in the Description page of Project Settings.


#include "D_CrystalLight.h"

// Sets default values
AD_CrystalLight::AD_CrystalLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AD_CrystalLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AD_CrystalLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 태어나면 크기가 일정속도로 커진다
}

