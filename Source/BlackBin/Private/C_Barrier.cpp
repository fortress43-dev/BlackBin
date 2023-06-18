// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Barrier.h"
#include "C_Player.h"

// Sets default values
AC_Barrier::AC_Barrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
AC_Barrier::~AC_Barrier()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	if (Host != nullptr)
	{
		//Host=><ABarrier>Barrier = nullptr;
	}
}
// Called when the game starts or when spawned
void AC_Barrier::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AC_Barrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsBoom)
	{
		FVector currentScale = GetActorScale3D();
		FVector newScale = currentScale * (1.00 + DeltaTime*5);
		SetActorScale3D(newScale);
		if (newScale.X >= 9.)
		{
			if (Host != nullptr)
			{
				Host->Barrier = nullptr;
			}
			Destroy();
		}
	}
}