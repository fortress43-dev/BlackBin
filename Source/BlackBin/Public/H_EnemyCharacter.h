// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "C_Mob.h"
#include "GameFramework/Character.h"
#include "H_EnemyCharacter.generated.h"


UENUM(BlueprintType)
enum class EBossMovingState : uint8
{
    MovingBackward,
    MovingForward,
    MovingLeft,
    MovingRight,
    Staying,
    Attacking,
    Dash,
    BackStep,
    SAttack,

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
        float bossStanceMode = 1600.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 randomNumber;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int NumberPercentage = 30;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int randDeg;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EBossMovingState> arrayState;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> arrayWeight;


   
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = STATE)
        EBossMovingState MoveState = EBossMovingState::Dash;

    void MovingBackward();
    void MovingForward();
    void MovingLeft();
    void MovingRight();
    void Staying();
    void Attacking();
    void Dash();
    void BackStep();
    void SAttack();
    void BackMove();
    void Checking();
    void SpawnHitBox();
    void SAttackMongtage();
    void FirstBasicAttack();
    EBossMovingState GetArrayWeight(const TArray<EBossMovingState>& ArrayState, const TArray<float>& ArrayWeight);

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* meshComp;

    //나중에 백스텝 한번에 할떄 쓰기
    /*UFUNCTION()
    void AnimEnded(UAnimMontage* Montage, bool bInterrupted);

    UPROPERTY()
    class UH_AnimInst* AnimIns;

    virtual void PostInitializeComponents() override;*/

    FVector PlayerLoc;
    FVector EnemyLoc;
    FVector dir; // Movement direction
    FRotator EnemyRot;
    float dt;
    float distance;
    float playerDistance;
    float ct = 0;
    float backwardSpeed = 600;
    float ct1 = 0;
    int randomN;
    int ranTime;
    
};


