// Fill out your copyright notice in the Description page of Project Settings.


#include "D_LightStatue.h"
#include "DebugMessages.h"

// Sets default values
AD_LightStatue::AD_LightStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AD_LightStatue::BeginPlay()
{
	Super::BeginPlay();

	// ������ �����ϱ� ���� �����Ѵ�
	auto Cube = FindComponentByClass<UStaticMeshComponent>();
	auto Material = Cube->GetMaterial(0);

	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	Cube->SetMaterial(0, DynamicMaterial);
	
}

// Called every frame
void AD_LightStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	isActive = IsAllCrystalShine();
	if(isActive)
	// is Active : ������ �ٲ��ش�
}

bool AD_LightStatue::IsAllCrystalShine()
{
	// �� �Լ��� ���߿� ũ����Ż�� ��Ƽ��� ���� �ҷ��� ���꿡 ����
	for (auto& ele : crystals) {
		if (!ele->isShine) { print("not all crystal is shining"); return false; }
		else { print("All crystal is shining") }
	}
	return true;
}

void AD_LightStatue::ShineStatue()
{

}

