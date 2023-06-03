// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D_Crystal.generated.h"

class UMaterialInstanceDynamic;

UCLASS()
class BLACKBIN_API AD_Crystal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_Crystal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	bool isShine = false;
	float curTime;
	void ShineCrystal();
	void ColorChange();
private:
	UMaterialInstanceDynamic* DynamicMaterial;

};
