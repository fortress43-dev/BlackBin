// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D_Crystal.h"
#include "D_LightStatue.generated.h"


UCLASS()
class BLACKBIN_API AD_LightStatue : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_LightStatue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnyWhere, Category = "TopCategory|SubCategory|here")
	TArray<class AD_Crystal*> crystals;
	
	UPROPERTY(EditAnyWhere)
		bool isActive;

private:
	bool IsAllCrystalShine();
	// 연결된 다른 물체의 재질에 영향을 주기 않기 위해 dyna-inst만듬
	UMaterialInstanceDynamic* DynamicMaterial;

	void ShineStatue();


};
