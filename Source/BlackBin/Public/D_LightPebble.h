// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D_LightPebble.generated.h"

UCLASS()
class BLACKBIN_API AD_LightPebble : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_LightPebble();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent*		boxComp;
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface*   dynamicMaterial; 
	
	UPROPERTY(EditAnywhere)
	class UActorSequenceComponent* seqComp;

};
