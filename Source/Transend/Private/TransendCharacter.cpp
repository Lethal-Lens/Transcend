

#include "Transend.h"
#include "TransendCharacter.h"

ATransendCharacter::ATransendCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//set jump counter to 0
	JumpCounter = 0;

	//test line update


	WeaponInventory.SetNum(2, false);

	//let character sprint on startup
	bCanSprint = true;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SocketOffset = FVector(0.f,100.f,300.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("SideViewCamera"));
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2000.f;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1260.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	CollisionComp = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CollisionComp"));
	CollisionComp->AttachTo(RootComponent);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ATransendCharacter::OnCollision);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input


void ATransendCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//CUSTOM ACTION BINDINGS///////////////////////////////////////////////////////////////////////

		//Movement Inputs
	InputComponent->BindAction("Sprint", IE_Pressed, this, &ATransendCharacter::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ATransendCharacter::StopSprint);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &ATransendCharacter::StartCrouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &ATransendCharacter::EndCrouch);
		//Weapon Switching Inputs
	InputComponent->BindAction("NextWeapon", IE_Pressed, this, &ATransendCharacter::NextWeapon);
	InputComponent->BindAction("PrevWeapon", IE_Pressed, this, &ATransendCharacter::PrevWeapon);
		//Firing Inputs
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATransendCharacter::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ATransendCharacter::StopFire);

	//////////////////////////////////////////////////////////////////////////////////////////

	InputComponent->BindAxis("MoveRight", this, &ATransendCharacter::MoveRight);
}

//At the beginning of the game
void ATransendCharacter::BeginPlay()
{

	Super::BeginPlay();
	//Equip Default Weapons at the start of the game
	GiveDefaultWeapons();
}

// START OF FIRING WEAPON ON HOLD (IN CASE THERE IS RAPID FIRE)
void ATransendCharacter::StartFire()
{
	//if Player Cannot Fire
	if (!bCanFire)
	{
		//Turn on bCanFire
		bCanFire = true;
		//if We are currently using a weapon
		if (CurrentWeapon != NULL)
		{
			//Start firing the weapon
			CurrentWeapon->Fire();
		}
	}
}

//STOPS FIRING WEAPON ON RELEASE
void ATransendCharacter::StopFire()
{
	//if Player can fire
	if (bCanFire)
	{
		//turn bCanFire off!
		bCanFire = false;
		//If we are currently using a weapon
		if (CurrentWeapon != NULL)
		{
			//Stop firing current Weapon
			CurrentWeapon->StopFire();
		}
	}
}


//FUNCTION THAT GIVES THE PLAYER DEFAULT WEAPONS FROM THE START(REASON FOR NEXT LEVEL LOADING AND TESTING)
void ATransendCharacter::GiveDefaultWeapons()
{
	//make a for loop for the length of the WeaponInventory Array
	for (int32 i = 0; i < WeaponInventory.Num(); i++)
	{
		//if there is a slot and there is a weapon in the slot
		if (DefaultWeaponInventory[i] && DefaultWeaponInventory[i] != NULL)
		{
			//Spawn the Weapon
			AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponInventory[i]);
			if (Spawner)
			{
				//Store the weapon's information into the slot
				WeaponInventory[i] = Spawner;
				//ensure the PLAYER is the owner
				WeaponInventory[i]->SetOwningPawn(this);
				//Unequip the weapon since we are not using it
				WeaponInventory[i]->OnUnEquip();
			}
		}
	}
	//make the CurrentWeapon the primary
	CurrentWeapon = WeaponInventory[0];
	//Equip the Primary Weapon
	CurrentWeapon->OnEquip();
}

void ATransendCharacter::OnCollision(AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//pointer instance of the collided weapon if it is OtherActor
	AWeapon *Weapon = Cast<AWeapon>(OtherActor);

	//if player collides with a weapon
	if (Weapon)
	{
		//process the weapon
		ProcessWeaponPickup(Weapon);
	}
}

