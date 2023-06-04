// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "D_GroundButton.generated.h"


UCLASS()
class BLACKBIN_API AD_GroundButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_GroundButton();

	UPROPERTY(EditAnywhere, Category = Animations)
		UAnimMontage* triggeredAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UAnimSequence* animSquence;
	UPROPERTY(EditAnywhere, Category = Animations)

		class ULevelSequencePlayer* realanim;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:



	//UPROPERTY(EditAnywhere)
	//	ALevelSequeceActor* animSource;
};
