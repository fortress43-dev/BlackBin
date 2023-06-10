// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "D_RotManager.generated.h"

UCLASS()
class BLACKBIN_API AD_RotManager : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AD_RotManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region /** Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
		class UInputMappingContext* InputMapping;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* RotAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* Interaction;
#pragma endregion

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void RotActionSkill();
	void InteractionStart();

};
