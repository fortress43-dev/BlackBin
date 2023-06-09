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

	// 재질을 수정하기 위해 저장한다
	auto Cube = FindComponentByClass<UStaticMeshComponent>();
	auto Material = Cube->GetMaterial(0);

	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	Cube->SetMaterial(0, DynamicMaterial);
	
}
float changeTime;
// Called every frame
void AD_LightStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	isActive = IsAllCrystalShine();

	if (statueState == normal && isActive) {
		Change2ShineStatue();
		curTime += DeltaTime;
		// Tick밖에서는 deltatime을 쓰는 것을 권장하지 않는다
	}
	// is Active : 재질을 바꿔준다
}

bool AD_LightStatue::IsAllCrystalShine()
{
	// 이 함수는 나중에 크리스탈이 액티브될 때만 불려야 연산에 좋다
	for (auto& ele : crystals) {
		if (!ele->isShine) { return false; }
		
	}
	return true;
}


void AD_LightStatue::Change2ShineStatue()
{
	// 3초동안 재질이 변하는 과정을 겪음
	float emissive = curTime * lightPower;
	DynamicMaterial->SetScalarParameterValue(TEXT("Emissive"), emissive);
	if (curTime > changeTime) { statueState = shine; print("Statue is shining"); changeTime = 0; }
}

