// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_AnimInstance.generated.h"

class AC_Player;
class MovementComponent;

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCancelableDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDoRotationDelegate);
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

		FOnNextAttackCheckDelegate	OnNextAttackCheck;
		FOnAttackHitCheckDelegate	OnAttackHitCheck;
		FOnCancelableDelegate		OnCancelable;
		FOnDoRotationDelegate		OnDoRotation;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
			bool IsBarrier;
	private:
		UFUNCTION()
		void AnimNotify_NextAttackCheck();

		UFUNCTION()
		void AnimNotify_AttackHitCheck();

		UFUNCTION()
		void AnimNotify_Cancelable();

		UFUNCTION()
		void AnimNotify_DoRotation();

		UFUNCTION()
		void AnimNotify_EndTrail();

		UFUNCTION()
		void AnimNotify_PlayTrail();
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
			float MovementSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
			bool IsFall;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
			bool ShouldMove;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
			FVector Velocity;

			class AC_Player* Host;
};
