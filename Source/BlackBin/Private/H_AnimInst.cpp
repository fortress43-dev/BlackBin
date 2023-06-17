// Fill out your copyright notice in the Description page of Project Settings.


#include "H_AnimInst.h"
#include "H_EnemyCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>

UH_AnimInst::UH_AnimInst()
{
    static ConstructorHelpers::FObjectFinder<UAnimMontage> SAttack(TEXT("/Script/Engine.AnimMontage'/Game/HSH/BluePrint/HM_SAttack.HM_SAttack'"));
    if (SAttack.Succeeded())
    {
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
