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
	AC_HitBox();
	float lifeTime = 0;
	int team	= 0;
	UPROPERTY(EditAnywhere)
	int dmg		= 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* boxComp;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* meshComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
