// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TransendCharacter.h"
#include "TransendPlayerController.generated.h"






//ENUM DECLARATION for the CHARACTER XP LEVEL SYSTEM
UENUM(BlueprintType)
namespace ELevelXP
{
	enum Level
	{
		E_One			UMETA(DisplayName = "One"),
		E_Two			UMETA(DisplayName = "Two"),
		E_Three			UMETA(DisplayName = "Three"),
		E_Four			UMETA(DisplayName = "Four"),
		E_Five			UMETA(DisplayName = "Five"),
		E_Six			UMETA(DisplayName = "Six"),
	};
}
/**
 * 
 */
UCLASS()
class TRANSEND_API ATransendPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

public:

	//enum object for Level XP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XPLeveling)
	TEnumAsByte<ELevelXP::Level> CurrentLevel;

	//Current XP for the character level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XPLeveling)
	int32 CurrentXP;


	//Min XP for the current character level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XPLeveling)
	int32 MinXP;

	//Max XP for the current character level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XPLeveling)
	int32 MaxXP;

	//Holds the calculation of the percentage of XP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XPLeveling)
	float CurrentPercentage;

	//returns the XP level of the character
	ELevelXP::Level GetXPLevel();

	//Tells the Pawn what abilities it may have
	void LevelAbilities();

	//calculates the XP bar percentage
	void GetXPBarPercentage();


	//Function that the player has been killed and starts timer
	UFUNCTION(BlueprintNativeEvent, Category = Death)
	void KilledPlayer();

	//Spawns the player into the map
	UFUNCTION(BlueprintNativeEvent, Category = Death)
	void SpawnPlayer();

	//variable to store the Constructor helper blueprint for spawning character
	TSubclassOf<ATransendCharacter> Spawner;

	virtual void Tick(float DeltaSeconds) override;

	//respawn point variable for the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnPoint)
	AActor *RespawnPoint;
	
	
};
