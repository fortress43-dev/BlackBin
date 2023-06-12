


// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "C_Player.h"

UC_AnimInstance::UC_AnimInstance()
{
	
}

void UC_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		MovementSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<AC_Player>(Pawn);
		if (Character)
		{
			IsFall = Character->GetMovementComponent()->IsFalling();
		}
	}
}