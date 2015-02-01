// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyPawn.h"
#include "TransendPlayerController.h"
#include "Rabbit.generated.h"

/**
 * 
 */
UCLASS()
class TRANSEND_API ARabbit : public AEnemyPawn
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue *DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	AActor *SpawnLocation;

	virtual void Death() override;
	
};
