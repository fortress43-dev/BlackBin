// Fill out your copyright notice in the Description page of Project Settings.


#include "H_EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "C_HitBox.h"
#include "C_Mob.h"
#include "Math/RandomStream.h"
#include "DebugMessages.h"
#include "H_AnimInst.h"

// Sets default values
AH_EnemyCharacter::AH_EnemyCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UBlueprint> HitBoxObject(TEXT("/Script/Engine.Blueprint'/Game/CSK/Blueprints/BP_HitBox.BP_HitBox'"));
    if (HitBoxObject.Object)
    {
        HitBoxClass = (UClass*)HitBoxObject.Object->GeneratedClass;
    }

    // Set size for collision capsule

    //GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    // Create mesh component and attach it to the root component

    // Set the character movement mode to use walking
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->GravityScale = 1.0f;

    // Disable the default jump capability of the character
    GetCharacterMovement()->JumpZVelocity = 0.0f;
    GetCharacterMovement()->AirControl = 0.0f;

    // Set the initial movement direction to zero
    dir = FVector::ZeroVector;

    // Set the initial movement speed and dash speed
    moveSpeed = 100.0f; // Adjust the value as needed
    dashSpeed = 700.0f; // Adjust the value as needed

    randomN = FMath::RandRange(1, 100);
    ranTime = 4;

    //���߿� �齺�� �ѹ��� �ҋ� ����
    /*AnimIns = Cast<UH_AnimInst>(GetMesh()->GetAnimInstance());
    AnimIns->OnMontageEnded.AddDynamic(this, &AH_EnemyCharacter::AnimEnded);*/
}

//���߿� �齺�� �ѹ��� �ҋ� ����
//void AH_EnemyCharacter::PostInitializeComponents()
//{
//    Super::PostInitializeComponents();
//}
// Called when the game starts or when spawned
void AH_EnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    //GetWorldTimerManager().SetTimer(TimerHandle, this, &AH_EnemyCharacter::TimerEvent, 3.0f, true);

    AnimInstance = Cast<UH_AnimInst>(GetMesh()->GetAnimInstance());
    AnimInstance->OnMontageEnded.AddDynamic(this, &AH_EnemyCharacter::OnAnimeMontageEnded);
    AnimInstance->attNotify.AddLambda([this]() -> void
        {
            //�� ��
            SpawnHitBox();
        });
}
//void AH_EnemyCharacter::TimerEvent()
//{
//    GetWorldTimerManager().SetTimer(TimerHandle, this, &AH_EnemyCharacter::TimerEvent, FMath::FRandRange(3.f, 4.f), true);
//    Checking();
//}
// Called every frame
void AH_EnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);


    dt = DeltaTime;
    ct += DeltaTime;
    ct2 += DeltaTime;
    if (MoveState != EBossMovingState::Dash && MoveState != EBossMovingState::Dying && MoveState != EBossMovingState::SAttack && MoveState != EBossMovingState::Attacking && MoveState != EBossMovingState::BackStep && MoveState != EBossMovingState::Idle) {
        StateTimer += DeltaTime;

    }
    if (StateTimer > 2.f)
    {

        Checking();
        StateTimer = -FMath::FRandRange(1.f, 3.f);
    }
    // Find the player character in the world
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);


    // ���� ���� ����� �Ÿ��� �˰�ʹ�.
    if (PlayerCharacter)
    {
        FVector enemyLocation = GetActorLocation();
        FVector playerLocation = PlayerCharacter->GetActorLocation();
        distance = FVector::Distance(enemyLocation, playerLocation);
        FVector Direction = (playerLocation - enemyLocation).GetSafeNormal();

        FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
        FRotator CurrentRotation = GetActorRotation();
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 1);
        NewRotation.Pitch = 0;
        NewRotation.Roll = 0;
        SetActorRotation(NewRotation);

        PlayerLoc = playerLocation;
        EnemyLoc = enemyLocation;
        EnemyRot = TargetRotation;
        EnemyRot.Roll = 0;
        EnemyRot.Pitch = 0;


        //ChangeState(EBossMovingState::Dash);

        switch (MoveState)
        {
        case EBossMovingState::Dash:
            Dash();
            break;
        case EBossMovingState::MovingBackward:
            MovingBackward();
            break;
        case EBossMovingState::MovingForward:
            MovingForward();
            break;
        case EBossMovingState::MovingLeft:
            MovingLeft();
            break;
        case EBossMovingState::MovingRight:
            MovingRight();
            break;
        case EBossMovingState::Staying:
            Staying();
            break;
        case EBossMovingState::Attacking:
            Attacking();
            break;
        case EBossMovingState::BasicOneAttack:
            BasicOneAttack();
            break;
        case EBossMovingState::BasicTwoAttack:
            BasicTwoAttack();
            break;
        case EBossMovingState::BackStep:
            BackStep();
            break;
        case EBossMovingState::SAttack:
            SAttack();
            break;
        case EBossMovingState::Idle:
            Idle();
            break;
        case EBossMovingState::Dying:
            Dying();
            break;
        default:
            break;
        }
    }

}

