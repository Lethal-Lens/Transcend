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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent *Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	UBoxComponent *BoxCollision;

	virtual void Death();

	UAudioComponent *PlayEnemySound(USoundCue *Sound);

};
