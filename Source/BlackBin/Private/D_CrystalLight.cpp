// Fill out your copyright notice in the Description page of Project Settings.


#include "D_CrystalLight.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Materials/MaterialInterface.h>


// Sets default values
AD_CrystalLight::AD_CrystalLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create components 
	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("sphereCollider"));
	meshComp	   = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));

	RootComponent = sphereCollider;
	meshComp->SetupAttachment(RootComponent);

	// StaticMesh
	ConstructorHelpers::FObjectFinder<UStaticMesh> tmpMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (tmpMesh.Succeeded()) {
		meshComp->SetStaticMesh(tmpMesh.Object);
	}

	// material Setup
	ConstructorHelpers::FObjectFinder<UMaterialInterface> tmpMat(TEXT("/Script/Engine.Material'/Game/DKW/Material/Barriers/M_CrystalLight.M_CrystalLight'"));
	if (tmpMat.Succeeded()) {
		meshComp->SetMaterial(0, tmpMat.Object);
	}
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
	// 여기서는
}

