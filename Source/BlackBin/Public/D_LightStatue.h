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
	UPROPERTY(EditAnyWhere, Category = "Crystals")
	TArray<class AD_Crystal*> crystals;
	
	UPROPERTY(VisibleAnywhere)
		bool isActive;

	float curTime;
	UPROPERTY(EditAnyWhere)
		float changeTime = 5;
	UPROPERTY(EditAnyWhere)
		float lightPower = 2;

	enum StatueState {
		normal,
		shine
	};

	StatueState statueState = normal;
private:
	
	// 연결된 다른 물체의 재질에 영향을 주기 않기 위해 dyna-inst만듬
	UMaterialInstanceDynamic* DynamicMaterial;


	bool IsAllCrystalShine();
	void Change2ShineStatue();
};
