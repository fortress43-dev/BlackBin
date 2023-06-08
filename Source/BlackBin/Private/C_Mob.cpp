// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Mob.h"

// Sets default values
AC_Mob::AC_Mob()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_Mob::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_Mob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AC_Mob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AC_Mob::Hit(float value)
{
	hp -= value;
	if (hp <= 0)
		Death();
}

void AC_Mob::Death()
{

}