// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTriggerBox.h"
#include "DebugMessages.h"


AMyTriggerBox::AMyTriggerBox() {
	//OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBox::PlayAnim);
}

void AMyTriggerBox::BeginPlay() {
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBox::PlayAnim);
}


void AMyTriggerBox::PlayAnim(AActor* overlappedActor, AActor* otherActor)
{
	print("collided");
	if (!isActive) {
		print("Button is not activated")
		return;
	}

	if (animFactory->SequencePlayer)
	{
		animFactory->SequencePlayer->Play();
	}

	
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

