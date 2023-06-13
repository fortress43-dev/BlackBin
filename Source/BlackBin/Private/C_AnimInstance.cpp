


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
			UMovementComponent* moveComp = Character->GetMovementComponent();
			UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(moveComp);
			IsFall = CharacterMovement->IsFalling();
			Velocity = CharacterMovement->Velocity;
			FVector Acc = CharacterMovement->GetCurrentAcceleration();
			ShouldMove = (Acc != FVector(0) && MovementSpeed > 3);
			UE_LOG(LogTemp, Warning, TEXT("%d"), IsFall);
		}
	}
}