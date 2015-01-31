// Fill out your copyright notice in the Description page of Project Settings.

#include "Transend.h"
#include "TransendPlayerController.h"


ATransendPlayerController::ATransendPlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerPawnBP(TEXT("Blueprint'/Game/Blueprints/MyCharacter.MyCharacter'"));
	if (PlayerPawnBP.Object)
	{
		Spawner = (UClass*)PlayerPawnBP.Object->GeneratedClass;
	}
}


void ATransendPlayerController::KilledPlayer()
{
	GetWorldTimerManager().SetTimer(this, &ATransendPlayerController::SpawnPlayer, 3.f, false);
}

void ATransendPlayerController::SpawnPlayer()
{
	ATransendCharacter *Respawned = GetWorld()->SpawnActor<ATransendCharacter>(Spawner,RespawnPoint->GetActorLocation(),RespawnPoint->GetActorRotation());
	if (Respawned)
	{
		Possess(Respawned);
	}

}