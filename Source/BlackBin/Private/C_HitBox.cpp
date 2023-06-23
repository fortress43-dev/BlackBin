// Fill out your copyright notice in the Description page of Project Settings.


#include "C_HitBox.h"
#include "C_Mob.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AC_HitBox::AC_HitBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(100));
	Fx = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/CSK/Blueprints/NS_Hit.NS_Hit'"));
	hitsound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/CSK/Sound/Snd_Hit.Snd_Hit'"));

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
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
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
				printf("HIT : %d", dmg);
				
				MobActor->Hit(dmg);
				FVector randVec = FVector(FMath::RandRange(-50, 50), FMath::RandRange(-50, 50), FMath::RandRange(-50, 50));
				if (Fx)
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Fx, OtherActor->GetActorLocation() + randVec);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitsound, GetActorLocation() + randVec, FMath::FRandRange(.3, .4));
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), slowmotion);
			}

		}
	}
}