// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/KillZVolume.h"
#include "TransendCharacter.h"
#include "TranscendKillVolume.generated.h"

/**
 * 
 */
UCLASS()
class TRANSEND_API ATranscendKillVolume : public AKillZVolume
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION()
	void OnCollision(AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	
};
