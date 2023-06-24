// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Arrow.h"
#include "C_Mob.h"
#include "Components/BoxComponent.h"
AC_Arrow::AC_Arrow()
{
	PrimaryActorTick.bCanEverTick = true;
	boxComp->SetBoxExtent(FVector3d(50, 10, 10));
}

// Called when the game starts or when spawned
void AC_Arrow::BeginPlay()
{
	Super::BeginPlay();
	IsBrocken = true;
}

// Called every frame
void AC_Arrow::Tick(float DeltaTime)
{
	if (ativate)
	{
		Super::Tick(DeltaTime);
		FVector ForwardVector = GetActorRotation().Vector(); // ȸ�� ���� ���

		// �̵��� �Ÿ��� ������ �����Ͽ� ForwardVector�� ����Ͽ� �̵�
		FVector NewLocation = GetActorLocation() + ForwardVector * 20.f;
		SetActorLocation(NewLocation);
	}
}

void AC_Arrow::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ativate)
	{
		Super::NotifyActorBeginOverlap(OtherActor);
	}
}