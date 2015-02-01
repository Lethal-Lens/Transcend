// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TransendCharacter.h"
#include "TransendPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class TRANSEND_API ATransendPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = Death)
	void KilledPlayer();

	UFUNCTION(BlueprintNativeEvent, Category = Death)
	void SpawnPlayer();

	TSubclassOf<ATransendCharacter> Spawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnPoint)
	AActor *RespawnPoint;

	FTimerHandle MyHandle;
	
	
};
