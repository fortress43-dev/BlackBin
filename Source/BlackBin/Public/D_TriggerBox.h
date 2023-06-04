// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Actor.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "D_TriggerBox.generated.h"

UCLASS()
class BLACKBIN_API AD_TriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_TriggerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	UFUNCTION()
		void PlayAnim(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY(EditAnywhere)
		ALevelSequenceActor* animFactory;
	UPROPERTY(EditAnywhere)
		bool isActive;

};
