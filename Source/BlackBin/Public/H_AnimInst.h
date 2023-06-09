// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "H_AnimInst.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnbasicOneAttackDelegate);

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

	void PlaySAttackMontage();

	void PlayRunMontage();

	void PlayBackmoveMontage();

	void PlayBasicAttackMongtage();

	void PlayBasicAttackOneMongtage();

	void PlayBasicAttackTwoMongtage();

	void PlayMovingForwardMontage();

	void PlayMovingBackwardMontage();

	void PlayMovingRightMongtage();

	void PlayMovingLeftMongtage();

	void PlayMovingIdleMongtage();

	void PlayDyingMongtage();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Velocity;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
    UAnimMontage* SAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RunningMongtage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* BackMoveMongtage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* BasicAttackMongtage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* BasicAttackOneMongtage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* BasicAttackTwoMongtage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* MoveRightMongtage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* MoveLeftMongtage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* MoveBackwardMongtage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* MpveForwardMongtage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* IdleMongtage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DyingMongtage;

	UFUNCTION()
	void AnimNotify_basicOneAttack();

	FOnbasicOneAttackDelegate attNotify;


};
