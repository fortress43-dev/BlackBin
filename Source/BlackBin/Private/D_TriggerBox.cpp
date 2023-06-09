// Fill out your copyright notice in the Description page of Project Settings.


#include "D_TriggerBox.h"
#include "DebugMessages.h"

// Sets default values
AD_TriggerBox::AD_TriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AD_TriggerBox::BeginPlay()
{
	Super::BeginPlay();
	auto Cube = FindComponentByClass<UStaticMeshComponent>();
	auto Material = Cube->GetMaterial(0);

	auto DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	Cube->SetMaterial(0, DynamicMaterial);
}

void AD_TriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// <summary>
/// play connected Animation with this trigger Actor
/// </summary>
/// <param name="overlappedActor"></param>
/// <param name="otherActor"></param>
void AD_TriggerBox::PlayAnim(AActor* overlappedActor, AActor* otherActor)
{
	isActive = IsPreConditionFulFill();

	print("collided");
	if (!isActive) {
		print("Button is not activated")
			return;
	}
	if (animFactory) {
		if (animFactory->SequencePlayer)
		{
			animFactory->SequencePlayer->Play();
		}
	}
	

	return;
	if (otherActor && otherActor != this) {
		//APawn* character = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		APawn* character = Cast<APawn>(overlappedActor);
		if (character && animFactory)
		{
			if (animFactory->SequencePlayer)
			{
				animFactory->SequencePlayer->Play();
			}
		}
	}
}

/// <summary>
/// 먼저 Active되어야 하는 오브제들이 모두 켜져있는 지 검사한다
/// </summary>
/// <returns></returns>
bool AD_TriggerBox::IsPreConditionFulFill()
{
	// 다형성을 이용해 받아오려고 했는데 연결된 변수를 찾지 못하는 문제가 있네
	// 여기 수정 필요 false 관계확인하기
	for (auto& ele : preConditionActors) {
		if (!ele->isActive) return false;
	}
	return true;
}

// 겹치고 있을때 bool함수
// 위 조건과 섞어줌
//=> 발동조건