// Fill out your copyright notice in the Description page of Project Settings.

#include "C_Player.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "C_Barrier.h"
#include "C_HitBox.h"
#include "C_Arrow.h"
#include "D_RotManager.h"
#include "DebugMessages.h"
// Sets default values
AC_Player::AC_Player()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	boxComp->SetupAttachment(GetCapsuleComponent());

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	//boxComp->SetupAttachment(GetCapsuleComponent());
	boxComp->SetCollisionProfileName(TEXT("Mob"));
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	static ConstructorHelpers::FObjectFinder<UBlueprint> BarrierObject(TEXT("/Script/Engine.Blueprint'/Game/CSK/Blueprints/BP_Barrier.BP_Barrier'"));
	if (BarrierObject.Object)
	{
		BarrierClass = (UClass*)BarrierObject.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> HitBoxObject(TEXT("/Script/Engine.Blueprint'/Game/CSK/Blueprints/BP_HitBox.BP_HitBox'"));
	if (HitBoxObject.Object)
	{
		HitBoxClass = (UClass*)HitBoxObject.Object->GeneratedClass;
	}
	\
	static ConstructorHelpers::FObjectFinder<UBlueprint> ArrowObject(TEXT("/Script/Engine.Blueprint'/Game/CSK/Blueprints/BP_Arrow.BP_Arrow'"));
	if (ArrowObject.Object)
	{
		ArrowClass = (UClass*)ArrowObject.Object->GeneratedClass;
	}
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 2000.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 200;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 500.f;
	GetCharacterMovement()->BrakingFriction = .05f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	JumpMaxCount = 2;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AC_Player::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	RotManager = Cast<AD_RotManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AD_RotManager::StaticClass()));
}
// Called every frame
void AC_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State != PLAYERSTATE::MOVEMENT)
	{
		switch (State)
		{
			case PLAYERSTATE::ROLL :
				StateRoll();
			break;
			case PLAYERSTATE::ATTACK :
				StateAttack();
			break;
			case PLAYERSTATE::POWERATTACK:
				StatePowerAttack();
			break;
			case PLAYERSTATE::ARROW:
				StateArrow();
			break;
			case PLAYERSTATE::POWERCHARGING:
				StatePowerCharging();
			break;
			case PLAYERSTATE::BARRIER:
				StateBarrier();
			break;
		}
		if (Statestep == 100)
		{
			StateReset();
		}
		AddMovementInput(StateDirectionX, StateVector.Y);
		AddMovementInput(StateDirectionY, StateVector.X);
	}
}
//////////////////////////////////////////////////////////////////////////
// Input

void AC_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AC_Player::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AC_Player::Look);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AC_Player::RunStart);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AC_Player::RunEnd);

		EnhancedInputComponent->BindAction(BarrierAction, ETriggerEvent::Triggered, this, &AC_Player::BarrierStart);
		EnhancedInputComponent->BindAction(BarrierAction, ETriggerEvent::Completed, this, &AC_Player::BarrierEnd);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AC_Player::Attack);

		EnhancedInputComponent->BindAction(PowerAttackAction, ETriggerEvent::Triggered, this, &AC_Player::PowerAttackStart);
		EnhancedInputComponent->BindAction(PowerAttackAction, ETriggerEvent::Completed, this, &AC_Player::PowerAttackEnd);

		EnhancedInputComponent->BindAction(ArrowAction, ETriggerEvent::Triggered, this, &AC_Player::ArrowStart);
		EnhancedInputComponent->BindAction(ArrowAction, ETriggerEvent::Completed, this, &AC_Player::ArrowEnd);

		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AC_Player::Roll);

		EnhancedInputComponent->BindAction(RotAction, ETriggerEvent::Triggered, this, &AC_Player::RotActionStart);
		EnhancedInputComponent->BindAction(InterAction, ETriggerEvent::Triggered, this, &AC_Player::InterActionStart);
	}

}
void AC_Player::Jump()
{
	if(State == PLAYERSTATE::MOVEMENT)
			Super::Jump();
}
void AC_Player::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && State == PLAYERSTATE::MOVEMENT)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);



		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		StateDirectionX = ForwardDirection;
		StateDirectionY = RightDirection;
		StateVector		= MovementVector;
	}
}

void AC_Player::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AC_Player::RunStart()
{
	if (Controller != nullptr)
	{
		if (State == PLAYERSTATE::MOVEMENT)
		{
			GetCharacterMovement()->MaxWalkSpeed = 500;
		}
	}
}

void AC_Player::RunEnd()
{
	if (Controller != nullptr)
	{
		if (State == PLAYERSTATE::MOVEMENT)
		{
			GetCharacterMovement()->MaxWalkSpeed = 200;
		}
	}
}