// Called to bind functionality to input
void AH_EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AH_EnemyCharacter::Dying()
{
    ct3 += dt;
    GetCharacterMovement()->MaxWalkSpeed = 0;
    //Hit(float(Hitbox->dmg));
    DyingMotion();

    SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));

    if (ct3 > 10) {
        Destroy();
    }
}

void AH_EnemyCharacter::Idle()
{
    GetCharacterMovement()->MaxWalkSpeed = 0;

    if (distance < dashTime) {
        MoveState = EBossMovingState::Dash;
    }
}

void AH_EnemyCharacter::MovingBackward()
{
    BackwardMoveAnim();
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �Ǵ� �̵� ��忡 �´� �ٸ� ��带 �����մϴ�.
    AddMovementInput(dir * -1);
    GetCharacterMovement()->MaxWalkSpeed = 150;
    SetActorRotation(EnemyRot);


}

void AH_EnemyCharacter::MovingForward()
{
    ForwardMoveAnim();
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �Ǵ� �̵� ��忡 �´� �ٸ� ��带 �����մϴ�.
    AddMovementInput(dir);
    GetCharacterMovement()->MaxWalkSpeed = 150;
    SetActorRotation(EnemyRot);



}

void AH_EnemyCharacter::MovingRight()
{
    float thetaToRadian = FMath::DegreesToRadians(60);
    float y = FMath::Sin(thetaToRadian);
    float x = FMath::Cos(thetaToRadian);
    FVector target(x, y, 0);
    FVector direc = target - GetActorLocation();
    RightMoveAnim();

    // ���������� �̵��ϴ� ���� ����
    SetActorRotation(EnemyRot);
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �Ǵ� �̵� ��忡 �´� �ٸ� ��带 �����մϴ�.
    AddMovementInput(direc);
    GetCharacterMovement()->MaxWalkSpeed = 150;


}



void AH_EnemyCharacter::MovingLeft()
{

    float thetaToRadian = FMath::DegreesToRadians(300);
    float y = FMath::Sin(thetaToRadian);
    float x = FMath::Cos(thetaToRadian);
    FVector target(x, y, 0);
    FVector direc = target - GetActorLocation();
    LeftMoveAnim();

    SetActorRotation(EnemyRot);
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �Ǵ� �̵� ��忡 �´� �ٸ� ��带 �����մϴ�.
    AddMovementInput(direc);
    GetCharacterMovement()->MaxWalkSpeed = 150;


}


/*randDeg = FMath::RandRange(-90, 90);
//���ʹ� ��ġ�� �÷��̾� ��ġ���� �������� �Ÿ���ŭ ������ �ʹ�?
FRotator Rot = GetCharacterMovement()->GetLastUpdateRotation();
Rot.Pitch = 0;
Rot.Roll = 0;
Rot.Yaw -= randDeg;*/


void AH_EnemyCharacter::Staying()
{

    //IdleAnim();
    GetCharacterMovement()->MaxWalkSpeed = 0;
    ct += dt;

}

void AH_EnemyCharacter::Attacking()
{
    //int RandAttack = FMath::RandRange(1, 100);
    if (distance < 300) {
        GetCharacterMovement()->MaxWalkSpeed = 0;
        //if (RandAttack > 67) {
           // printf("&d", RandAttack);
        FirstBasicAttack();
        //}

        /*else if (RandAttack <= 67, RandAttack > 33) {
            printf("&d", RandAttack);
           SecondBasicAttack();
        }
        else {
            printf("&d", RandAttack);
            ThirdBasicAttack();
        }*/

        //SpawnHitBox();

    }

}

