// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Mob.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DebugMessages.h"

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
	//printf("mob hp : %d", hp);

}

// Called to bind functionality to input
void AC_Mob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AC_Mob::Hit(AC_HitBox* box, float value)
{
	hp -= value;
	if (hp <= 0)
		Death();
	//FString RotationString = FString::Printf(TEXT("Rotation: %f %f"), team, value);
	//UKismetSystemLibrary::PrintString(this, RotationString, true, false, FLinearColor::Red, 2.0f);
}
void AC_Mob::Hit(float value)
{
	hp -= value;
	if (hp <= 0)
		Death();
	//FString RotationString = FString::Printf(TEXT("Rotation: %f %f"), team, value);
	//UKismetSystemLibrary::PrintString(this, RotationString, true, false, FLinearColor::Red, 2.0f);
}
void AC_Mob::Death()
{
	Destroy();
}