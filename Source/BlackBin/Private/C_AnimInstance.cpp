


// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "C_Player.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
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
			ArrowZ = Host->FollowCamera->GetComponentRotation().Vector().Z;
			ArrowP = Character->gageArrow/100;
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
		FRotator SetRot = Host->GetActorRotation();
		Host->GetCharacterMovement()->Velocity.X = SetRot.Vector().X * 500;
		Host->GetCharacterMovement()->Velocity.Y = SetRot.Vector().Y * 500;
		Host->Trail->Activate();
	}
}

void UC_AnimInstance::AnimNotify_EndTrail()
{
	if (Host)
	{
		Host->GetCharacterMovement()->Velocity.X = 0;
		Host->GetCharacterMovement()->Velocity.Y = 0;
		Host->Trail->Deactivate();
	}
}