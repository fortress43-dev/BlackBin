// Fill out your copyright notice in the Description page of Project Settings.


#include "C_HitBox.h"

// Sets default values
AC_HitBox::AC_HitBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_HitBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_HitBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	lifeTime = FMath::Max(lifeTime - 10.f * DeltaTime, 0);
	if (lifeTime == 0)
	{
		Destroy();
	}
}