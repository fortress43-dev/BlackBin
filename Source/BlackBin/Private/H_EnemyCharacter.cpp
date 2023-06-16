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

    
    // 적과 나의 방향과 거리를 알고싶다.
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
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // 또는 이동 모드에 맞는 다른 모드를 선택합니다.
    GetCharacterMovement()->Velocity = dir * moveSpeed;
    SetActorLocation(newLocation);
    
}

void AH_EnemyCharacter::MovingForward()
{
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    FVector newLocation = GetActorLocation() + dir * moveSpeed * dt;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // 또는 이동 모드에 맞는 다른 모드를 선택합니다.
    GetCharacterMovement()->Velocity = dir * moveSpeed;
    SetActorLocation(newLocation);
    
}

void AH_EnemyCharacter::MovingRight()
{
// 오른쪽으로 이동하는 로직 구현
        FVector NewLocation = GetActorLocation() + FVector(1.0f, 0.0f, 0.0f) * moveSpeed * dt;
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // 또는 이동 모드에 맞는 다른 모드를 선택합니다.
        GetCharacterMovement()->Velocity = dir * moveSpeed;
        SetActorLocation(NewLocation); // 보스의 위치를 새로운 위치로 설정
}
  

void AH_EnemyCharacter::MovingLeft()
{
    FVector NewLocation = GetActorLocation() + FVector(-1.0f, 0.0f, 0.0f) * moveSpeed * dt;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // 또는 이동 모드에 맞는 다른 모드를 선택합니다.
    GetCharacterMovement()->Velocity = dir * moveSpeed;
    SetActorLocation(NewLocation); // 보스의 위치를 새로운 위치로 설정
}

    
    /*randDeg = FMath::RandRange(-90, 90);
    //에너미 위치랑 플레이어 위치에서 반지름의 거리만큼 돌리고 싶다?
    FRotator Rot = GetCharacterMovement()->GetLastUpdateRotation();
    Rot.Pitch = 0;
    Rot.Roll = 0;
    Rot.Yaw -= randDeg;*/
    

void AH_EnemyCharacter::Staying()
{
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->Velocity = FVector::ZeroVector;
}

void AH_EnemyCharacter::Attacking()
{
    if (distance > 300) {
        GetCharacterMovement()->StopMovementImmediately();
        GetCharacterMovement()->Velocity = FVector::ZeroVector;
        SpawnHitBox();
    }
    else {
        Checking();
    }
}

void AH_EnemyCharacter::Dash()
{
    if (MoveState == EBossMovingState::Dash && distance > 200) {
        // Dash towards the player character
        dir = PlayerLoc - EnemyLoc;
        dir.Normalize();
        FVector newLocation = GetActorLocation() + dir * dashSpeed * dt;
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // 또는 이동 모드에 맞는 다른 모드를 선택합니다.
        GetCharacterMovement()->Velocity = dir * dashSpeed;
        SetActorLocation(newLocation);
        
    }
    else {
        Checking();
    }

}

void AH_EnemyCharacter::BackStep()
{

    if (MoveState == EBossMovingState::BackStep && distance < 900) 
    {
        dir = PlayerLoc - EnemyLoc;
        dir.Normalize();
        FVector newLocation = GetActorLocation() + dir * dashSpeed * dt * -1;
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // 또는 이동 모드에 맞는 다른 모드를 선택합니다.
        GetCharacterMovement()->Velocity = dir * dashSpeed;
        SetActorLocation(newLocation);
    }
    // 3. 만약 뒤로 300 이동했다면-> 뒤로이동하기 시작한 위치에서 부터 300 떨어졌다면

    /*printf("VectorX: %f", backwardDirection.X);
    printf("VectorY: %f", backwardDirection.Y);
    printf("VectorZ: %f", backwardDirection.Z);
    printf("Speed %f", backwardSpeed);*/
}

void AH_EnemyCharacter::Checking()
{
    ranTime = FMath::RandRange(2, 5);


	if (ct >  ranTime){
        //만약 거리가 100보다 멀다면
		if (distance > 900) {
            //세개의 스테이트 중에서
            arrayState = { EBossMovingState::Dash, EBossMovingState::MovingForward, EBossMovingState::MovingRight,EBossMovingState::MovingLeft, EBossMovingState::Staying };
            arrayWeight = { 0.9f, 0.2f, 0.2f, 0.2f, 0.2f};
            //가중치를 계산해서 하나를 뽑아라
			MoveState = GetArrayWeight(arrayState, arrayWeight);
		}
        else if(distance < 1000 && distance > 300){

            arrayState = { EBossMovingState::MovingBackward, EBossMovingState::MovingForward, EBossMovingState::MovingLeft,EBossMovingState::MovingRight, EBossMovingState::Staying };
            arrayWeight = { 0.2f, 0.6f, 0.6f, 0.6f, 0.4f};

            MoveState = GetArrayWeight(arrayState, arrayWeight);

        }
        //
        else {
            arrayState = { EBossMovingState::MovingBackward, EBossMovingState::Attacking, EBossMovingState::BackStep, EBossMovingState::Staying };
            arrayWeight = { 0.4f, 0.8f, 0.1f, 0.3f};

            MoveState = GetArrayWeight(arrayState, arrayWeight);
        }
        printf("%d", MoveState);
        ct = 0;
	}
}

EBossMovingState AH_EnemyCharacter::GetArrayWeight(const TArray<EBossMovingState>& ArrayState, const TArray<float>& ArrayWeight)
{
    // 가중치 배열의 합 구하기
    float TotalWeight = 0.0f;
    for (float Weight : ArrayWeight)
    {
        TotalWeight += Weight;
    }

    // 0부터 합까지의 구간을 생성하여 랜덤 값을 선택
    float RandomValue = FMath::FRandRange(0.0f, TotalWeight);

    // 가중치에 따라 요소 선택
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


//히트박스를 3초동안 내 위치 앞에 스폰 하고싶다
    //1. 히트박스 스폰오기
void AH_EnemyCharacter::SpawnHitBox()
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;


    FRotator rotator = GetActorRotation();

    FVector SpawnLocation = GetActorLocation();
    FVector addLoc = GetActorForwardVector() * 100;
    SpawnLocation.Z -= 50.f;

    //2. 만약 커렌트 타임이 3을 넘어가면
    
    if (ct > 2) {
        //히트박스를 소환한다

        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AC_HitBox* Hitbox = GetWorld()->SpawnActor<AC_HitBox>(HitBoxClass, SpawnLocation + addLoc, rotator, SpawnParams);
        //만약 히트박스가 소환됬다면
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

//조건을 피격시 50퍼센트로 바꿀거임
    // 뒤로 이동하는 상태가 아니고, 거리가 400 이하면 
    /*if (bState != EBossState::MoveBack && distance < 400)
    {
        // 뒤로 300 정도 이동하는 상태로 전환하고 싶다.
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
