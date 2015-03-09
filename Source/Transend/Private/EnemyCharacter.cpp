// Fill out your copyright notice in the Description page of Project Settings.

#include "Transend.h"
#include "EnemyCharacter.h"

AEnemyCharacter::AEnemyCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	MaxHP = 100;
	CurrentHP = MaxHP;
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentHP <= 0)
	{
		Death();
	}
}

void AEnemyCharacter::Death()
{
	//Super::Death();

	//ATransendPlayerController *PC = (ATransendPlayerController*)GetWorld()->GetFirstPlayerController();
	//PC->CurrentXP += 25; //// FOR TESTING!
	//PC->RespawnPoint = SpawnLocation;
	//PlayEnemySound(DeathSound);
	//Destroy();
}

