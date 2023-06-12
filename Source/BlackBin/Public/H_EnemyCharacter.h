// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "C_Mob.h"
#include "GameFramework/Character.h"
#include "H_EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
    IDLE,
    DEFAULT,
    ATTACK,
    DASHING,
    MoveBack,
};

UCLASS()
class BLACKBIN_API AH_EnemyCharacter : public AC_Mob
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AH_EnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void Hit(float value) override;
public:
    class ASproutBossPCharacter* player;

    UPROPERTY(EditAnywhere)
        TSubclassOf<AC_HitBox> HitBoxClass = AC_HitBox::StaticClass();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float bossIsFar = 800;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float bossIsClose = 200;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float moveSpeed; // Movement speed

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float dashSpeed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float bossStanceMode = 1500;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 randomNumber;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int NumberPercentage = 30;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
        EBossState bState = EBossState::IDLE;


    void MoveBackward();

    void IDLEState();
    
    //기본 이동상태
    void DEFAULTState();
    //기본 공격 상태
    void ATTACKState();
    //대쉬중
    void DASHINGState();

    void SpawnHitBox();

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
        class UStaticMeshComponent* meshComp;

    FVector PlayerLoc;
    FVector EnemyLoc;
    FVector dir; // Movement direction
    float dt;
    float distance;
    float playerDistance;
    float ct = 0;
    float backwardSpeed = 800;
    bool IsBackStep;
    float ct1 = 0;
};


