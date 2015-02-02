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
		CurrentLevel = ELevelXP::E_One;
	}
	if (CurrentXP >= 100 && CurrentXP < 200)
	{
		MinXP = 100;
		MaxXP = 200;
		CurrentLevel = ELevelXP::E_Two;
	}
	return CurrentLevel;
}

void ATransendPlayerController::LevelAbilities()
{

	ATransendCharacter *Character = (ATransendCharacter*)GetWorld()->GetFirstPlayerController()->GetPawn();
	if (Character)
	{
		switch (CurrentLevel)
		{
		case ELevelXP::E_One:
			Character->bDoubleJumpEnabled = false;
			Character->bSprintEnabled = false;
			break;
		case ELevelXP::E_Two:
			Character->bDoubleJumpEnabled = true;
			Character->bSprintEnabled = true;
			break;
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