void AC_Player::Attack()
{
	if (Controller != nullptr)
	{
		if (State == PLAYERSTATE::MOVEMENT)
		{
			//IsAttack = true;
			//FMotionWarpingTarget Target = {};
			//Target.Name = FName("Target");
			//Target.Location = TargetActor->GetActorLocation();
			//Target.Rotation = TargetActor->GetActorRotation();

			//MotionWarpComponent->AddOrUpdateWarpTarget(Target);
			// find out which way is forward
			//const FRotator Rotation = Controller->GetControlRotation();
			//const FRotator YawRotation(0, Rotation.Yaw, 0);

			//// get forward vector
			//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			//// get right vector 
			//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			Statestep = 0;
			State = PLAYERSTATE::ATTACK;

			FVector2D MovementVector = FVector2D(Controller->GetControlRotation().Vector());

			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);



			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);

			StateDirectionX = ForwardDirection;
			StateDirectionY = RightDirection;
			StateVector = FVector2D(FVector::RightVector);
		}
		else if (State == PLAYERSTATE::ARROW)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FRotator rotator = Controller->GetControlRotation();
			FVector  SpawnLocation = GetActorLocation();
			FVector	 addLoc = GetActorRightVector() * 100;
			SpawnLocation.Z -= 50.f;
			AC_Arrow* Arrow = GetWorld()->SpawnActor<AC_Arrow>(ArrowClass, SpawnLocation + addLoc, rotator, SpawnParams);
			if (Arrow)
			{
				Arrow->dmg = 6;
				Arrow->lifeTime = 100;
				Arrow->boxComp->SetCollisionProfileName(TEXT("HitBox"));
			}
		}
	}
}

void AC_Player::Roll()
{
	if (Controller != nullptr && State == PLAYERSTATE::MOVEMENT)
	{
		//IsAttack = true;
		//FMotionWarpingTarget Target = {};
		//Target.Name = FName("Target");
		//Target.Location = TargetActor->GetActorLocation();
		//Target.Rotation = TargetActor->GetActorRotation();


		//MotionWarpComponent->AddOrUpdateWarpTarget(Target);
		//PlayAnimMontage(RollMontage);
		//FVector ForVector = FollowCamera->GetComponentRotation().Vector() *800;
		//StateVector = FVector2D(ForVector);
		boxComp->SetCollisionProfileName(TEXT("NoCollision"));
		State = PLAYERSTATE::ROLL;
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


		// add movement 
		AddMovementInput(ForwardDirection, StateVector.Y);
		AddMovementInput(RightDirection, StateVector.X);

		StateDirectionX = ForwardDirection;
		StateDirectionY = RightDirection;

		GetCharacterMovement()->Velocity.X = YawRotation.RotateVector(FVector(StateVector.Y, StateVector.X, 0)).X * 800;
		GetCharacterMovement()->Velocity.Y = YawRotation.RotateVector(FVector(StateVector.Y, StateVector.X, 0)).Y*800;
	}
}

void AC_Player::ArrowStart()
{
	if (Controller != nullptr && State == PLAYERSTATE::MOVEMENT)
	{
		CameraBoom->TargetArmLength = 200.0f;
		FollowCamera->SetRelativeLocation(FVector(0.f, 100.f, 0.f));
		State = PLAYERSTATE::ARROW;
		GetCharacterMovement()->Velocity.X = 0;
		GetCharacterMovement()->Velocity.Y = 0;
	}
}
void AC_Player::ArrowEnd()
{
	if (Controller != nullptr && State == PLAYERSTATE::ARROW)
	{
		CameraBoom->TargetArmLength = 400.0f;
		FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Statestep = 100;
	}
}

void AC_Player::PowerAttackStart()
{
	if (Controller != nullptr && State == PLAYERSTATE::MOVEMENT)
	{
		State = PLAYERSTATE::POWERCHARGING;
		Statestep = 0;
		StateTimer = 0;
		GetCharacterMovement()->Velocity.X = 0;
		GetCharacterMovement()->Velocity.Y = 0;
	}
}
void AC_Player::PowerAttackEnd()
{
	if (Controller != nullptr && State == PLAYERSTATE::POWERCHARGING)
	{
		State = PLAYERSTATE::POWERATTACK;
		StateTimer = 0;
		Statestep = 0;
	}
}

