// Fill out your copyright notice in the Description page of Project Settings.


#include "H_AnimInst.h"
#include "H_EnemyCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>

UH_AnimInst::UH_AnimInst()
{
    static ConstructorHelpers::FObjectFinder<UAnimMontage> SAttack(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/HM_SAttack.HM_SAttack'"));
    if (SAttack.Succeeded())
    {
        UE_LOG(LogTemp, Warning, TEXT("adafaf"));
        SAttackMontage = SAttack.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> RunningMon(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/HM_Running.HM_Running'"));
    if (RunningMon.Succeeded())
    {
        RunningMongtage = RunningMon.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> BackMoveMon(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/HM_BackStep.HM_BackStep'"));
    if (BackMoveMon.Succeeded())
    {
        BackMoveMongtage = BackMoveMon.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> BasicMoveMong(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/HM_BasicAttack.HM_BasicAttack'"));
    if (BasicMoveMong.Succeeded())
    {
        BasicAttackMongtage = BasicMoveMong.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> BasicOneMong(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/HM_BasicAttack2.HM_BasicAttack2'"));
    if (BasicOneMong.Succeeded())
    {
        BasicAttackOneMongtage = BasicOneMong.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> BasicTwoMong(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/HM_BasicAttack3.HM_BasicAttack3'"));
    if (BasicTwoMong.Succeeded())
    {
        BasicAttackTwoMongtage = BasicTwoMong.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> MovingRight(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/HM_MovingRight.HM_MovingRight'"));
    if (MovingRight.Succeeded())
    {
        MoveRightMongtage = MovingRight.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage> MovingLeft(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/HM_MovingLeft.HM_MovingLeft'"));
    if (MovingLeft.Succeeded())
    {
        MoveLeftMongtage = MovingLeft.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> MovingBackward(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/HM_MovingBackward.HM_MovingBackward'"));
    if (MovingBackward.Succeeded())
    {
        MoveBackwardMongtage = MovingBackward.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> MovingForward(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/HM_MovingForward.HM_MovingForward'"));
    if (MovingForward.Succeeded())
    {
        MpveForwardMongtage = MovingForward.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> MovingIdle(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/HM_MovingIdle.HM_MovingIdle'"));
    if (MovingIdle.Succeeded())
    {
        IdleMongtage = MovingIdle.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> Dying(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/DyingAnim.DyingAnim'"));
    if (Dying.Succeeded())
    {
        DyingMongtage = Dying.Object;
    }

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

void UH_AnimInst::PlaySAttackMontage()
{
    
    if (!Montage_IsPlaying(SAttackMontage))
    {
        Montage_Play(SAttackMontage, 1.0f);
    }
}

void UH_AnimInst::PlayRunMontage()
{
    if (!Montage_IsPlaying(RunningMongtage))
    {
        Montage_Play(RunningMongtage, 1.0f);
    }
}

void UH_AnimInst::PlayBackmoveMontage()
{
    if (!Montage_IsPlaying(BackMoveMongtage))
    {
        Montage_Play(BackMoveMongtage, 1.0f);
    }
}

void UH_AnimInst::PlayBasicAttackMongtage()
{
    if (!Montage_IsPlaying(BasicAttackMongtage))
    {
        Montage_Play(BasicAttackMongtage, 1.0f);
    }
}

void UH_AnimInst::PlayBasicAttackOneMongtage()
{
    if (!Montage_IsPlaying(BasicAttackOneMongtage))
    {
        Montage_Play(BasicAttackOneMongtage, 1.0f);
    }
}

void UH_AnimInst::PlayBasicAttackTwoMongtage()
{
    if (!Montage_IsPlaying(BasicAttackTwoMongtage))
    {
        Montage_Play(BasicAttackTwoMongtage, 1.0f);
    }
}

void UH_AnimInst::PlayMovingForwardMontage()
{
    if (!Montage_IsPlaying(MpveForwardMongtage))
    {
        Montage_Play(MpveForwardMongtage, 1.0f);
    }
}

void UH_AnimInst::PlayMovingBackwardMontage()
{
    if (!Montage_IsPlaying(MoveBackwardMongtage))
    {
        Montage_Play(MoveBackwardMongtage, 1.0f);
    }
}

void UH_AnimInst::PlayMovingRightMongtage()
{
    if (!Montage_IsPlaying(MoveRightMongtage))
    {
        Montage_Play(MoveRightMongtage, 1.0f);
    }
}

void UH_AnimInst::PlayMovingLeftMongtage()
{
    if (!Montage_IsPlaying(MoveLeftMongtage))
    {
        Montage_Play(MoveLeftMongtage, 1.0f);
    }
}

void UH_AnimInst::PlayMovingIdleMongtage()
{
    if (!Montage_IsPlaying(IdleMongtage))
    {
        Montage_Play(IdleMongtage, 1.0f);
    }
}

void UH_AnimInst::PlayDyingMongtage()
{
    if (!Montage_IsPlaying(DyingMongtage))
    {
        Montage_Play(DyingMongtage, 1.0f);
    }
}

void UH_AnimInst::AnimNotify_basicOneAttack()
{
    attNotify.Broadcast();
}
