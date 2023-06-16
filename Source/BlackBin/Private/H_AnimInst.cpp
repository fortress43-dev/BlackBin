// Fill out your copyright notice in the Description page of Project Settings.


#include "H_AnimInst.h"
#include "H_EnemyCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>

UH_AnimInst::UH_AnimInst()
{

}

void UH_AnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    auto Pawn = TryGetPawnOwner();
    if (::IsValid(Pawn))
    {
        auto MovementSpeed = Pawn->GetVelocity().Size();
        auto Character = Cast<AH_EnemyCharacter>(Pawn);
        if (Character)
        {
            UMovementComponent* moveComp = Character->GetMovementComponent();
            UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(moveComp);
           
            Velocity = CharacterMovement->Velocity;
            FVector Acc = CharacterMovement->GetCurrentAcceleration();
            /*ShouldMove = (Acc != FVector(0) && MovementSpeed > 3);*/
        }
    }
}