// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "H_EnemySpawner.generated.h"

UCLASS()
class BLACKBIN_API AH_EnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AH_EnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<class AH_EnemyCharacter> EnemyCharacterClass;
	
	UPROPERTY()
	class AH_EnemySpawnLocation* Target;
	void SpawnEnemy();
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;
};
