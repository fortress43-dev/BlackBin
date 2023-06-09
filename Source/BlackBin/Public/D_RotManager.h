// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D_RotManager.generated.h"

UCLASS()
class BLACKBIN_API AD_RotManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_RotManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	int rotLevel;
	TArray<AActor*> rots;

	enum RotState {
		guide, attack, healing, moveHeavy
	};

	RotState rotState = guide;
	enum RotAttackState {
		onGoing, circleMove, Explosion
	};

	void StateMachine();

	// basic rotAction
	void Guide();
	void Attack();
	void MoveHeavy();
};
