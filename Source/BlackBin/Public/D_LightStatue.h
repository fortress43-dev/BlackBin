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
	// ����� �ٸ� ��ü�� ������ ������ �ֱ� �ʱ� ���� dyna-inst����
	UMaterialInstanceDynamic* DynamicMaterial;

	void ShineStatue();


};
