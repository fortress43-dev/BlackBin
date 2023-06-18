// Fill out your copyright notice in the Description page of Project Settings.


#include "D_CrystalLight.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Materials/MaterialInterface.h>
#include "DebugMessages.h"

// Sets default values
AD_CrystalLight::AD_CrystalLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region Components 
	// create components 
	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("sphereCollider"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));

	RootComponent = sphereCollider;
	meshComp->SetupAttachment(RootComponent);
#pragma endregion

#pragma region Collision Setting
	sphereCollider->SetCollisionProfileName("LightBarrier");

#pragma endregion

#pragma region ConstructHelpers
	// Construct Helpers StaticMesh
	ConstructorHelpers::FObjectFinder<UStaticMesh> tmpMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (tmpMesh.Succeeded()) {
		meshComp->SetStaticMesh(tmpMesh.Object);
	}

	// material Setup
	ConstructorHelpers::FObjectFinder<UMaterialInterface> tmpMat(TEXT("/Script/Engine.Material'/Game/DKW/Material/Barriers/M_CrystalLight.M_CrystalLight'"));
	if (tmpMat.Succeeded()) {
		meshComp->SetMaterial(0, tmpMat.Object);
	}

	ConstructorHelpers::FObjectFinder<UCurveFloat> tmpCurve(TEXT("/Script/Engine.CurveFloat'/Game/DKW/Anim/BerrierAnim/CF_CrystalLight.CF_CrystalLight'"));
	if (tmpCurve.Succeeded()) {
		AnimCurve = tmpCurve.Object;
	}
#pragma endregion
}

// Called when the game starts or when spawned
void AD_CrystalLight::BeginPlay()
{
	
	Super::BeginPlay();

	if (AnimCurve) {
		print("animcurve");
		
		// callback 함수를 선언해준다
		//  플레이 중 / 끝날 때
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;
		
		// callback 함수에 함수들을 bind 해준다
		//  플레이도중에 실행할 함수와 끝날때 실행할 함수를 bind
		TimelineCallback.BindUFunction(this, FName("ScaleUP"));
		TimelineFinishedCallback.BindUFunction(this, FName("DestroyCrystal"));
		
		// time line 과 callback 함수를 연결시킨다
		//  플레이 하는 동안만 callback 함수들을 호출한다
		//  끝났을 때는 미리 bind 해준 함수들을 실행한다
		MyTimeline.AddInterpFloat(AnimCurve, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
	print("playing");
	MyTimeline.PlayFromStart();
	scale = GetActorScale3D();
}

// Called every frame
void AD_CrystalLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 태어나면 크기가 일정속도로 커진다
	MyTimeline.TickTimeline(DeltaTime);
	curTime += DeltaTime;
}

void AD_CrystalLight::ScaleUP() {

	TimelineValue = MyTimeline.GetPlaybackPosition();
	CurveFloatValue = AnimCurve->GetFloatValue(TimelineValue);
	
	// multiply curveValue with original value
	FVector newScale = FVector::OneVector*CurveFloatValue;
	SetActorScale3D(newScale);

}
void AD_CrystalLight::DestroyCrystal() {
	print("CrystalLight destroyed");
	Destroy();
}
