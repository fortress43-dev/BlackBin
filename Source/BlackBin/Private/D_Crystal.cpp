// Fill out your copyright notice in the Description page of Project Settings.


#include "D_Crystal.h"
#include "Components/BoxComponent.h"
#include"Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"

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
	// ���� ������
	isShine = true;

	// ����Ʈ�� ����ȴ�
	// �ٸ� ����Ʈ ����� �ڽ��� ��ġ�� �����Ѵ�. ������ ������� 1/2
	
}

void AD_Crystal::ShineCrystal() {
	float blend = 0.5f + FMath::Cos(curTime*2);
	DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), blend);
	if (curTime > 100) curTime = 0;

}

void AD_Crystal::ColorChange() {

}
