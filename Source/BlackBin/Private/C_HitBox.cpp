// Fill out your copyright notice in the Description page of Project Settings.


#include "C_HitBox.h"
#include "C_Mob.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
// Sets default values
AC_HitBox::AC_HitBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(boxComp);
	//boxComp->bgenera

	
	//boxComp = GetComponentByClass<UBoxComponent>();
	//GetComponentsByClass<UBoxComponent>();

}

// Called when the game starts or when spawned
void AC_HitBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_HitBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	lifeTime = FMath::Max(lifeTime - 10.f * DeltaTime, 0);
	if (lifeTime == 0)
	{
		Destroy();
	}
}

void AC_HitBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor->IsA<AC_Mob>())
	{
		AC_Mob* MobActor = Cast<AC_Mob>(OtherActor);
		if (MobActor)
		{
			if (MobActor->team != team)
			{
				MobActor->Hit(float(dmg));
			}

		}
	}
}