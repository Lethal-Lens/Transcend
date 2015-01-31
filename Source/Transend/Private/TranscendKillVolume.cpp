// Fill out your copyright notice in the Description page of Project Settings.

#include "Transend.h"
#include "TranscendKillVolume.h"


ATranscendKillVolume::ATranscendKillVolume(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	BrushComponent->OnComponentBeginOverlap.AddDynamic(this, &ATranscendKillVolume::OnCollision);
}

void ATranscendKillVolume::OnCollision(AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	ATransendCharacter *Character = Cast<ATransendCharacter>(OtherActor);
	
	if (Character)
	{
		FVector Spawner = Character->RespawnPoint->GetActorLocation();
		Character->SetActorLocation(Spawner);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I DIED!");
	}
}