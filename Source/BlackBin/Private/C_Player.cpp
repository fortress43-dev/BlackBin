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
#include "C_AnimInstance.h"
#include "C_Barrier.h"
#include "C_HitBox.h"
#include "C_Arrow.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AC_Player::AC_Player()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	boxComp->SetupAttachment(GetCapsuleComponent());
	boxComp->SetCollisionProfileName(TEXT("Mob"));
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	barriersound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/CSK/Sound/Snd_BarrierS.Snd_BarrierS'"));
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
	zoomTarget = CameraBoom->TargetArmLength;
}

void AC_Player::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BarrierClass = AC_Barrier::StaticClass();
	HitBoxClass = AC_HitBox::StaticClass();
	ArrowClass = AC_Arrow::StaticClass();

	TArray<UActorComponent*> Components;
	GetComponents(Components);
	for (UActorComponent* Component : Components)
	{
		if (UNiagaraComponent* NiagaraComponent = Cast<UNiagaraComponent>(Component))
		{
			if (NiagaraComponent->GetName() == "Trailer")
			{
				Trail = NiagaraComponent;
				UE_LOG(LogTemp, Warning, TEXT("%s"), *NiagaraComponent->GetName());
				break;
			}
		}
	}
	if (Trail)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASDASDAS"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FALL"));
	}
	AnimIns = Cast<UC_AnimInstance>(GetMesh()->GetAnimInstance());
	AnimIns->OnMontageEnded.AddDynamic(this, &AC_Player::OnAnimeMontageEnded);

	AnimIns->OnNextAttackCheck.AddLambda([this]() -> void {
		if (IsCheckCombo == true)
		AnimIns->Montage_JumpToSection(FName(*FString::Printf(TEXT("Attack%d"), attackIndex + 1)), AttackMontage);
		IsCheckCombo = false;
		attackIndex = (attackIndex + 1) % 4;
		Statestep = 0;
		StateTimer = 0;
	});

	AnimIns->OnAttackHitCheck.AddLambda([this]() -> void {

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
			Hitbox->lifeTime = 3;
			Hitbox->slowmotion = .5;
			Hitbox->boxComp->SetCollisionProfileName(TEXT("HitBox"));
		}
	});
	AnimIns->OnPowerAttackHitCheck.AddLambda([this]() -> void {

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		FRotator rotator = GetActorRotation();
		FVector  SpawnLocation = GetActorLocation();
		FVector	 addLoc = GetActorForwardVector() * (100 + gagePower);
		SpawnLocation.Z -= 50.f;

		AC_HitBox* Hitbox = GetWorld()->SpawnActor<AC_HitBox>(HitBoxClass, SpawnLocation + addLoc, rotator, SpawnParams);
		if (Hitbox)
		{
			Hitbox->lifeTime = 3;
			Hitbox->team = team;
			Hitbox->slowmotion = .3;
			Hitbox->dmg = 8 * (1 + gagePower / 30);
		}
		Hitbox->SetActorScale3D(FVector(1 + gagePower / 50));
		Hitbox->boxComp->SetCollisionProfileName(TEXT("HitBox"));
		});
	AnimIns->OnCancelable.AddLambda([this]() -> void {

		bCancelable = true;
	});

	AnimIns->OnDoRotation.AddLambda([this]() -> void {

		IsRotation = true;
	});
}

