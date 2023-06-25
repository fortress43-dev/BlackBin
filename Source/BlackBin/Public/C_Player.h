// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "C_Mob.h"
#include "InputActionValue.h"
#include "C_Player.generated.h"

#define MOB_STATEEND 100;
enum class PLAYERSTATE
{
	DEFAULT,
	MOVEMENT,
	ATTACK,
	ROLL,
	KNOCKBACK,
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
		class UAnimMontage* PopMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* PowerAttackChargingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* KnockBackMontage;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AC_Barrier> BarrierClass;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AC_HitBox> HitBoxClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AC_Arrow> ArrowClass;

	UPROPERTY()
		class UC_AnimInstance* AnimIns;

	UFUNCTION()
		void OnAnimeMontageEnded(UAnimMontage* Montage, bool bInterrupted);



	virtual void PostInitializeComponents() override;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		//TObjectPtr<class UMotionWarpingComponent> MotionWarpComponent;
public:
	AC_Player();

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USplineMeshComponent* BowStringComp1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USplineMeshComponent* BowStringComp2;
	UPROPERTY()
	bool IsFocus = false;
	UPROPERTY()
	class UStaticMeshComponent* StaffComp;
	UPROPERTY()
	class UNiagaraComponent* Trail;
	UPROPERTY()
	class UNiagaraComponent* Charging;
	UPROPERTY()
	class UNiagaraComponent* ArrowCharging;
	UPROPERTY()
	class AC_Arrow* Arrow;
	PLAYERSTATE	State	= PLAYERSTATE::MOVEMENT;
	UPROPERTY()
	float	StateTimer	= 0;
	UPROPERTY()
	float	gagePower	= 0;
	UPROPERTY()
	float	gageArrow	= 0;
	UPROPERTY()
	float	BarrierShield = 100;
	UPROPERTY()
	float	zoomTarget;
	UPROPERTY()
	FRotator	RotationTarget;
	UPROPERTY()
	bool	IsRotation		= true;
	UPROPERTY()
	bool	IsCheckCombo	= false;
	UPROPERTY()
	bool	bCancelable		= false;
	UPROPERTY()
	bool	DoCombo			= false;
	UPROPERTY()
	int		attackIndex	= 0;
	UPROPERTY()
	class AC_Barrier* Barrier;
	UPROPERTY()
	FVector StateDirectionX;
	UPROPERTY()
	FVector StateDirectionY;
	UPROPERTY()
	FVector2D StateVector;
	UPROPERTY()
	FVector2D PlayerVector;

	virtual void Hit(class AC_HitBox* box, float value) override;
protected:
	
	bool IsDoState		= false;
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
	void ArrowCheck();
	void ArrowAttack();
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
	void StateKnockBack();
	TObjectPtr<class USoundBase> barriersound;
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

