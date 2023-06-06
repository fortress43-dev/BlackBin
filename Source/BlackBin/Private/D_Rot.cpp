// Fill out your copyright notice in the Description page of Project Settings.


#include "D_Rot.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
AD_Rot::AD_Rot()
{
	InitRot();
}

// Called when the game starts or when spawned
void AD_Rot::BeginPlay()
{
	Super::BeginPlay();
	//리스트에 사용할 포지션을 넣어놓자
	StoreLerpPoints(lineResolution);

	return;

	FVector lastPos;
	FVector curPos;
	float t = 0;
	float dt = 1 / 20.0f;
	
	for (int i = 0; i < 20; i++)
	{
		curPos = Lerp3Points(st, mid, en, t);
		if (t != 0) { DrawDebugLine(GetWorld(), lastPos, curPos, FColor(255, 0, 0),true); }
		LinePoseList.Add(curPos);
		lastPos = curPos;
		t += dt;
	}

}

// Called every frame
void AD_Rot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawLerpLine();
	return;
	FVector rotPos = GetActorLocation();
	curTime += DeltaTime * speed;
	if (curTime < 1.0f) {
		Lerp3Points(st,mid,en,curTime);
	}
	else { curTime = 0; }
}
/// <summary>
/// this Function is for short Test for making Lerp Curve
/// </summary>
/// <param name="stPos"></param>
/// <param name="midPos"></param>
/// <param name="enPos"></param>
/// <param name="t"></param>
FVector AD_Rot::Lerp3Points(FVector stPos, FVector midPos, FVector enPos, float t)
{
	FVector curPos;
	FVector tmp1;
	FVector tmp2;
	tmp1   = FMath::Lerp(stPos, midPos, t);
	tmp2   = FMath::Lerp(midPos, enPos, t);
	curPos = FMath::Lerp(tmp1, tmp2, t);
	
	SetActorLocation(curPos);
	return curPos;
}

/// <summary>
/// Create BoxComp
/// </summary>
inline void AD_Rot::InitRot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	//MyBoxComponent->InitBoxExtent(FVector(50, 50, 50));
	RootComponent = MyBoxComponent;

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	rot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rot"));
	rot->SetupAttachment(RootComponent);
	if (tempMesh.Succeeded()) {
		rot->SetStaticMesh(tempMesh.Object);
	}
}

void AD_Rot::StoreLerpPoints(int resolution)
{
	float dt = 1.0f / resolution;
	float t = 0;
	if (posRefList.Num() < 3) return;
	for(int i = 2; i < posRefList.Num(); i++) {
		FVector pos;
		FVector tmp1;
		FVector tmp2;

		for (int j = 0; j < resolution; i++)
		{
			t = dt * j; // DeltaTime과 같은 원리
			tmp1 = FMath::Lerp(posRefList[i - 2], posRefList[i - 1], t);
			tmp2 = FMath::Lerp(posRefList[i - 1], posRefList[i], t);
			pos  = FMath::Lerp(tmp1, tmp2, t);
			LinePoseList.Add(pos);
		}
	}
}

void AD_Rot::DrawLerpLine()
{
	if (LinePoseList.Num() < 2)return;
	for (int i = 1; i < LinePoseList.Num(); i++)
	{
		FVector LineStart = LinePoseList[i - 1];
		FVector LineEnd = LinePoseList[i];

		DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor(255, 0, 0), true, -1, 0, 10);
	}
}