// �뽬�����϶� �뽬�ӵ��� ��� �̵��Ѵ�.
// ��, �����Ÿ� �̻��϶� �뽬��带 �����Ѵ�.
// ���� �����Ÿ� �̸��̸� �뽬��带 ������ �ʹ�.


void AH_EnemyCharacter::Dash()
{
    if (distance > 200) {
        // Dash towards the player character


        AnimInstance->PlayRunMontage();

        dir = PlayerLoc - EnemyLoc;
        dir.Normalize();
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �Ǵ� �̵� ��忡 �´� �ٸ� ��带 �����մϴ�.
        GetCharacterMovement()->MaxWalkSpeed = 1500;
        AddMovementInput(dir);

    }
    else if (distance <= 200) {
        MoveState = EBossMovingState::SAttack;

    }


}

void AH_EnemyCharacter::BackStep()
{

    if (MoveState == EBossMovingState::BackStep && distance < 700)
    {

        BackMove();
        dir = PlayerLoc - EnemyLoc;
        dir.Normalize();
        SetActorRotation(EnemyRot);
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �Ǵ� �̵� ��忡 �´� �ٸ� ��带 �����մϴ�.
        AddMovementInput(dir * -1);
        GetCharacterMovement()->MaxWalkSpeed = 1800;
    }
}

void AH_EnemyCharacter::SAttack() {

    SAttackMongtage();
    //SpawnHitBox();
   
}

void AH_EnemyCharacter::BasicOneAttack()
{
    if (distance < 300) {
        GetCharacterMovement()->MaxWalkSpeed = 0;

        SecondBasicAttack();
        //SpawnHitBox();

    }
    else {
        Checking();
    }


}

void AH_EnemyCharacter::BasicTwoAttack()
{
    if (distance < 300) {
        GetCharacterMovement()->MaxWalkSpeed = 0;
        ThirdBasicAttack();
        //SpawnHitBox();

    }
    else {
        Checking();
    }


}

void AH_EnemyCharacter::BackMove()
{
   
    AnimInstance->PlayBackmoveMontage();
    
}



void AH_EnemyCharacter::DyingMotion()
{
  
    AnimInstance->PlayDyingMongtage();
}

void AH_EnemyCharacter::Checking()
{
    //if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage))
    //{
    //    // ��� ���� �ִϸ��̼��� �ִٸ� �����Ͽ� ��ٸ�
    //    return;
    //}
    printf("Checking");
    if (distance > 900) {
        //if (animationFinished) {
        arrayState = { EBossMovingState::Dash, EBossMovingState::MovingForward, EBossMovingState::MovingRight,EBossMovingState::MovingLeft, EBossMovingState::Staying };
        arrayWeight = { 0.9f, 0.2f, 0.2f, 0.2f, 0.2f };
        //����ġ�� ����ؼ� �ϳ��� �̾ƶ�
        MoveState = GetArrayWeight(arrayState, arrayWeight);

        //}
    }
    else if (distance <= 900 && distance > 300) {
        // if (animationFinished) {
        arrayState = { EBossMovingState::MovingBackward, EBossMovingState::MovingForward, EBossMovingState::MovingLeft,EBossMovingState::MovingRight, EBossMovingState::Staying,EBossMovingState::Dash };
        arrayWeight = { 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.2f };

        MoveState = GetArrayWeight(arrayState, arrayWeight);

        // }
    }
    else if (distance <= 300) {
        // if (animationFinished) {
        arrayState = { EBossMovingState::MovingBackward, EBossMovingState::Attacking,EBossMovingState::BasicOneAttack ,EBossMovingState::BasicTwoAttack, EBossMovingState::BackStep, EBossMovingState::Staying };
        arrayWeight = { 0.1f, 0.3f, 0.2f, 0.2f, 0.1f, 0.1f };

        MoveState = GetArrayWeight(arrayState, arrayWeight);


    }


    printf("%d", MoveState);
    ct2 = 0;
    ct = 0;
}


