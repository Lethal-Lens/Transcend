// Fill out your copyright notice in the Description page of Project Settings.

#include "Transend.h"
#include "TranscendKillVolume.h"
#include "TransendPlayerController.h"


ATranscendKillVolume::ATranscendKillVolume(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	GetBrushComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATranscendKillVolume::OnCollision);
}

void ATranscendKillVolume::OnCollision(AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	ATransendCharacter *Character = Cast<ATransendCharacter>(OtherActor);
	
	if (Character)
	{
		Character->Destroy();
		ATransendPlayerController *PC = (ATransendPlayerController*)GetWorld()->GetFirstPlayerController();
		PC->KilledPlayer();
	}
}