// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_AnimInstance.generated.h"

class AC_Player;
class MovementComponent;
/**
 * 
 */
UCLASS()
class BLACKBIN_API UC_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	public:
		UC_AnimInstance();
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	private:
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
			float MovementSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
			bool IsFall;
};
