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

	void PlaySAttackMontage();

	void PlayRunMontage();

	void PlayBackmoveMontage();

	void PlayBasicAttackMongtage();

	void PlayBasicAttackOneMongtage();

	void PlayBasicAttackTwoMongtage();

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
};
