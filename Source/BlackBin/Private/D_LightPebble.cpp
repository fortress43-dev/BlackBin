// Fill out your copyright notice in the Description page of Project Settings.


#include "D_LightPebble.h"
#include "DebugMessages.h"
#include <Components/BoxComponent.h>
#include "ActorSequenceComponent.h"

// 생성자
// 초기 모델링을 할당한다

// Overlap Funtion 
// 오버랩되면 빛이 밝아졌다가 다시 어두워진다.
// 커브 곡선은 사용한다.
// 부딪힌 후 0.5


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

	// 이미션의 색을 올렸다가 내린다
	// 필요 : 재질과 파라미터

}
