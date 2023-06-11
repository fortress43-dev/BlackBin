// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "C_Mob.h"
#include "InputActionValue.h"
#include "C_Barrier.h"
#include "C_HitBox.h"
#include "C_Player.generated.h"

#define MOB_STATEEND 100;
enum class PLAYERSTATE
{
	MOVEMENT,
	ATTACK,
	ROLL,
	BARRIER,
	ARROW,
	POWERATTACK,
	POWERCHARGING
};

UCLASS()
class BLACKBIN_API AC_Player : public AC_Mob
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* PowerAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* BarrierAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ArrowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* PowerAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* PowerAttackChargingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* RollMontage;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AC_Barrier> BarrierClass = AC_Barrier::StaticClass();

	UPROPERTY(EditAnywhere)
		TSubclassOf<AC_HitBox> HitBoxClass = AC_HitBox::StaticClass();

	UPROPERTY(EditAnywhere)
		TSubclassOf<AC_Arrow> ArrowClass = AC_Arrow::StaticClass();
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		//TObjectPtr<class UMotionWarpingComponent> MotionWarpComponent;
public:
	AC_Player();
	PLAYERSTATE	State	= PLAYERSTATE::MOVEMENT;
	float	StateTimer	= 0;
	float	gagePower	= 0;
	float	BarrierShield = 100;
	AC_Barrier* Barrier;
	FVector StateDirectionX;
	FVector StateDirectionY;
	FVector2D StateVector;
	virtual void Hit(float value) override;
protected:
	int Statestep		= 0;
	virtual void Tick(float DeltaTime) override;
	/** Called for movement input */
	virtual void Jump() override;

	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void RunStart();
	void RunEnd();

	void BarrierStart();
	void BarrierEnd();

	void Attack();
	void PowerAttackStart();
	void PowerAttackEnd();
	void ArrowStart();
	void ArrowEnd();
	void Roll();

	void StateReset();
	void StateArrow();
	void StateAttack();
	void StatePowerAttack();
	void StateBarrier();
	void StateRoll();
	void StatePowerCharging();
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