void ATransendCharacter::ProcessWeaponPickup(AWeapon *Weapon)
{
	//If there is a Weapon to be processed
	if (Weapon != NULL)
	{
		//Check to see if either the weapon inventory is empty
		if (WeaponInventory[0] == NULL || WeaponInventory[1] == NULL)
		{
			//spawn the picked up weapon
			AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Weapon->GetClass());
			if (Spawner)
			{
				//if the Weapon that we spawned is primary
				if (Spawner->WeapConfig.bIsPrimary)
				{
					//set the primary slot of the spawned weapon
					WeaponInventory[0] = Spawner;
				}
				else
				{
					//set the secondary slot of the spawned weapon
					WeaponInventory[1] = Spawner;
				}
			}
			//destroy the spawned weapon we touched(it's been stored ;) )
			Weapon->Destroy();
		}
		else
		{
			//check to see if the player has enough storage for the ammo

			//if the weapon is primary
			if (Weapon->WeapConfig.bIsPrimary)
			{
				//if the Names are equal
				if (Weapon->WeapConfig.Name == WeaponInventory[0]->WeapConfig.Name)
				{
					//math calculations to check if there is room for the ammo
					if (WeaponInventory[0]->CurrentAmmo >= 0 && Weapon->CurrentAmmo <= (WeaponInventory[0]->WeapConfig.MaxAmmo - WeaponInventory[0]->CurrentAmmo))
					{
						//if there is, add the ammo to the weapon
						WeaponInventory[0]->CurrentAmmo += Weapon->CurrentAmmo;
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Added " + FString::FromInt(Weapon->CurrentAmmo));
						//then destroy the weapon
						Weapon->Destroy();
					}
					else //if ammo is full
					{
						if (WeaponInventory[0]->CurrentAmmo > WeaponInventory[0]->WeapConfig.MaxAmmo)
						{
							WeaponInventory[0]->CurrentAmmo = WeaponInventory[0]->WeapConfig.MaxAmmo;
						}
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Full Ammo on " + WeaponInventory[0]->WeapConfig.Name);
					}
				}
			}
			else // if secondary, do the same as above
			{
				if (Weapon->WeapConfig.Name == WeaponInventory[1]->WeapConfig.Name)
				{
					if (WeaponInventory[1]->CurrentAmmo >= 0 && Weapon->CurrentAmmo <= (WeaponInventory[1]->WeapConfig.MaxAmmo - WeaponInventory[1]->CurrentAmmo))
					{
						WeaponInventory[1]->CurrentAmmo += Weapon->CurrentAmmo;
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Added " + FString::FromInt(Weapon->CurrentAmmo));
						Weapon->Destroy();
					}
					else //if ammo is full
					{
						if (WeaponInventory[1]->CurrentAmmo > WeaponInventory[1]->WeapConfig.MaxAmmo)
						{
							WeaponInventory[1]->CurrentAmmo = WeaponInventory[1]->WeapConfig.MaxAmmo;
						}
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Full Ammo on " + WeaponInventory[1]->WeapConfig.Name);
					}
				}
			}
		}
	}
}

//FUNCTION TO TRIGGER NEXT WEAPON WITH MOUSE WHEEL UP
void ATransendCharacter::NextWeapon()
{
	//if we have a current weapon and if the current weapon is primary
	if (CurrentWeapon != NULL && CurrentWeapon->WeapConfig.bIsPrimary)
	{
		//equip the secondary weapon
		EquipWeapon(WeaponInventory[1]);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Secondary");
	}
	//if we have a current weapon and if the current weapon is secondary
	else
	{
		//equip the primary
		EquipWeapon(WeaponInventory[0]);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Primary");
	}
}

//FUNCTION TO TRIGGER NEXT WEAPON WITH MOUSE WHEEL DOWN
void ATransendCharacter::PrevWeapon()
{
	if (CurrentWeapon != NULL && CurrentWeapon->WeapConfig.bIsPrimary)
	{
		EquipWeapon(WeaponInventory[1]);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Secondary");
	}
	else
	{
		EquipWeapon(WeaponInventory[0]);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Primary");
	}
}

void ATransendCharacter::EquipWeapon(AWeapon *Weapon)
{
	//call the current weapon's unequip function
	CurrentWeapon->OnUnEquip();
	//the new current weapon is the Weapon Parameter we dumped from the WeaponInventory slot we called
	CurrentWeapon = Weapon;
	//Equip the new current weapon
	CurrentWeapon->OnEquip();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "NEW");
}

