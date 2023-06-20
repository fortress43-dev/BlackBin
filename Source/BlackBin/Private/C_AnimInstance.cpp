


// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


#include "C_Player.h"
#include "NiagaraComponent.h"
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
			UMovementComponent* moveComp = Character->GetMovementComponent();
			UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(moveComp);
			IsFall = CharacterMovement->IsFalling();
			Velocity = CharacterMovement->Velocity;
			FVector Acc = CharacterMovement->GetCurrentAcceleration();
			ShouldMove = (Acc != FVector(0) && MovementSpeed > 3);
			Host = Character;
			IsArrow = bool(Character->gageArrow);
		}
	}
}

void UC_AnimInstance::AnimNotify_AttackHitCheck()
{
	//UE_LOG(LogTemp, Warning, TEXT("111111"));
	OnAttackHitCheck.Broadcast();
}

void UC_AnimInstance::AnimNotify_PowerAttackHitCheck()
{
	OnPowerAttackHitCheck.Broadcast();
}

void UC_AnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}
void UC_AnimInstance::AnimNotify_Cancelable()
{
	OnCancelable.Broadcast();
}

void UC_AnimInstance::AnimNotify_DoRotation()
{
	OnDoRotation.Broadcast();
}


void UC_AnimInstance::AnimNotify_PlayTrail()
{
	if (Host)
	{
		Host->Trail->Activate();
	}
}

void UC_AnimInstance::AnimNotify_EndTrail()
{
	if (Host)
	{
		Host->Trail->Deactivate();
	}
}