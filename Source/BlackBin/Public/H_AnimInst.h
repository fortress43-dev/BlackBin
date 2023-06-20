// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "H_AnimInst.generated.h"

/**
 * 
 */
UCLASS()
class BLACKBIN_API UH_AnimInst : public UAnimInstance
{
	GENERATED_BODY()

		UH_AnimInst();
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Velocity;

	
};
