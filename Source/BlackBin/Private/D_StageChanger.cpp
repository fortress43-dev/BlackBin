// Fill out your copyright notice in the Description page of Project Settings.


#include "D_StageChanger.h"
#include "Sound/SoundBase.h"
#include <Components/BoxComponent.h>
#include "DebugMessages.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AD_StageChanger::AD_StageChanger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>("boxComp");
	RootComponent = boxComp;
	boxComp->SetBoxExtent(FVector(50, 10, 20));

	// theme Sound call
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT(""));
	if (tempSound.Succeeded()) {themeMusic = tempSound.Object;}
	
}

// Called when the game starts or when spawned
void AD_StageChanger::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AD_StageChanger::OnComponentBeginOverlap);
}

// Called every frame
void AD_StageChanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AD_StageChanger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	print("stagechager");
	UGameplayStatics::PlaySound2D(GetWorld(), themeMusic);
}

