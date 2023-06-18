// Fill out your copyright notice in the Description page of Project Settings.


#include "D_Crystal.h"
#include "D_CrystalLight.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "ActorSequenceComponent.h"
#include "DebugMessages.h"

// Sets default values
AD_Crystal::AD_Crystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#pragma region Component Setting
	boxComp = CreateDefaultSubobject<UBoxComponent>("boxComp");
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>("meshComp");

	RootComponent = boxComp;
	meshComp->SetupAttachment(boxComp);

	// init transform
	// meshComp->SetRelativeRotation(FRotator(90,0,0));
	
#pragma endregion


#pragma region ConstructHelpers
	// Crystal light
	ConstructorHelpers::FClassFinder<AD_CrystalLight>tmpObj(TEXT("/Script/Engine.Blueprint'/Game/DKW/Blueprints/BP_CrystalLight.BP_CrystalLight_C'"));
	if (tmpObj.Succeeded()) {
		LightBarrierFactory = tmpObj.Class;
	}

	// Crystal
	ConstructorHelpers::FObjectFinder<UStaticMesh>tmpCrystal(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tmpCrystal.Succeeded()) {
		meshComp->SetStaticMesh(tmpCrystal.Object);
	}
#pragma endregion

#pragma region Collision Setting
	// Collision Channel Settings
	// barrier		-> crystal 0
	// crystalLight -> crystal 0

	boxComp->SetCollisionProfileName("Crystal");

#pragma endregion
}

// Called when the game starts or when spawned
void AD_Crystal::BeginPlay()
{
	
	Super::BeginPlay();
	
	auto Cube = FindComponentByClass<UStaticMeshComponent>();
	auto Material = Cube->GetMaterial(0);

	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	Cube->SetMaterial(0, DynamicMaterial);

	// 현재 이 함수가 begin overlap 에 들어가면 stack over flow 를 발생시킴
	SpawnCrystalLight();
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
	
	
}

void AD_Crystal::ShineCrystal() {
	float blend = 0.5f + FMath::Cos(curTime*2);
	float emissive = FMath::Cos(curTime * 2);
	DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), blend);
	DynamicMaterial->SetScalarParameterValue(TEXT("Emissive"), emissive);
	if (curTime > 100) curTime = 0;

}

void AD_Crystal::ColorChange() {
	// UActorSequenceComponent* seqComp;
	//seqComp->PlaySequence();
}

void AD_Crystal::SpawnCrystalLight() {
	isShine = true;
	print("Light Barrier collided on Crystal");

	// Spawn Option 을 지정해준다
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 다른 라이트 베리어를 자신의 위치에 스폰한다.
	FVector pos = GetActorLocation();
	if (LightBarrierFactory) {
		GetWorld()->SpawnActor<AD_CrystalLight>(LightBarrierFactory, pos, FRotator::ZeroRotator, param);
	}
	else {
		print("no light Factory !!!!!!!!!!!");
	}
	print("Light Barrier created");
	// 이펙트가 실행된다
	FVector EffectLocation = GetActorLocation();
	if (NS_CrystalEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_CrystalEffect, EffectLocation);
	}
	if (SB_CrystalSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_CrystalSound, EffectLocation, FRotator::ZeroRotator);
	}
}