void ATransendCharacter::Tick(float DeltaSeconds)
{
	//if the player is sprinting
	if (bIsSprinting)
	{ 
		//increase the movement speed overtime
		GetCharacterMovement()->MaxWalkSpeed += SprintIncrement;

		//if the movement speed is faster than the max sprint speed
		if (GetCharacterMovement()->MaxWalkSpeed >= SprintSpeed)
		{
			//cap it at SprintSpeed
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
	}
	else
	{
		//decrease the movement speed overtime
		GetCharacterMovement()->MaxWalkSpeed -= SprintDecline;

		//if the movement speed is slower than the Walking speed
		if (GetCharacterMovement()->MaxWalkSpeed <= WalkingSpeed)
		{
			//cap it at WalkingSpeed
			GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
		}
	}
}

void ATransendCharacter::StartCrouch()
{
	if (!bCanCrouch)
	{
		bCanCrouch = true;
		GetCapsuleComponent()->SetCapsuleHalfHeight(48.0f);				//make the capsulecollision half the height of the character
		GetMesh()->SetRelativeLocation(FVector(0, 0, -49));				//set the mesh with the new center alignment of the capsulecollision
		this->LaunchCharacter(FVector(0, 0, -48), false, false);	//launch the character down to the ground to ensure crouch is to the ground *bug fix*
	}
}

void ATransendCharacter::EndCrouch()
{
	if (bCanCrouch)
	{
		bCanCrouch = false;
		GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);					//sets capsulecollision to normal height
		this->LaunchCharacter(FVector(0, 0, 48), false, false);			//launches character back on top of the mesh *bug fix*
		GetMesh()->SetRelativeLocation(FVector(0, 0, -97));					//set the mesh location with the center of the capsule collision
	}
}

void ATransendCharacter::Jump()
{
	Super::Jump();

	//if double jump is enabled (Level 2)
	if (bDoubleJumpEnabled)
	{
		//as long as JumpCounter is less than 2
		if (JumpCounter < 2)
		{
			//On the first jump
			if (JumpCounter == 0 && !GetCharacterMovement()->IsFalling())
			{
				//simply jump
				bPressedJump = true;
				JumpKeyHoldTime = 0.1f;
				JumpCounter++;
				bNowJumping = true;
			}
			//On the second jump
			if (JumpCounter == 1 && GetCharacterMovement()->IsFalling())
			{
				//simulate jump by launching character
				JumpKeyHoldTime = 0.8f;
				this->LaunchCharacter(FVector(0, 0, DoubleJumpHeight), false, true);
				JumpCounter++;
			}
		}
		//if greater than 2
		else
		{
			StopJumping();
			bNowJumping = false;
		}
	}
	//if double jump is disabled
	else
	{
		//as long as JumpCounter is less than 1
		if (JumpCounter < 1)
		{
			//On the first jump
			if (JumpCounter == 0 && !GetCharacterMovement()->IsFalling())
			{
				//simply jump
				bPressedJump = true;
				JumpKeyHoldTime = 0.1f;
				JumpCounter++;
				bNowJumping = true;
			}
		}
		//if greater than 2
		else
		{
			StopJumping();
			bNowJumping = false;
		}
	}

}

void ATransendCharacter::StopJumping()
{
	Super::StopJumping();
	bNowJumping = false;
}

void ATransendCharacter::OnLanded(const FHitResult& Hit)
{
	Super::OnLanded(Hit);
	
	//When landed, reset the counters and key hold time
	JumpCounter = 0;
	JumpKeyHoldTime = 0.0f;
}

void ATransendCharacter::StartSprint()
{
	//if character can sprint and not on cooldown and if sprinting is enabled (Level 2)
	if (bCanSprint && !bSprintCooldown && bSprintEnabled)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Sprinting");
		bIsSprinting = true;
		//this is a timer that is dictated by the MaxSprintTime variable. once the timer gets to that variable value, it will then call the function SprintCoolDown
		GetWorldTimerManager().SetTimer(this, &ATransendCharacter::SprintCooldown, MaxSprintTime, false); 
	}
}

void ATransendCharacter::StopSprint()
{
	//as long as the character isnt on cooldown and if sprinting is enabled (Level 2)
	if (!bSprintCooldown && bSprintEnabled)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Not Sprinting");
		bIsSprinting = false;
		//Reason for the timer clear is to make sure the timer isnt running when we are not sprinting. 
		GetWorldTimerManager().ClearTimer(this, &ATransendCharacter::SprintCooldown);
	}
}

void ATransendCharacter::SprintCooldown()
{
	bSprintCooldown = true;
	bCanSprint = false;
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "CoolingDown");
	GetWorldTimerManager().SetTimer(this, &ATransendCharacter::StopSprintCooldown, CooldownTime, false);
}

void ATransendCharacter::StopSprintCooldown()
{
	bCanSprint = true;
	bSprintCooldown = false;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Can Run Again");
}

void ATransendCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

