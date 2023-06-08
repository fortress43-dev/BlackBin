// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "HitBox.h" // HitBox 액터 헤더 파일을 포함해야 합니다.
#include <cstdlib>

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
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
void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    


}
// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
   
    

    dt = DeltaTime;

    // Find the player character in the world
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    if (PlayerCharacter)
    {
        FVector enemyLocation = GetActorLocation();
        FVector playerLocation = PlayerCharacter->GetActorLocation();
        distance = FVector::Distance(enemyLocation, playerLocation);

        PlayerLoc = playerLocation;
        EnemyLoc = enemyLocation;

        /*if (distance > 400.0f)
        {
            // Dash towards the player character
            dir = playerLocation - enemyLocation;
            dir.Normalize();
            FVector newLocation = GetActorLocation() + dir * dashSpeed * dt;
            SetActorLocation(newLocation);
        }
        else if (distance > 50.0f)
        {
            // Move slowly towards the player character
            dir = playerLocation - enemyLocation;
            dir.Normalize();
            FVector newLocation = GetActorLocation() + dir * moveSpeed * dt;
            SetActorLocation(newLocation);
        }
        else
        {
            // Stay idle
            dir = FVector::ZeroVector;
        }
        */
    }

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
    default:
        break;
    }
}


void AEnemyCharacter::DEFAULTState()
{
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    FVector newLocation = GetActorLocation() + dir * moveSpeed * dt;
    SetActorLocation(newLocation);

    if (distance < bossIsClose) {
        bState = EBossState::ATTACK;
    }
    else if(distance > bossIsFar){
        bState = EBossState::DODASH;
    }
}

void AEnemyCharacter::ATTACKState()
{
    // Stay idle, do nothing
    dir = FVector::ZeroVector;

    //기본공격을 함
    
   
    //거리가 300 이상이되면 DEFAULT 이동을 한다
    if(distance > bossIsClose)
        bState = EBossState::DEFAULT;
}       

void AEnemyCharacter::DODASHState()
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

void AEnemyCharacter::DASHINGState()
{
    

    // Move towards the player character
    dir = PlayerLoc - EnemyLoc;
    dir.Normalize();
    FVector newLocation = GetActorLocation() + dir * dashSpeed * dt;
    SetActorLocation(newLocation);

    if (distance < 200) {
        bState = EBossState::ATTACK;
    }
}

