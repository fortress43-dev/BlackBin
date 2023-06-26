// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "C_HitBox.h"
#include "C_Arrow.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBIN_API AC_Arrow : public AC_HitBox
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	AC_Arrow();
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
public:
	// Called every frame
	TObjectPtr<class UNiagaraSystem> ArrowFx;
	bool ativate;
	virtual void Tick(float DeltaTime) override;

};
