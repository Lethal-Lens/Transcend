

#pragma once

#include "GameFramework/Actor.h"
#include "WeaponProjectile.h"
#include "Weapon.generated.h"

//CUSTOM TRACE CHANNEL FOR THE INSTANT HIT PORTION
#define TRACE_WEAPON ECC_GameTraceChannel1

/**
 * 
 */

//forward declaration of character. couldn't use #include "TransendCharacter.h" because of circular dependancy
class ATransendCharacter;

//enum of Projectile types. Can be selected in blueprint for Gameplay adjustments
UENUM(BlueprintType)
namespace EProjectile
{
	enum Type
	{
		E_Bullet			UMETA(DisplayName = "Bullet"),
		E_Projectile		UMETA(DisplayName = "Projectile"),
		E_Melee				UMETA(DisplayName = "Melee"),
	};
}

//Struct to hold base variables for the weapon
USTRUCT(BlueprintType)
struct FWeaponConfig
{
	GENERATED_USTRUCT_BODY()

	//Max Ammo of weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 MaxAmmo;

	//Max Clip of weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 MaxClip;

	//Range of the weapon's projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponRange;

	//Time between shots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float TimeBetweenShots;

	//How much does each shot cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 ShotCost;

	//Spread of the weapon for each shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponSpread;

	//Name of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FString Name;

	//Splash art for the UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UTexture2D* SplashArt;

	//Check to see if it's the primary weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	bool bIsPrimary;

	//Checks to see if weapon is equipped
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	bool bIsEquipped;
};


UCLASS()
class TRANSEND_API AWeapon : public AActor
{
	GENERATED_UCLASS_BODY()
	
	//Mesh of the weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent *Mesh;

	//Dynamic collision for the weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UBoxComponent *CollisionComp;

	//Fire Sound for weapon
	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue* FireSound;

	//Weapon Projectile(Like rocket, grenade, etc)
	UPROPERTY(EditDefaultsOnly, Category = Config)
	TSubclassOf<AWeaponProjectile> WeaponProj;

	//Struct object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FWeaponConfig WeapConfig;

	//enum object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	TEnumAsByte<EProjectile::Type> ProjType;

	//Current Ammo for weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 CurrentAmmo;

	//Current Ammo in clip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 CurrentClip;
	
	bool bWantsToFire;

	void Fire();
	void StopFire();
	void ReloadAmmo();

	//virtual fire functions. reason for virtual is in case if the firing function behaves different
	virtual void Instant_Fire();
	virtual void ProjectileFire();
	virtual void MeleeFire();

	//retrieves owner for this weapon
	void SetOwningPawn(ATransendCharacter *NewOwner);

	//attaches weapon to player
	void AttachtoPlayer();
	//detaches weapon from player
	void DetachFromPlayer();

	//Things for the weapon to do when equipping
	void OnEquip();
	//things for the weapon to do when unequipping
	void OnUnEquip();

	UAudioComponent* PlayWeaponSound(USoundCue *Sound);

protected:

	//retrieves the trace line points between the socket and the hit object
	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	//processes the instant hit trace
	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

	//a pointer instance to store the Character class
	ATransendCharacter *MyPawn;
};
