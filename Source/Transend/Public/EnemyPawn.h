// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "EnemyPawn.generated.h"

/**
 * 
 */
UCLASS()
class TRANSEND_API AEnemyPawn : public APawn
{
	GENERATED_UCLASS_BODY()

	//Skeletal Mesh for the Pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent *Mesh;

	//Collision for the AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	UBoxComponent *BoxCollision;

	//Function for base death data
	virtual void Death();

	//Plays Sounds for enemy
	UAudioComponent *PlayEnemySound(USoundCue *Sound);

};