EBossMovingState AH_EnemyCharacter::GetArrayWeight(const TArray<EBossMovingState>& ArrayState, const TArray<float>& ArrayWeight)
{
    // ����ġ �迭�� �� ���ϱ�
    float TotalWeight = 0.0f;
    for (float Weight : ArrayWeight)
    {
        TotalWeight += Weight;
    }

    // 0���� �ձ����� ������ �����Ͽ� ���� ���� ����
    float RandomValue = FMath::FRandRange(0.0f, TotalWeight);

    // ����ġ�� ���� ��� ����
    float AccumulatedWeight = 0.0f;

    for (int32 Index = 0; Index < ArrayWeight.Num(); ++Index)
    {
        AccumulatedWeight += ArrayWeight[Index];
        if (RandomValue <= AccumulatedWeight)
        {
            return ArrayState[Index];

        }
    }
    return EBossMovingState::Dash;
}


void AH_EnemyCharacter::OnAnimeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == AnimInstance->BackMoveMongtage) {    
        Checking(); 
    }
    else if (Montage == AnimInstance->BasicAttackMongtage) {
        Checking();
    }
    else if (Montage == AnimInstance->BasicAttackMongtage) {
        Checking();
    }
    else if (Montage == AnimInstance->BasicAttackOneMongtage) {
        Checking();
    }
    else if (Montage == AnimInstance->BasicAttackTwoMongtage) {
        Checking();
    }
    else if (Montage == AnimInstance->SAttackMontage) {
        Checking();
    }
    
}

void AH_EnemyCharacter::SpawnHitBox()
{

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;


    FRotator rotator = GetActorRotation();

    FVector SpawnLocation = GetActorLocation();
    FVector addLoc = GetActorForwardVector() * 200;
    SpawnLocation.Z -= 50.f;


    /*AnimInstance = Cast<UH_AnimInst>(GetMesh()->GetAnimInstance());*/

	/* isAttackingOne = AnimInstance->Montage_IsPlaying(AnimInstance->BasicAttackMongtage);
	 isAttackingTwo = AnimInstance->Montage_IsPlaying(AnimInstance->BasicAttackOneMongtage);
	 isAttackingThree = AnimInstance->Montage_IsPlaying(AnimInstance->BasicAttackTwoMongtage);
	 isAttackingStrong = AnimInstance->Montage_IsPlaying(AnimInstance->SAttackMontage);*/
    //2. ���� Ŀ��Ʈ Ÿ���� 3�� �Ѿ��

   /* if (ct > 3) {*/
        //��Ʈ�ڽ��� ��ȯ�Ѵ�

        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        Hitbox = GetWorld()->SpawnActor<AC_HitBox>(HitBoxClass, SpawnLocation + addLoc, rotator, SpawnParams);

        //���� ��Ʈ�ڽ��� ��ȯ��ٸ�
        if (Hitbox)
        {
            Hitbox->Host = this;
            Hitbox->IsBrocken = true;
            Hitbox->dmg = 10;
            Hitbox->lifeTime = 1;
            Hitbox->team = team;
            Hitbox->boxComp->SetCollisionProfileName(TEXT("HitBox"));
        }
		/*    ct = 0;
		}*/
}

void AH_EnemyCharacter::SAttackMongtage()
{

   

    AnimInstance->PlaySAttackMontage();

}

void AH_EnemyCharacter::FirstBasicAttack()
{
 

    AnimInstance->PlayBasicAttackMongtage();

}

void AH_EnemyCharacter::SecondBasicAttack()
{
   

    AnimInstance->PlayBasicAttackOneMongtage();

}

void AH_EnemyCharacter::ThirdBasicAttack()
{
    

    AnimInstance->PlayBasicAttackTwoMongtage();

}

void AH_EnemyCharacter::RightMoveAnim()
{
   
    AnimInstance->PlayMovingRightMongtage();

}

void AH_EnemyCharacter::LeftMoveAnim()
{
    
    AnimInstance->PlayMovingLeftMongtage();

}

void AH_EnemyCharacter::ForwardMoveAnim()
{
    
  

    AnimInstance->PlayMovingForwardMontage();

}

void AH_EnemyCharacter::BackwardMoveAnim()
{
   
    AnimInstance->PlayMovingBackwardMontage();

}

void AH_EnemyCharacter::IdleAnim()
{
   
    AnimInstance->PlayMovingIdleMongtage();

}


void AH_EnemyCharacter::Hit(AC_HitBox* box, float value) {

    hp -= value;
    if (hp <= 0) {

        MoveState = EBossMovingState::Dying;
        //CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    }

}
