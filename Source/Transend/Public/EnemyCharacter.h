// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TRANSEND_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue *DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
		AActor *SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		uint8 CurrentHP;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
		uint8 MaxHP;

	virtual void Tick(float DeltaSeconds) override;

	//Function for base death data
	virtual void Death();
	
	
};
