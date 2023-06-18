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
		
		// callback �Լ��� �������ش�
		//  �÷��� �� / ���� ��
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;
		
		// callback �Լ��� �Լ����� bind ���ش�
		//  �÷��̵��߿� ������ �Լ��� ������ ������ �Լ��� bind
		TimelineCallback.BindUFunction(this, FName("ScaleUP"));
		TimelineFinishedCallback.BindUFunction(this, FName("DestroyCrystal"));
		
		// time line �� callback �Լ��� �����Ų��
		//  �÷��� �ϴ� ���ȸ� callback �Լ����� ȣ���Ѵ�
		//  ������ ���� �̸� bind ���� �Լ����� �����Ѵ�
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
	// �¾�� ũ�Ⱑ �����ӵ��� Ŀ����
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
