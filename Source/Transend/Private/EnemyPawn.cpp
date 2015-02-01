// Fill out your copyright notice in the Description page of Project Settings.

#include "Transend.h"
#include "EnemyPawn.h"

AEnemyPawn::AEnemyPawn(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	Mesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxCollision"));
	BoxCollision->AttachTo(RootComponent);
}

void AEnemyPawn::Death()
{
	//Destroy();
}

UAudioComponent *AEnemyPawn::PlayEnemySound(USoundCue *Sound)
{
	UAudioComponent* AC = NULL;
	if (Sound)
	{
		AC = UGameplayStatics::PlaySoundAttached(Sound, RootComponent);
	}

	return AC;
}