void AC_Player::OnAnimeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage)
	{
		if (!DoCombo)
		{
			if (!bInterrupted)//정상 종료
			{
				StateReset();
			}
			attackIndex = 0;
		}
		Trail->Deactivate();
		IsCheckCombo = false;
		DoCombo = false;
	}
	else if (Montage == RollMontage)
	{
		if (!bInterrupted)
		{
			StateReset();
		}
		IsRotation							= true;
		GetCharacterMovement()->Velocity.X = 0;
		GetCharacterMovement()->Velocity.Y = 0;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.0f, 0.0f);
	}
	else if (Montage == PowerAttackChargingMontage)
	{
		if (!DoCombo)
		{
			if (!bInterrupted)//정상 종료
			{
				StateReset();
			}
		}
		Trail->Deactivate();
	}
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
		else
		{
			AddMovementInput(StateDirectionX, StateVector.Y);
			AddMovementInput(StateDirectionY, StateVector.X);
		}
	}
	if (CameraBoom->TargetArmLength != zoomTarget)
	{
		if (abs(CameraBoom->TargetArmLength - zoomTarget) > 5)
		{
			CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength, zoomTarget, .2);
		}
		else
		{
			CameraBoom->TargetArmLength = zoomTarget;
		}
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

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AC_Player::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AC_Player::ArrowAttack);

		EnhancedInputComponent->BindAction(PowerAttackAction, ETriggerEvent::Triggered, this, &AC_Player::PowerAttackStart);
		EnhancedInputComponent->BindAction(PowerAttackAction, ETriggerEvent::Completed, this, &AC_Player::PowerAttackEnd);

		EnhancedInputComponent->BindAction(ArrowAction, ETriggerEvent::Triggered, this, &AC_Player::ArrowStart);
		EnhancedInputComponent->BindAction(ArrowAction, ETriggerEvent::Completed, this, &AC_Player::ArrowEnd);

		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AC_Player::Roll);
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

	if (Controller != nullptr)
	{ 
		if(State == PLAYERSTATE::MOVEMENT)
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
	PlayerVector = MovementVector;
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
		if (State == PLAYERSTATE::ATTACK)
		{
			IsCheckCombo = true;
		}
		if (State == PLAYERSTATE::MOVEMENT || bCancelable)
		{
			//============================================================
			if (!(AnimIns->Montage_IsPlaying(AttackMontage)))
			{
				AnimIns->Montage_Play(AttackMontage, 1.0f);
			}

			attackIndex = 1;
			State = PLAYERSTATE::ATTACK;

			//===============================================================





			FVector2D MovementVector = FVector2D(0, 1);

			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			RotationTarget = YawRotation;

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);

			bCancelable = false;
			StateDirectionX = ForwardDirection;
			StateDirectionY = RightDirection;
			UE_LOG(LogTemp, Warning, TEXT("x		: %d"), StateDirectionX);
			UE_LOG(LogTemp, Warning, TEXT("y		: %d"), StateDirectionY);
			StateVector = FVector2D(0, 0);
			Statestep = 0;
			StateTimer = 0;
		}
		else if (State == PLAYERSTATE::ARROW)
		{	
			gageArrow = FMath::Clamp(gageArrow + .5, 0, 100); // ...at this rotation rate
		}
	}
}

void AC_Player::ArrowAttack()
{
	if (State == PLAYERSTATE::ARROW)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FRotator rotator = Controller->GetControlRotation();
		FVector  SpawnLocation = GetActorLocation();
		FVector	 addLoc = GetActorRightVector() * 50;
		rotator.Yaw += 5;
		SpawnLocation.Z -= 50.f;
		AC_Arrow* Arrow = GetWorld()->SpawnActor<AC_Arrow>(ArrowClass, SpawnLocation + addLoc, rotator, SpawnParams);
		if (Arrow)
		{
			Arrow->dmg = 6 + gageArrow/20;
			Arrow->lifeTime = 100;
			Arrow->boxComp->SetCollisionProfileName(TEXT("HitBox"));
		}
	}
}

void AC_Player::Roll()
{
	if (Controller != nullptr && (State == PLAYERSTATE::MOVEMENT || State == PLAYERSTATE::ATTACK || State == PLAYERSTATE::POWERATTACK) )
	{
		AnimIns->Montage_Play(RollMontage, 1.0f);
		//boxComp->SetCollisionProfileName(TEXT("NoCollision"));
		State = PLAYERSTATE::ROLL;

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection	= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection	= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		StateVector = PlayerVector;
		// add movement 
		AddMovementInput(ForwardDirection, StateVector.Y);
		AddMovementInput(RightDirection, StateVector.X);

		StateDirectionX = ForwardDirection;
		StateDirectionY = RightDirection;
		GetCharacterMovement()->MaxWalkSpeed = 0;
		GetCharacterMovement()->Velocity.X = YawRotation.RotateVector(FVector(StateVector.Y, StateVector.X, 0)).X * 1500;
		GetCharacterMovement()->Velocity.Y = YawRotation.RotateVector(FVector(StateVector.Y, StateVector.X, 0)).Y*1500;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 1500.0f, 0.0f);
		IsRotation = false;
	}
}

