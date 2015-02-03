// Fill out your copyright notice in the Description page of Project Settings.

#include "Transend.h"
#include "TransendPlayerController.h"


ATransendPlayerController::ATransendPlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	//Constructor helper to store the correct TSubclassOf variable for respawning the player into the map after death
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerPawnBP(TEXT("Blueprint'/Game/Blueprints/MyCharacter.MyCharacter'"));
	if (PlayerPawnBP.Object)
	{
		Spawner = (UClass*)PlayerPawnBP.Object->GeneratedClass;
	}
}

void ATransendPlayerController::Tick(float DeltaSeconds)
{
	CurrentLevel = GetXPLevel();
	LevelAbilities();
	GetXPBarPercentage();
}

ELevelXP::Level ATransendPlayerController::GetXPLevel()
{
	if (CurrentXP < 100)
	{
		MinXP = 0;
		MaxXP = 100;
		CurrentLevel = ELevelXP::E_01;
	}
	if (CurrentXP >= 100 && CurrentXP < 200)
	{
		MinXP = 100;
		MaxXP = 200;
		CurrentLevel = ELevelXP::E_02;
	}
	return CurrentLevel;
}

void ATransendPlayerController::LevelAbilities()
{

	ATransendCharacter *Character = (ATransendCharacter*)GetWorld()->GetFirstPlayerController()->GetPawn();
	//if the character exists
	if (Character)
	{
		//switch statement for the CurrentLevel of the player
		switch (CurrentLevel)
		{
			//Level 01
		case ELevelXP::E_01:
			//Turn off the double jump and Sprint
			Character->bDoubleJumpEnabled = false;
			Character->bSprintEnabled = false;
			break;

			//Level 02
		case ELevelXP::E_02:
			//turn on the double jump and Sprint
			Character->bDoubleJumpEnabled = true;
			Character->bSprintEnabled = true;
			break;

			//else
		default:
			break;
		}
	}
}

void ATransendPlayerController::GetXPBarPercentage()
{
	CurrentPercentage = FMath::GetRangePct(MinXP, MaxXP, CurrentXP);
}

void ATransendPlayerController::KilledPlayer_Implementation()
{
	GetWorldTimerManager().SetTimer(this, &ATransendPlayerController::SpawnPlayer, 3.f, false);
}

void ATransendPlayerController::SpawnPlayer_Implementation()
{
	ATransendCharacter *Respawned = GetWorld()->SpawnActor<ATransendCharacter>(Spawner,RespawnPoint->GetActorLocation(),RespawnPoint->GetActorRotation());
	if (Respawned)
	{
		Possess(Respawned);
	}

}