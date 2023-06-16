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
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

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
    
}

// Called every frame
void AH_EnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    dt = DeltaTime;
    ct += DeltaTime;
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

        Checking();
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
        case EBossMovingState::BackStep:
            BackStep();
            break;
        case EBossMovingState::SAttack:
            SAttack();
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



void AH_EnemyCharacter::MovingBackward()
{
    
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �Ǵ� �̵� ��忡 �´� �ٸ� ��带 �����մϴ�.
    AddMovementInput(dir * -1);
    GetCharacterMovement()->MaxWalkSpeed = 150;
    SetActorRotation(EnemyRot);
}

void AH_EnemyCharacter::MovingForward()
{
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �Ǵ� �̵� ��忡 �´� �ٸ� ��带 �����մϴ�.
    AddMovementInput(dir);
    GetCharacterMovement()->MaxWalkSpeed = 150;
    SetActorRotation(EnemyRot);
}

void AH_EnemyCharacter::MovingRight()
{
// ���������� �̵��ϴ� ���� ����
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    SetActorRotation(EnemyRot);
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �Ǵ� �̵� ��忡 �´� �ٸ� ��带 �����մϴ�.
    AddMovementInput(dir * FVector::RightVector);
    GetCharacterMovement()->MaxWalkSpeed = 150;
}
  

void AH_EnemyCharacter::MovingLeft()
{
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    SetActorRotation(EnemyRot);
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �Ǵ� �̵� ��忡 �´� �ٸ� ��带 �����մϴ�.
    AddMovementInput(dir * FVector::LeftVector);
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
    GetCharacterMovement()->MaxWalkSpeed = 0;
}

void AH_EnemyCharacter::Attacking()
{
    if (distance < 300) {
        GetCharacterMovement()->MaxWalkSpeed = 0;
        SpawnHitBox();
    }
    else {
        Checking();
    }
}

// �뽬�����϶� �뽬�ӵ��� ��� �̵��Ѵ�.
// ��, �����Ÿ� �̻��϶� �뽬��带 �����Ѵ�.
// ���� �����Ÿ� �̸��̸� �뽬��带 ������ �ʹ�.


void AH_EnemyCharacter::Dash()
{
    if (MoveState == EBossMovingState::Dash && distance > 200) {
        // Dash towards the player character

        auto AnimInstance = Cast<UH_AnimInst>(GetMesh()->GetAnimInstance());
        if (nullptr == AnimInstance) return;

        AnimInstance->PlayRunMontage();

        dir = PlayerLoc - EnemyLoc;
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �Ǵ� �̵� ��忡 �´� �ٸ� ��带 �����մϴ�.
        GetCharacterMovement()->MaxWalkSpeed = 900;
        AddMovementInput(dir);
        
        if (distance < 300) {
            MoveState = EBossMovingState::SAttack;

        }
        
    }
    

}

void AH_EnemyCharacter::BackStep()
{

    if (MoveState == EBossMovingState::BackStep && distance < 900) 
    {

        BackMove();
        dir = PlayerLoc - EnemyLoc;
        dir.Normalize();
        SetActorRotation(EnemyRot);
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �Ǵ� �̵� ��忡 �´� �ٸ� ��带 �����մϴ�.
        AddMovementInput(dir * -1);
        GetCharacterMovement()->MaxWalkSpeed = 700;
        
        
    }
    // 3. ���� �ڷ� 300 �̵��ߴٸ�-> �ڷ��̵��ϱ� ������ ��ġ���� ���� 300 �������ٸ�

 
}

void AH_EnemyCharacter::SAttack()
{
	SAttackMongtage();
    SpawnHitBox();
}

void AH_EnemyCharacter::BackMove()
{
    auto AnimInstance = Cast<UH_AnimInst>(GetMesh()->GetAnimInstance());
    if (nullptr == AnimInstance) return;

    AnimInstance->PlayBackmoveMontage();
}

