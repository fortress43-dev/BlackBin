// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_HitBox.generated.h"

UCLASS()
class BLACKBIN_API AC_HitBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere)
	bool IsBrocken;
	UPROPERTY(EditAnywhere)
	float lifeTime = 0;
	UPROPERTY(EditAnywhere)
	int team	= 0;
	UPROPERTY(EditAnywhere)
	int dmg		= 0;
	UPROPERTY(EditAnywhere)
	float slowmotion = 1;
	UPROPERTY(EditAnywhere)
	class AC_Mob* Host;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* boxComp;
	TObjectPtr<class USoundBase> hitsound;
	TObjectPtr<class UNiagaraSystem> Fx;
protected:
	AC_HitBox();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
