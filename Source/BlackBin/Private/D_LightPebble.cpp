// Fill out your copyright notice in the Description page of Project Settings.


#include "D_LightPebble.h"
#include "DebugMessages.h"
#include <Components/BoxComponent.h>
#include "ActorSequenceComponent.h"

// ������
// �ʱ� �𵨸��� �Ҵ��Ѵ�

// Overlap Funtion 
// �������Ǹ� ���� ������ٰ� �ٽ� ��ο�����.
// Ŀ�� ��� ����Ѵ�.
// �ε��� �� 0.5


// Sets default values
AD_LightPebble::AD_LightPebble()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region Component Setting

#pragma region Components
	boxComp = CreateDefaultSubobject<UBoxComponent>("boxComp");
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>("meshComp");
	seqComp = CreateDefaultSubobject<UActorSequenceComponent>("seqComp");

	RootComponent = boxComp;
	meshComp->SetupAttachment(boxComp);

	boxComp->SetBoxExtent(FVector(10, 10, 10));
	meshComp->SetRelativeScale3D(FVector::OneVector * 0.2f);
	
#pragma endregion

#pragma region Collision Setting
	boxComp->SetCollisionProfileName("Crystal");

#pragma endregion

	

	
#pragma region Constructor
	ConstructorHelpers::FObjectFinder<UStaticMesh>tmpMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tmpMesh.Succeeded()) {
		meshComp->SetStaticMesh(tmpMesh.Object);
	}
#pragma endregion

	


#pragma endregion
}

// Called when the game starts or when spawned
void AD_LightPebble::BeginPlay()
{
	Super::BeginPlay();
	seqComp->PlaySequence();
	
}

// Called every frame
void AD_LightPebble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AD_LightPebble::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	print("Puddles");

	// �̹̼��� ���� �÷ȴٰ� ������
	// �ʿ� : ������ �Ķ����

}
