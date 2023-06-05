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

// Called every frame
void AD_LightStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	isActive = IsAllCrystalShine();
	if(isActive)
	// is Active : 재질을 바꿔준다
}

bool AD_LightStatue::IsAllCrystalShine()
{
	// 이 함수는 나중에 크리스탈이 액티브될 때만 불려야 연산에 좋다
	for (auto& ele : crystals) {
		if (!ele->isShine) { print("not all crystal is shining"); return false; }
		else { print("All crystal is shining") }
	}
	return true;
}

void AD_LightStatue::ShineStatue()
{

}

