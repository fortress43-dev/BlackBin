// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D_LightTorch.generated.h"

UCLASS()
class BLACKBIN_API AD_LightTorch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_LightTorch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UNiagaraSystem* NS_TorchEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		USoundBase* SB_TorchSound;

	UPROPERTY(EditAnywhere)
		float emissive = 10;

	UPROPERTY(VisibleAnywhere)
		bool isActive = false;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 빛을 맞으면 밝은 상태로 변한다
	bool isShine = false;

};
