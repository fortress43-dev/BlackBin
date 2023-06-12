// Fill out your copyright notice in the Description page of Project Settings.


#include "D_Rot.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "DebugMessages.h"
#include "Kismet/GameplayStatics.h"
#include "C_Barrier.h"



// Sets default values
AD_Rot::AD_Rot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	return;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCompCollision"));
	SetRootComponent(boxComp);
	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	//boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	//boxComp->SetCollisionProfileName(TEXT(""));
}

// Called when the game starts or when spawned
void AD_Rot::BeginPlay()
{
	Super::BeginPlay();
	p1 = GetActorLocation();
	p2 = FVector(p1.X, p1.Y, p1.Z + rotActiveHeight);

	// rotManager setting
	//rotManager = Cast<AD_RotManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AD_RotManager::StaticClass()));
}

void AD_Rot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	FString name = OtherActor->GetName();
	AC_Barrier* barrier= Cast<AC_Barrier>(OtherActor);
	if (barrier) {
		rotCollect = RotCollect::activating;
		printf("Rot was hitted with %s", *name);
	}
	UE_LOG(LogTemp, Warning, TEXT("collide with : %s"), *name);
}


// Called every frame
void AD_Rot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	dt = DeltaTime;
	switch (rotCollect)
	{
	case hidden:

		break;
	case activating:
		ActivateRot();
		break;
	case activated:
		CollectActivatedRot();
		break;
	case collected:
		// AI Move����� ���ش�
		usingAIMove = true;
		break;
	default:
		break;
	}

}
/// <summary>
/// this Function is for calling from player's class with input system
/// </summary>
void AD_Rot::InterAction()
{
	usingAIMove = true;
}

void AD_Rot::ActivateRot()
{
	curTime += dt;
	//1�� �ȿ� rot�� ���� �ö󰣴�
	FVector newPos = FMath::Lerp(p1, p2, curTime);
	SetActorLocation(newPos);

	if (curTime > 1.0f) {
		rotCollect = RotCollect::activated;
		curTime = 0;
		//rotManager->curRot = this;

		// for proto
		rotCollect = RotCollect::collected;
		rotState = RotState::follow;

		print("rot Collect Success / rot is following you");
	}
}

void AD_Rot::CollectActivatedRot()
{
	// rot�� ��ġ�� player��ó ������ ��ġ�� ��ġ�ϰ�
	// navi�� �Ҵ�
	// UI�� ����
	if (onCollectKey) {
		//SetActorLocation(p1);
		rotCollect = RotCollect::collected;
		rotState = RotState::follow;
		
		print("rot Collect Success / rot is following you");

	}
}






