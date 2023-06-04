// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "Engine/TriggerBox.h"
#include "MyTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBIN_API AMyTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


public:
	AMyTriggerBox();

	UFUNCTION()
		void PlayAnim(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY(EditAnywhere)
		ALevelSequenceActor* animFactory;
	UPROPERTY(EditAnywhere)
		bool isActive;

};
