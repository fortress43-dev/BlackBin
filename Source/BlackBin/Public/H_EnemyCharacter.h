// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "H_EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
	DEFAULT,
	ATTACK,
	DODASH,
	DASHING

};

UCLASS()
class BLACKBIN_API AH_EnemyCharacter : public ACharacter
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

public:
    class ASproutBossPCharacter* player;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float bossIsFar = 800;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float bossIsClose = 200;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float moveSpeed; // Movement speed

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float dashSpeed;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
        EBossState bState = EBossState::DEFAULT;


    //기본 이동상태
    void DEFAULTState();
    //대쉬 상태
    void DODASHState();
    //기본 공격 상태
    void ATTACKState();
    //대쉬중
    void DASHINGState();


private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
        class UStaticMeshComponent* meshComp;

    FVector PlayerLoc;
    FVector EnemyLoc;
    FVector dir; // Movement direction
    float dt;
    float distance;
};
