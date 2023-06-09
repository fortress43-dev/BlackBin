// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Mob.generated.h"

UCLASS()
class BLACKBIN_API AC_Mob : public ACharacter
{
	GENERATED_BODY()

public:
	float max_hp	= 100;
	int	hp			= max_hp;
	// Sets default values for this character's properties
	UPROPERTY(EditAnywhere)
	int team = 0;
	AC_Mob();
	void Hit(float value);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void Death();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