void AC_Player::ArrowStart()
{
	if (Controller != nullptr && State == PLAYERSTATE::MOVEMENT)
	{
		zoomTarget = 200.0f;
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
		zoomTarget = 400.0f;
		FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Statestep = 100;
	}
}

void AC_Player::PowerAttackStart()
{
	if (Controller != nullptr && (State == PLAYERSTATE::MOVEMENT || bCancelable))
	{
		State = PLAYERSTATE::POWERCHARGING;
		Statestep = 0;
		StateTimer = 0;
		GetCharacterMovement()->Velocity.X = 0;
		GetCharacterMovement()->Velocity.Y = 0;
		GetCharacterMovement()->MaxWalkSpeed = 0;
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		RotationTarget = YawRotation;

		StateVector = FVector2D(0);
		AnimIns->Montage_Play(PowerAttackMontage, 0, EMontagePlayReturnType::MontageLength, .5f);
	}
}
void AC_Player::PowerAttackEnd()
{
	if (Controller != nullptr && State == PLAYERSTATE::POWERCHARGING)
	{
		State = PLAYERSTATE::POWERATTACK;
		StateTimer = 0;
		Statestep = 0;
		GetCharacterMovement()->MaxWalkSpeed = 200;
		FVector2D MovementVector = FVector2D(0, 1);

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		RotationTarget = YawRotation;

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		bCancelable = false;
		StateDirectionX = ForwardDirection;
		StateDirectionY = RightDirection;
		UE_LOG(LogTemp, Warning, TEXT("x		: %d"), StateDirectionX);
		UE_LOG(LogTemp, Warning, TEXT("y		: %d"), StateDirectionY);
		StateVector = FVector2D(0, 0);
		Statestep = 0;
		StateTimer = 0;
		AnimIns->Montage_Play(PowerAttackMontage, 1, EMontagePlayReturnType::MontageLength, .5f);
	}
}

void AC_Player::BarrierStart()
{
	if (Controller != nullptr && State == PLAYERSTATE::MOVEMENT)
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
		GetCharacterMovement()->MaxWalkSpeed = 0;
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		RotationTarget = YawRotation;

		StateVector = FVector2D(0);
		//Barrier = GetWorld()->SpawnActor<AC_Barrier>(BarrierClass, SpawnLocation, rotator, SpawnParams);
		//if (Barrier)
		//{
		//	Barrier->lifeTime	= 100;
		//	Barrier->Host		= this;
		//}
		AnimIns->IsBarrier = true;
		zoomTarget = 370.f;\
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), barriersound, GetActorLocation(), .4);
	}
}
void AC_Player::BarrierEnd()
{
	if (Controller != nullptr)
	{
		//Barrier->IsBoom = true;
		State = PLAYERSTATE::MOVEMENT;
		GetCharacterMovement()->MaxWalkSpeed = 200;
		AnimIns->Montage_Play(PopMontage, 2);
		AnimIns->IsBarrier = false;
		zoomTarget = 400.f;
	}
}

void AC_Player::StateAttack()
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	FRotator SetRot = GetActorRotation();
	if (RotationTarget.Yaw != SetRot.Yaw)
	{
		RotationTarget = YawRotation;

		if (RotationTarget.Yaw > 180)
		{
			RotationTarget.Yaw -= 360;
		}
		if (SetRot.Yaw - RotationTarget.Yaw > 180)
		{
			SetRot.Yaw = FMath::Lerp(SetRot.Yaw, RotationTarget.Yaw + 360, .1);
		}
		else if (SetRot.Yaw - RotationTarget.Yaw < -180)
		{
			SetRot.Yaw = FMath::Lerp(SetRot.Yaw, RotationTarget.Yaw - 360, .1);
		}
		else
		{
			SetRot.Yaw = FMath::Lerp(SetRot.Yaw, RotationTarget.Yaw, .1);
		}
		SetActorRotation(SetRot);
	}
	if (Statestep == 0)
	{
		GetCharacterMovement()->Velocity.X = SetRot.Vector().X * 500;
		GetCharacterMovement()->Velocity.Y = SetRot.Vector().Y * 500;
		StateDirectionX = FRotationMatrix(SetRot).GetUnitAxis(EAxis::X);
		StateDirectionY = FRotationMatrix(SetRot).GetUnitAxis(EAxis::Y);

		Statestep = 1;
		StateTimer = 0;
	}
}

