// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "D_Rot.generated.h"

UCLASS()
class BLACKBIN_API AD_Rot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_Rot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
		AActor* stActor;
	UPROPERTY(EditAnywhere)
		AActor* enActor;

	bool isArrived;
	float dstVal;

	FVector st;
	FVector mid;
	FVector en;
	AActor* target;

	FVector a1;
	FVector a2;

private:
	
		UStaticMeshComponent* rot;
	
		UBoxComponent* MyBoxComponent;
};