void AH_EnemyCharacter::Checking()
{
    ranTime = FMath::RandRange(2, 5);


	if (ct >  ranTime){
        //���� �Ÿ��� 100���� �ִٸ�
		if (distance > 900) {
            //������ ������Ʈ �߿���
            arrayState = { EBossMovingState::Dash, EBossMovingState::MovingForward, EBossMovingState::MovingRight,EBossMovingState::MovingLeft, EBossMovingState::Staying };
            arrayWeight = { 0.9f, 0.2f, 0.2f, 0.2f, 0.2f};
            //����ġ�� ����ؼ� �ϳ��� �̾ƶ�
			MoveState = GetArrayWeight(arrayState, arrayWeight);
		}
        else if(distance < 1000 && distance > 200){

            arrayState = { EBossMovingState::MovingBackward, EBossMovingState::MovingForward, EBossMovingState::MovingLeft,EBossMovingState::MovingRight, EBossMovingState::Staying };
            arrayWeight = { 0.2f, 0.6f, 0.6f, 0.6f, 0.4f};

            MoveState = GetArrayWeight(arrayState, arrayWeight);

        }
        
        else {
            arrayState = { EBossMovingState::MovingBackward, EBossMovingState::Attacking, EBossMovingState::BackStep, EBossMovingState::Staying };
            arrayWeight = { 0.4f, 0.8f, 0.3f, 0.3f};

            MoveState = GetArrayWeight(arrayState, arrayWeight);
        }
        printf("%d", MoveState);
        ct = 0;
	}
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


//��Ʈ�ڽ��� 3�ʵ��� �� ��ġ �տ� ���� �ϰ�ʹ�
    //1. ��Ʈ�ڽ� ��������
void AH_EnemyCharacter::SpawnHitBox()
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;


    FRotator rotator = GetActorRotation();

    FVector SpawnLocation = GetActorLocation();
    FVector addLoc = GetActorForwardVector() * 100;
    SpawnLocation.Z -= 50.f;

    //2. ���� Ŀ��Ʈ Ÿ���� 3�� �Ѿ��
    
    if (ct > 2) {
        //��Ʈ�ڽ��� ��ȯ�Ѵ�

        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AC_HitBox* Hitbox = GetWorld()->SpawnActor<AC_HitBox>(HitBoxClass, SpawnLocation + addLoc, rotator, SpawnParams);
        FirstBasicAttack();
        //���� ��Ʈ�ڽ��� ��ȯ��ٸ�
        if (Hitbox)
        {
            printf("Attack");
            Hitbox->dmg = 10;
            Hitbox->lifeTime = 10;
            Hitbox->team = team;
            Hitbox->boxComp->SetCollisionProfileName(TEXT("HitBox"));
        }
        ct = 0;
    }
}

void AH_EnemyCharacter::SAttackMongtage()
{
    auto AnimInstance = Cast<UH_AnimInst>(GetMesh()->GetAnimInstance());
    if (nullptr == AnimInstance) return;

    AnimInstance->PlaySAttackMontage();
}

void AH_EnemyCharacter::FirstBasicAttack()
{
    auto AnimInstance = Cast<UH_AnimInst>(GetMesh()->GetAnimInstance());
    if (nullptr == AnimInstance) return;

    AnimInstance->PlayBasicAttackMongtage();
}

//������ �ǰݽ� 50�ۼ�Ʈ�� �ٲܰ���
    // �ڷ� �̵��ϴ� ���°� �ƴϰ�, �Ÿ��� 400 ���ϸ� 
    /*if (bState != EBossState::MoveBack && distance < 400)
    {
        // �ڷ� 300 ���� �̵��ϴ� ���·� ��ȯ�ϰ� �ʹ�.
        //GetActorLocation() + dir * dashSpeed * dt * -1;
        bState = EBossState::MoveBack;
    }*/

void AH_EnemyCharacter::Hit(float value) {
    Super::Hit(value);
    if (randomN < NumberPercentage) {
         //1 ~ 100 random number
        printf("RanN : %d",randomN);
     
    }
    else printf("RanN : %d", randomN);
}

//void AH_EnemyCharacter::AnimEnded(UAnimMontage* Montage, bool bInterrupted)
//{
//
//}