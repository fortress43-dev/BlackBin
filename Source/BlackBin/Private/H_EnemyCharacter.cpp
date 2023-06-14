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


}

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
        case EBossMovingState::MovingSide:
            MovingSide();
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
    FVector newLocation = GetActorLocation() + dir * moveSpeed * dt * -1;
    SetActorLocation(newLocation);
}

void AH_EnemyCharacter::MovingForward()
{
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    FVector newLocation = GetActorLocation() + dir * moveSpeed * dt;
    SetActorLocation(newLocation);
}

void AH_EnemyCharacter::MovingSide()
{

    if (randomN > 50) {
        // ���������� �̵��ϴ� ���� ����
        FVector NewLocation = GetActorLocation() + FVector(1.0f, 0.0f, 0.0f) * moveSpeed * dt;
            SetActorLocation(NewLocation); // ������ ��ġ�� ���ο� ��ġ�� ����
    }
    else {
        //�������� �̵��ϸ�
        FVector NewLocation = GetActorLocation() + FVector(-1.0f, 0.0f, 0.0f) * moveSpeed * dt;
        SetActorLocation(NewLocation); // ������ ��ġ�� ���ο� ��ġ�� ����
    }
    printf("Rotate");
    /*randDeg = FMath::RandRange(-90, 90);
    //���ʹ� ��ġ�� �÷��̾� ��ġ���� �������� �Ÿ���ŭ ������ �ʹ�?
    FRotator Rot = GetCharacterMovement()->GetLastUpdateRotation();
    Rot.Pitch = 0;
    Rot.Roll = 0;
    Rot.Yaw -= randDeg;*/
    
    
}

void AH_EnemyCharacter::Staying()
{
    dir = FVector::ZeroVector;
}

void AH_EnemyCharacter::Attacking()
{
    dir = FVector::ZeroVector;

    //�⺻������ ��
    SpawnHitBox();
}

void AH_EnemyCharacter::Dash()
{
    if (MoveState == EBossMovingState::Dash && distance > 200) {
        // Dash towards the player character
        dir = PlayerLoc - EnemyLoc;
        dir.Normalize();
        FVector newLocation = GetActorLocation() + dir * dashSpeed * dt;
        SetActorLocation(newLocation);
    }
    else {
        MoveState = EBossMovingState::Attacking;
    }

}

void AH_EnemyCharacter::BackStep()
{

    if (MoveState == EBossMovingState::BackStep && distance < 900) 
    {
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    // �ڷ� ������ �̵��ϴ� ���� ����
    // 1. ���ⱸ�ϱ�
    FVector backwardDirection = dir * -1.0f;
    // 2. �̵��ϱ�
    FVector newLocation = GetActorLocation() + backwardDirection * backwardSpeed * dt;
    SetActorLocation(newLocation);
    }
    // 3. ���� �ڷ� 300 �̵��ߴٸ�-> �ڷ��̵��ϱ� ������ ��ġ���� ���� 300 �������ٸ�

    /*printf("VectorX: %f", backwardDirection.X);
    printf("VectorY: %f", backwardDirection.Y);
    printf("VectorZ: %f", backwardDirection.Z);
    printf("Speed %f", backwardSpeed);*/
}

void AH_EnemyCharacter::Checking()
{
	if (ct >  2){
        //���� �Ÿ��� 100���� �ִٸ�
		if (distance > 1000) {
            //������ ������Ʈ �߿���
            arrayState = { EBossMovingState::Dash, EBossMovingState::MovingForward, EBossMovingState::MovingSide };
            arrayWeight = { 0.4f, 0.2f, 0.2f };
            //����ġ�� ����ؼ� �ϳ��� �̾ƶ�
			MoveState = GetArrayWeight(arrayState, arrayWeight);
		}
        else if(distance < 1000 && distance > 300){

            arrayState = { EBossMovingState::MovingBackward, EBossMovingState::MovingForward, EBossMovingState::MovingSide};
            arrayWeight = { 0.4f, 0.2f, 0.4f };

            MoveState = GetArrayWeight(arrayState, arrayWeight);

        }
        //
        else {
            arrayState = { EBossMovingState::MovingBackward, EBossMovingState::Attacking, EBossMovingState::BackStep};
            arrayWeight = { 0.1f, 0.5f, 0.4f };

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
    UE_LOG(LogTemp, Warning, TEXT("Selected Start!"));

    for (int32 Index = 0; Index < ArrayWeight.Num(); ++Index)
    {
        AccumulatedWeight += ArrayWeight[Index];
        if (RandomValue <= AccumulatedWeight)
        {
            UE_LOG(LogTemp, Warning, TEXT("Selected Index : %d"), Index);
            return ArrayState[Index];

        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Not Selected Index..."));
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

    if (ct > 3) {
        AC_HitBox* Hitbox = GetWorld()->SpawnActor<AC_HitBox>(HitBoxClass, SpawnLocation + addLoc, rotator, SpawnParams);
        if (Hitbox)
        {
            Hitbox->dmg = 10;
            Hitbox->lifeTime = 10;
            Hitbox->team = team;
            Hitbox->boxComp->SetCollisionProfileName(TEXT("HitBox"));
        }
        ct = 0;
    }
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
         // �ڷ� 300 ���� �̵��ϴ� ���·� ��ȯ�ϰ� �ʹ�.
         //GetActorLocation() + dir * dashSpeed * dt * -1;
         

    }
    else printf("RanN : %d", randomN);
}
