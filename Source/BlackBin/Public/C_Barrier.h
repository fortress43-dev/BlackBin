// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Barrier.generated.h"

class AC_Player;
UCLASS()
class BLACKBIN_API AC_Barrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_Barrier();
	~AC_Barrier();

	bool IsBoom		= false;
	int lifeTime	= 0;
	AC_Player* Host = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
