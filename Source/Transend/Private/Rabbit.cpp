// Fill out your copyright notice in the Description page of Project Settings.

#include "Transend.h"
#include "Rabbit.h"


ARabbit::ARabbit(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void ARabbit::Death()
{
	Super::Death();

	ATransendPlayerController *PC = (ATransendPlayerController*)GetWorld()->GetFirstPlayerController();
	PC->CurrentXP += 25; //// FOR TESTING!
	PC->RespawnPoint = SpawnLocation;
	PlayEnemySound(DeathSound);
	Destroy();

}