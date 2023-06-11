// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "C_Barrier.h"
#include "D_Rot.generated.h"


UENUM(BlueprintType)
enum RotCollect {
	hidden,
	activating,
	activated,
	collected
};

UENUM(BlueprintType)
enum RotState {
	none,
	follow,
	fired,
	orbitTarget,
	explosion
};

UCLASS()
class BLACKBIN_API AD_Rot : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_Rot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	//virtual void NotifyActorBeginCursorOver();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	AActor* target;
	RotCollect rotCollect = RotCollect::hidden;
	RotState rotState = RotState::none;

	UPROPERTY(EditAnywhere)
		UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
		bool onCollectKey = false;
	UPROPERTY(BlueprintReadOnly)
		bool useAIMove = false;
private:

	//Rot Activating Animation
	float curTime;
	float speed = 10;
	float dt;
	FVector p1;
	FVector p2;
	float rotActiveHeight = 100.0f;
	void ActivateRot();
	void CollectActivatedRot();
};


