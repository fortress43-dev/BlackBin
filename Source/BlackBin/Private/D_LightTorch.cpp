// Fill out your copyright notice in the Description page of Project Settings.


#include "D_LightTorch.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DebugMessages.h"



// Sets default values
AD_LightTorch::AD_LightTorch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AD_LightTorch::BeginPlay()
{
	Super::BeginPlay();
	
}

void AD_LightTorch::NotifyActorBeginOverlap(AActor* OtherActor)
{
	print("LightTorch is collided");
	if (isActive)return;
	// Effect, Light, Sound 를 spawn한다
	FVector EffectLocation = GetActorLocation();
	if (NS_TorchEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_TorchEffect, EffectLocation);
	}
	if (SB_TorchSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_TorchSound, EffectLocation, FRotator::ZeroRotator);
	}

	// 콘의 머테리얼 정보를 가져온다
	// 파라메터를 바꾼다
	auto Cube = FindComponentByClass<UStaticMeshComponent>();
	auto Material = Cube->GetMaterial(0);

	auto DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	Cube->SetMaterial(0, DynamicMaterial);

	DynamicMaterial->SetScalarParameterValue(TEXT("Opacity"), 1.0f);
	DynamicMaterial->SetScalarParameterValue(TEXT("Emissive"), emissive);

	// prevent Spawning again
	isActive = true;
}

// Called every frame
void AD_LightTorch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

