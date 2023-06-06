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
	UPROPERTY(EditAnywhere)
		float speed = 1;
	UPROPERTY(EditAnywhere)
		TArray<FVector> posRefList;
	UPROPERTY(EditAnywhere)
		int lineResolution = 10;
	UPROPERTY(EditAnywhere)
		TArray<FVector> LinePoseList;
	bool isArrived;
	float dstVal;

	FVector st  {-500,0,0};
	FVector mid {0,0,500};
	FVector en  {500,0,0};

	AActor* target;



private:
		float myDeltaTime;
		UStaticMeshComponent* rot;
		UBoxComponent* MyBoxComponent;
		float curTime = 0;
		FVector Lerp3Points(FVector st, FVector mid, FVector en, float t);
		void InitRot();

		void StoreLerpPoints(int);
		void DrawLerpLine();
};


