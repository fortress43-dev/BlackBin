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
    BasicOneAttack,
    BasicTwoAttack,
    Dash,
    BackStep,
    SAttack,
    Idle,
    Dying,
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

    class AC_HitBox* Hitbox;

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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    USkeletalMeshComponent* MeshComponent;

   
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = STATE)
        EBossMovingState MoveState = EBossMovingState::Idle;

    void Dying();
    void Idle();
    void MovingBackward();
    void MovingForward();
    void MovingLeft();
    void MovingRight();
    void Staying();
    void Attacking();
    void Dash();
    void BackStep();
    void SAttack();
    void BasicOneAttack();
    void BasicTwoAttack();
    void BackMove();
    void Checking();
    void SpawnHitBox();
    void SAttackMongtage();
    void FirstBasicAttack();
    void SecondBasicAttack();
    void ThirdBasicAttack();
    void RightMoveAnim();
    void LeftMoveAnim();
    void ForwardMoveAnim();
    void BackwardMoveAnim();
    void IdleAnim();
    void CheckJump();
    void CheckSAttack();
    void DyingMotion();
    //void ChangeState(EBossMovingState NewState);
    //void TimerEvent();
    //UAnimInstance* GetAnimationInstance();
   // UAnimInstance* GetAnimationMontage(EBossMovingState State);
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
    FVector Direc;
    float dt;
    float distance;
    float playerDistance;
    float ct = 0;
    float ct1 = 0;
    float backwardSpeed = 600;
    float ct2 = 0;
    float ct3 = 0;
    int randomN;
    int ranTime;
    float StateTimer;
    bool animFinished = false;
    FTimerHandle TimerHandle;
    UAnimMontage* CurrentMontage;
    bool isJumping = false;

    bool isAttackingOne = false;
    bool isAttackingTwo = false;
    bool isAttackingThree = false;
    bool isAttackingStrong = false;

};


