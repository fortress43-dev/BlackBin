// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D_Crystal.generated.h"

class UMaterialInstanceDynamic;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class BLACKBIN_API AD_Crystal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_Crystal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UNiagaraSystem* NS_CrystalEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		USoundBase* SB_CrystalSound;

	UPROPERTY(EditAnywhere, Category = "Factory")
		TSubclassOf<class AD_CrystalLight> LightBarrierFactory;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnyWhere)
	bool isShine;
	float curTime;
	void ShineCrystal();
	void ColorChange();

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent*		boxComp;
private:
	void SpawnCrystalLight();
	
};
