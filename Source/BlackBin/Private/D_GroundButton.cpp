// Fill out your copyright notice in the Description page of Project Settings.

#include "D_GroundButton.h"
#include "DebugMessages.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AD_GroundButton::AD_GroundButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AD_GroundButton::BeginPlay()
{
	Super::BeginPlay();
}

void AD_GroundButton::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

// Called every frame
void AD_GroundButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