void AC_Player::StatePowerAttack()
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	FRotator SetRot = GetActorRotation();
	if (Statestep == 0)
	{
		GetCharacterMovement()->Velocity.X = SetRot.Vector().X * 500;
		GetCharacterMovement()->Velocity.Y = SetRot.Vector().Y * 500;
		StateDirectionX = FRotationMatrix(SetRot).GetUnitAxis(EAxis::X);
		StateDirectionY = FRotationMatrix(SetRot).GetUnitAxis(EAxis::Y);

		Statestep = 1;
		StateTimer = 0;
	}
}
void AC_Player::StateRoll()
{
	if (IsRotation)
	{
		FRotator SetRot = GetActorRotation();
		if (RotationTarget.Yaw != SetRot.Yaw)
		{
			UE_LOG(LogTemp, Warning, TEXT("now		:%s"), *SetRot.ToString());
			UE_LOG(LogTemp, Warning, TEXT("target	: %s"), *RotationTarget.ToString());
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			RotationTarget = YawRotation;

			if (RotationTarget.Yaw > 180)
			{
				RotationTarget.Yaw -= 360;
			}
			if (SetRot.Yaw - RotationTarget.Yaw > 180)
			{
				SetRot.Yaw = FMath::Lerp(SetRot.Yaw, RotationTarget.Yaw + 360, .1);
			}
			else if (SetRot.Yaw - RotationTarget.Yaw < -180)
			{
				SetRot.Yaw = FMath::Lerp(SetRot.Yaw, RotationTarget.Yaw - 360, .1);
			}
			else
			{
				SetRot.Yaw = FMath::Lerp(SetRot.Yaw, RotationTarget.Yaw, .1);
			}
			SetActorRotation(SetRot);
		}
	}
}
void AC_Player::StateBarrier()
{
	FRotator SetRot = GetActorRotation();
	if (RotationTarget.Yaw != SetRot.Yaw)
	{
			if (RotationTarget.Yaw > 180)
			{
				RotationTarget.Yaw -= 360;
			}
			if (SetRot.Yaw - RotationTarget.Yaw > 180)
			{
				SetRot.Yaw = FMath::Lerp(SetRot.Yaw, RotationTarget.Yaw + 360, .1);
			}
			else if (SetRot.Yaw - RotationTarget.Yaw < -180)
			{
				SetRot.Yaw = FMath::Lerp(SetRot.Yaw , RotationTarget.Yaw - 360, .1);
			}
			else
			{
				SetRot.Yaw = FMath::Lerp(SetRot.Yaw, RotationTarget.Yaw, .1);
			}
			SetActorRotation(SetRot);
	}
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
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	RotationTarget = YawRotation;
	FRotator SetRot = GetActorRotation();
	if (RotationTarget.Yaw != SetRot.Yaw)
	{
		UE_LOG(LogTemp, Warning, TEXT("now		:%s"), *SetRot.ToString());
		UE_LOG(LogTemp, Warning, TEXT("target	: %s"), *RotationTarget.ToString());
		float persent = .1 - .08* gagePower/100;
		if (RotationTarget.Yaw > 180)
		{
			RotationTarget.Yaw -= 360;
		}
		if (SetRot.Yaw - RotationTarget.Yaw > 180)
		{
			SetRot.Yaw = FMath::Lerp(SetRot.Yaw, RotationTarget.Yaw + 360, persent);
		}
		else if (SetRot.Yaw - RotationTarget.Yaw < -180)
		{
			SetRot.Yaw = FMath::Lerp(SetRot.Yaw, RotationTarget.Yaw - 360, persent);
		}
		else
		{
			SetRot.Yaw = FMath::Lerp(SetRot.Yaw, RotationTarget.Yaw, persent);
		}
		SetActorRotation(SetRot);
	}
	gagePower = FMath::Clamp(gagePower + .5, 0, 100);
}
void AC_Player::StateReset()
{
	bCancelable		= false;
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