// Fill out your copyright notice in the Description page of Project Settings.


#include "D_RotManager.h"

// Sets default values
AD_RotManager::AD_RotManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AD_RotManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AD_RotManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// rot 공격 키를 누르면 rot의 상태가 공격으로 전환
	// rot 가 목적지를 향해서 날아감
	// 목적지에 다다르면 원운동을 한다
	// 다시 키를 누르면 폭발한다
	// rot는 공중으로 날아가며 2초간 넉다운 상태에 빠진다

	


}

void AD_RotManager::StateMachine()
{

}