void AC_Player::BarrierStart()
{
	if (Controller != nullptr && Barrier == nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		FRotator rotator;
		FVector  SpawnLocation = GetActorLocation();
		//SpawnLocation.Z += 1050.0f;
		State = PLAYERSTATE::BARRIER;
		Statestep = 0;
		StateTimer = 0;
		GetCharacterMovement()->Velocity.X = 0;
		GetCharacterMovement()->Velocity.Y = 0;
		StateVector = FVector2D(0);
		Barrier = GetWorld()->SpawnActor<AC_Barrier>(BarrierClass, SpawnLocation, rotator, SpawnParams);
		if (Barrier)
		{
			Barrier->lifeTime	= 100;
			Barrier->Host		= this;
		}
	}
}
void AC_Player::BarrierEnd()
{
	if (Controller != nullptr)
	{
		Barrier->IsBoom = true;
		State = PLAYERSTATE::MOVEMENT;
	}
}

void AC_Player::StateAttack()
{
	switch (Statestep)
	{
		case 0 :
			if (StateTimer++ < 20)
				break;

				Statestep++;
		break;
		case 1 :
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FRotator rotator = GetActorRotation();
			FVector  SpawnLocation = GetActorLocation();
			FVector	 addLoc = GetActorForwardVector() * 100;
			SpawnLocation.Z -= 50.f;

			AC_HitBox* Hitbox = GetWorld()->SpawnActor<AC_HitBox>(HitBoxClass, SpawnLocation + addLoc, rotator, SpawnParams);
			if (Hitbox)
			{
				Hitbox->dmg = 10;
				Hitbox->lifeTime = 10;
				Hitbox->boxComp->SetCollisionProfileName(TEXT("HitBox"));
			}
			Statestep = MOB_STATEEND;
		break;
	}
}
void AC_Player::StatePowerAttack()
{
	switch (Statestep)
	{
	case 0:
		if (StateTimer++ < 20)
			break;

		Statestep++;
		break;
	case 1:
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		FRotator rotator = GetActorRotation();
		FVector  SpawnLocation = GetActorLocation();
		FVector	 addLoc = GetActorForwardVector() * 100;
		SpawnLocation.Z -= 50.f;

		AC_HitBox* Hitbox = GetWorld()->SpawnActor<AC_HitBox>(HitBoxClass, SpawnLocation + addLoc, rotator, SpawnParams);
		if (Hitbox)
		{
			Hitbox->lifeTime = 10;
			Hitbox->team = team;
			Hitbox->dmg = 8 * (1 + gagePower / 30);
		}
			Hitbox->SetActorScale3D(FVector(1 + gagePower/50));
			Hitbox->boxComp->SetCollisionProfileName(TEXT("HitBox"));
		Statestep = MOB_STATEEND;
		gagePower = 0;
		break;
	}
}
void AC_Player::StateRoll()
{
	switch (Statestep)
	{
	case 0:
		GetCharacterMovement()->MaxWalkSpeed = 500;
		if (StateTimer++ < 60)
			break;
			StateTimer = 0;
			Statestep++;
		break;
	case 1:
		GetCharacterMovement()->MaxWalkSpeed = 0;
		if (StateTimer++ < 10)
		break;
			boxComp->SetCollisionProfileName(TEXT("Mob"));
			Statestep = MOB_STATEEND;
		break;

	}
}
void AC_Player::StateBarrier()
{
	
}
void AC_Player::StateArrow()
{
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.0f, 0.0f); // ...at this rotation rate

	FVector2D MovementVector = FVector2D(Controller->GetControlRotation().Vector());

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	StateDirectionX = ForwardDirection;
	StateDirectionY = RightDirection;
	StateVector = FVector2D(FVector::RightVector);

}
void AC_Player::StatePowerCharging()
{
	switch (Statestep)
	{
	case 0:
		if (StateTimer++ < 10)
			break;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 100.0f, 0.0f); // ...at this rotation rate
		StateTimer = 0;
		Statestep++;
		break;
	}
	gagePower = FMath::Clamp(gagePower + .5, 0, 100);
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.0f, 0.0f); // ...at this rotation rate

	FVector2D MovementVector = FVector2D(Controller->GetControlRotation().Vector());

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	StateDirectionX = ForwardDirection;
	StateDirectionY = RightDirection;
	StateVector = FVector2D(FVector::RightVector);
}
void AC_Player::RotActionStart()
{
	print("RotAction Triggered");
	if (RotManager) {
		RotManager->RotActionSkill();
	}
}
void AC_Player::InterActionStart()
{
	print("InterAction Triggered");
	if (RotManager) {
		RotManager->InteractionStart();
	}
}
void AC_Player::StateReset()
{
	Statestep		= 0;
	StateTimer		= 0;
	State			= PLAYERSTATE::MOVEMENT;
	GetCharacterMovement()->MaxWalkSpeed = 200;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
}

void AC_Player::Hit(float value)
{
	if (Barrier != nullptr)
	{
		BarrierShield -= 10;
		value = FMath::Max(value - BarrierShield, 0);
	}
	if (value > 0)
	{ 
		Super::Hit(value);
	}
}