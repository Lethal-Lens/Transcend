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
		E_01			UMETA(DisplayName = "1"),
		E_02			UMETA(DisplayName = "2"),
		E_03			UMETA(DisplayName = "3"),
		E_04			UMETA(DisplayName = "4"),
		E_05			UMETA(DisplayName = "5"),
		E_06			UMETA(DisplayName = "6"),
		E_07			UMETA(DisplayName = "7"),
		E_08			UMETA(DisplayName = "8"),
		E_09			UMETA(DisplayName = "9"),
		E_10			UMETA(DisplayName = "10"),
		E_11			UMETA(DisplayName = "11"),
		E_12			UMETA(DisplayName = "12"),
		E_13			UMETA(DisplayName = "13"),
		E_14			UMETA(DisplayName = "14"),
		E_15			UMETA(DisplayName = "15"),		//First Graphical Change
		E_16			UMETA(DisplayName = "16"),
		E_17			UMETA(DisplayName = "17"),
		E_18			UMETA(DisplayName = "18"),
		E_19			UMETA(DisplayName = "19"),
		E_20			UMETA(DisplayName = "20"),
		E_21			UMETA(DisplayName = "21"),
		E_22			UMETA(DisplayName = "22"),
		E_23			UMETA(DisplayName = "23"),
		E_24			UMETA(DisplayName = "24"),
		E_25			UMETA(DisplayName = "25"),
		E_26			UMETA(DisplayName = "26"),
		E_27			UMETA(DisplayName = "27"),
		E_28			UMETA(DisplayName = "28"),
		E_29			UMETA(DisplayName = "29"),
		E_30			UMETA(DisplayName = "30"),		//Second Graphical Change
		E_31			UMETA(DisplayName = "31"),
		E_32			UMETA(DisplayName = "32"),
		E_33			UMETA(DisplayName = "33"),
		E_34			UMETA(DisplayName = "34"),
		E_35			UMETA(DisplayName = "35"),
		E_36			UMETA(DisplayName = "36"),
		E_37			UMETA(DisplayName = "37"),
		E_38			UMETA(DisplayName = "38"),
		E_39			UMETA(DisplayName = "39"),
		E_40			UMETA(DisplayName = "40"),
		E_41			UMETA(DisplayName = "41"),
		E_42			UMETA(DisplayName = "42"),
		E_43			UMETA(DisplayName = "43"),
		E_44			UMETA(DisplayName = "44"),
		E_45			UMETA(DisplayName = "45"),
		E_46			UMETA(DisplayName = "46"),
		E_47			UMETA(DisplayName = "47"),
		E_48			UMETA(DisplayName = "48"),
		E_49			UMETA(DisplayName = "49"),
		E_50			UMETA(DisplayName = "50"),		//Final Graphical Change
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
