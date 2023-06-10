// Fill out your copyright notice in the Description page of Project Settings.


#include "D_RotManager.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DebugMessages.h"

// Sets default values
AD_RotManager::AD_RotManager()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AD_RotManager::BeginPlay()
{
	Super::BeginPlay();
    print("begin works well");
}

// Called every frame
void AD_RotManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AD_RotManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Get the player controller
    APlayerController* PC = Cast<APlayerController>(GetController());

    // Get the local player subsystem
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
    // Clear out existing mapping, and add our mapping
    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext(InputMapping, 0);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
        EnhancedInputComponent->BindAction(RotAction, ETriggerEvent::Triggered, this, &AD_RotManager::RotActionSkill);
        EnhancedInputComponent->BindAction(Interaction, ETriggerEvent::Triggered, this, &AD_RotManager::InteractionStart);
    
    
    
    }

   
}

void AD_RotManager::RotActionSkill()
{
    print("rotAcion");
}

void AD_RotManager::InteractionStart()
{
    print("interaction");
}



