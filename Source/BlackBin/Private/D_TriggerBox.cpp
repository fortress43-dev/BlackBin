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
/// ���� Active�Ǿ�� �ϴ� ���������� ��� �����ִ� �� �˻��Ѵ�
/// </summary>
/// <returns></returns>
bool AD_TriggerBox::IsPreConditionFulFill()
{
	// �������� �̿��� �޾ƿ����� �ߴµ� ����� ������ ã�� ���ϴ� ������ �ֳ�
	// ���� ���� �ʿ� false ����Ȯ���ϱ�
	for (auto& ele : preConditionActors) {
		if (!ele->isActive) return false;
	}
	return true;
}

// ��ġ�� ������ bool�Լ�
// �� ���ǰ� ������
//=> �ߵ�����