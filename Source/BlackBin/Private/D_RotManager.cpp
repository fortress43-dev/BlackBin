// Fill out your copyright notice in the Description page of Project Settings.


#include "D_RotManager.h"
#include "InputMappingContext.h"
#include "DebugMessages.h"

// Sets default values
AD_RotManager::AD_RotManager()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AD_RotManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AD_RotManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AD_RotManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
}

void AD_RotManager::RotActionSkill()
{
    print("called rotAcion");
}

void AD_RotManager::InteractionStart()
{
    print("called interaction");
	//if (curRot)curRot->onCollectKey == true;
}



