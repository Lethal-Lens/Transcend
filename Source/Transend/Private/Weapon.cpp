

#include "Transend.h"
#include "TransendCharacter.h"
#include "Weapon.h"

AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "Mesh");
	RootComponent = Mesh;

	CollisionComp = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "CollisionComp");
	CollisionComp->AttachParent = Mesh;
}

void AWeapon::Fire()
{
	//if the player isnt firing
	if (!bWantsToFire)
	{
		//fire weapon
		bWantsToFire = true;
		//TIMER HANDLER FOR TIME BETWEEN SHOTS(WILL BE IN NEXT BUILD)
		FTimerHandle MyHandle;
		switch (ProjType)
		{
		case EProjectile::E_Bullet:
			Instant_Fire();
			break;
		case EProjectile::E_Projectile:
			ProjectileFire();
			break;
		case EProjectile::E_Melee:
			MeleeFire();
		default:
			break;
		}
	}
	else
	{

	}
}

void AWeapon::StopFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		GetWorldTimerManager().ClearTimer(this, &AWeapon::Instant_Fire);
		GetWorldTimerManager().ClearTimer(this, &AWeapon::ProjectileFire);
	}
}

void AWeapon::OnEquip()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachtoPlayer();
	WeapConfig.bIsEquipped = true;
}

void AWeapon::OnUnEquip()
{
	DetachFromPlayer();
	WeapConfig.bIsEquipped = false;
}

void AWeapon::AttachtoPlayer()
{
	if (MyPawn)
	{
		DetachFromPlayer();

		USkeletalMeshComponent *Character = MyPawn->GetMesh();
		Mesh->SetHiddenInGame(false);
		Mesh->AttachTo(Character, "WeapSocket");

	}
}

void AWeapon::DetachFromPlayer()
{
	Mesh->DetachFromParent();
	Mesh->SetHiddenInGame(true);
}

void AWeapon::SetOwningPawn(ATransendCharacter *NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

void AWeapon::ReloadAmmo()
{
	if (CurrentAmmo > 0)
	{
		if (CurrentAmmo < WeapConfig.MaxClip)
		{
			CurrentClip = CurrentAmmo;
		}
		else
		{
			CurrentAmmo -= WeapConfig.MaxClip;
			CurrentClip += WeapConfig.MaxClip;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "No Ammo Left");
	}
}

FHitResult AWeapon::WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingle(Hit, TraceFrom, TraceTo, TRACE_WEAPON, TraceParams);

	return Hit;
}

void AWeapon::ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread)
{
	const FVector EndTrace = Origin + ShootDir * WeapConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
	DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Black, true, 10000, 10.f);

}

void AWeapon::Instant_Fire()
{
	if (CurrentClip > 0)
	{
		// Get the camera transform
		FVector CameraLoc;
		FRotator CameraRot;
		GetActorEyesViewPoint(CameraLoc, CameraRot);
		const int32 RandomSeed = FMath::Rand();
		FRandomStream WeaponRandomStream(RandomSeed);
		const float CurrentSpread = WeapConfig.WeaponSpread;
		const float SpreadCone = FMath::DegreesToRadians(WeapConfig.WeaponSpread * 0.5);
		const FVector AimDir = Mesh->GetSocketRotation("MF").Vector();
		const FVector StartTrace = Mesh->GetSocketLocation("MF");
		const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
		const FVector EndTrace = StartTrace + ShootDir * WeapConfig.WeaponRange;
		const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

		ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);
		CurrentClip -= WeapConfig.ShotCost;

		GetWorldTimerManager().SetTimer(this, &AWeapon::Instant_Fire, WeapConfig.TimeBetweenShots, false);
		PlayWeaponSound(FireSound);
	}
	else
	{
		ReloadAmmo();
	}
}

void AWeapon::ProjectileFire()
{
	if (CurrentClip > 0)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		// spawn the projectile at the muzzle
		AWeaponProjectile* const Projectile = GetWorld()->SpawnActor<AWeaponProjectile>(WeaponProj, Mesh->GetSocketLocation("MF"), Mesh->GetSocketRotation("MF"), SpawnParams);
		if (Projectile)
		{

		}
		CurrentClip -= WeapConfig.ShotCost;
		GetWorldTimerManager().SetTimer(this, &AWeapon::ProjectileFire, WeapConfig.TimeBetweenShots, false);
		PlayWeaponSound(FireSound);
	}
	else
	{
		ReloadAmmo();
	}
}

void AWeapon::MeleeFire()
{

}

UAudioComponent* AWeapon::PlayWeaponSound(USoundCue *Sound)
{
	UAudioComponent* AC = NULL;
	if (Sound && MyPawn)
	{
		AC = UGameplayStatics::PlaySoundAttached(Sound, MyPawn->GetRootComponent());
	}

	return AC;
}