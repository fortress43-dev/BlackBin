// Fill out your copyright notice in the Description page of Project Settings.


#include "D_Crystal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AD_Crystal::AD_Crystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AD_Crystal::BeginPlay()
{
	Super::BeginPlay();
	
	auto Cube = FindComponentByClass<UStaticMeshComponent>();
	auto Material = Cube->GetMaterial(0);

	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	Cube->SetMaterial(0, DynamicMaterial);

}

// Called every frame
void AD_Crystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isShine == true) { 
		curTime += DeltaTime;
		ShineCrystal(); 
	}
	
}

void AD_Crystal::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	// 빛을 밝힌다
	isShine = true;
	
	// 이펙트가 실행된다
	FVector EffectLocation = GetActorLocation();
	if (NS_CrystalEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_CrystalEffect, EffectLocation);
	}
	if (SB_CrystalSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_CrystalSound, EffectLocation, FRotator::ZeroRotator);
	}
	// 다른 라이트 베리어를 자신의 위치에 스폰한다. 하지만 사이즈는 1/2
	
}

void AD_Crystal::ShineCrystal() {
	float blend = 0.5f + FMath::Cos(curTime*2);
	DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), blend);
	if (curTime > 100) curTime = 0;

}

void AD_Crystal::ColorChange() {

}
