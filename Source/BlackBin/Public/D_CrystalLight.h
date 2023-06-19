// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "D_CrystalLight.generated.h"

UCLASS()
class BLACKBIN_API AD_CrystalLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_CrystalLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent*		sphereCollider;
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface*	material;
	
	// curve float
	float RotateValue;
	float CurveFloatValue;
	float TimelineValue;
	
	UPROPERTY(EditAnywhere)
	UCurveFloat* AnimCurve;
	FTimeline MyTimeline;

	UPROPERTY(EditAnywhere)
		float maxSize = 7.f;

	FVector scale;
	float curTime = 0;
	UFUNCTION()
	void ScaleUP();

	UFUNCTION()
	void DestroyCrystal();
};
