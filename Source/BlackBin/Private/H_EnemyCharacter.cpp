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
    meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
    meshComp->SetupAttachment(GetCapsuleComponent());

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
    moveSpeed = 300.0f; // Adjust the value as needed
    dashSpeed = 2000.0f; // Adjust the value as needed
    


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

        PlayerLoc = playerLocation;
        EnemyLoc = enemyLocation;

        // Update the state
        switch (bState)
        {
        case EBossState::DEFAULT:
            DEFAULTState();
            break;
        case EBossState::ATTACK:
            ATTACKState();
            break;
        case EBossState::DODASH:
            DODASHState();
            break;
        case EBossState::DASHING:
            DASHINGState();
            break;
        case EBossState::MoveBack:
            MoveBackward();
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



void AH_EnemyCharacter::DEFAULTState()
{
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    FVector newLocation = GetActorLocation() + dir * moveSpeed * dt;
    SetActorLocation(newLocation);

    if (distance < bossIsClose) {
        bState = EBossState::ATTACK;
    }
    
}

void AH_EnemyCharacter::ATTACKState()
{
    // Stay idle, do nothing
    dir = FVector::ZeroVector;

    //기본공격을 함
    SpawnHitBox();

    //거리가 300 이상이되면 DEFAULT 이동을 한다
    if (distance > bossIsClose)
        bState = EBossState::DEFAULT;
}

void AH_EnemyCharacter::DODASHState()
{
    // Dash towards the player character
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    FVector newLocation = GetActorLocation() + dir * dashSpeed * dt;
    SetActorLocation(newLocation);

    // Transition to DASHING state
    if (FVector::Distance(GetActorLocation(), PlayerLoc) <= bossIsClose)
    {
        bState = EBossState::DASHING;
    }
}

void AH_EnemyCharacter::DASHINGState()
{


    // 빠른 속도로 플레이어를 향해 온다
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    FVector newLocation = GetActorLocation() + dir * dashSpeed * dt;
    SetActorLocation(newLocation);

    if (distance < 200) {
        bState = EBossState::ATTACK;
    }
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


void AH_EnemyCharacter::MoveBackward()
{
    // 뒤로 빠르게 이동하는 로직 구현
    // 1. 방향구하기
    FVector backwardDirection = dir * -1.0f;
    // 2. 이동하기
    FVector newLocation = GetActorLocation() + backwardDirection * backwardSpeed * dt;
    SetActorLocation(newLocation);
    // 3. 만약 뒤로 300 이동했다면-> 뒤로이동하기 시작한 위치에서 부터 300 떨어졌다면
    if (distance > 600) {
        // -> 상태를 Default 로 전화하고 싶다.
        bState = EBossState::DEFAULT;
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

    if (FMath::RandRange(1, 100) < 100 && bState != EBossState::MoveBack) {
         //1 ~ 100 random number
       
         // 뒤로 300 정도 이동하는 상태로 전환하고 싶다.
         //GetActorLocation() + dir * dashSpeed * dt * -1;
         bState = EBossState::MoveBack;

    }
}
