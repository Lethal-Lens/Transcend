
#pragma once
#include "GameFramework/Character.h"
#include "Weapon.h"
#include "TransendCharacter.generated.h"


UENUM(BlueprintType)
enum EAnimStates
{
	E_Idle			UMETA(DisplayName = "Idle"),
	E_Walking		UMETA(DisplayName = "Walking"),
	E_Crouching		UMETA(DisplayName = "Crouching"),
	E_Firing		UMETA(DisplayName = "Firing"),
	E_Reloading		UMETA(DisplayName = "Reloading"),
	E_Jumping		UMETA(DisplayName = "Jumping"),
};

UCLASS(config=Game)
class ATransendCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	class UBoxComponent *CollisionComp;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:

	/** Called for side to side input */
	void MoveRight(float Val);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomCharacterMovement)
	TEnumAsByte<EAnimStates> CurrentState;

	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void OnLanded(const FHitResult& Hit) override;

	void StartSprint();
	void StopSprint();

	void SprintCooldown();
	void StopSprintCooldown();

	//start of crouch
	UFUNCTION(BlueprintCallable, Category = Crouching)
	virtual void StartCrouch();

	//end of crouch
	UFUNCTION(BlueprintCallable, Category = Crouching)
	virtual void EndCrouch();

	//value to adjust double jump height
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomCharacterMovement)
	float DoubleJumpHeight;

	//checks if the player is high enough level to double jump
	bool bDoubleJumpEnabled;

	bool bSprintEnabled;

	//Current Health of the Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	int32 Health;

	//Maximum Health for the Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	int32 HealthMax;

	//Returns The percentage of health for the health bar UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float CurrentHealthPercentage;

	//function that updates the health
	void SetHealthBarUpdate();

	//returns the amount of health 
	int32 GetHealth();

	//bool checks to see if sprinting and if cooling down
	bool bCanSprint;
	bool bSprintCooldown;
	bool bIsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomCharacterMovement)
	bool bCanCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomCharacterMovement)
	bool bNowJumping;
	
	//Sets the sprint speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomCharacterMovement)
	float SprintSpeed;

	//sets the Walking speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterMovement)
	float WalkingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterMovement)
	float JumpHoldTime;

	//Incline on how fast the player will go from WalkingSpeed to SprintSpeed
	//The higher the number, the faster the player will get to SprintSpeed. Lower the number, the slower the player will get to SprintSpeed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomCharacterMovement)
	float SprintIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomCharacterMovement)
	float SprintDecline;

	//Sets the cooldown timer for sprint exhaustion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomCharacterMovement)
	float CooldownTime;

	//sets the max time the character can sprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomCharacterMovement)
	float MaxSprintTime;

	//counter for double jump
	int32 JumpCounter;

	//Update function for every frame
	virtual void Tick(float DeltaSeconds) override;

	//holds the player's current weapon instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	AWeapon* CurrentWeapon;

	//Gives default inventory at the start of the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TArray<TSubclassOf<AWeapon>> DefaultWeaponInventory;

	//Weapon Inventory for the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TArray<AWeapon*> WeaponInventory;

	//bool to enable on hold fire and release fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool bCanFire;

	//gives player default weapons on start
	void GiveDefaultWeapons();

	//Tells Inventory if that weapon is already in inventory. If it is, the function will then add ammo. If ammo is full, it will then not process the pickup
	void ProcessWeaponPickup(AWeapon *Weapon);

	//gets the next weapon in the inventory
	void NextWeapon();

	//gets previous weapon in the inventory
	void PrevWeapon();

	//Equips weapon
	void EquipWeapon(AWeapon *Weapon);

	UFUNCTION(BlueprintCallable, Category = Event)
	virtual void BeginPlay() override;

protected:


	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	/** Fires Current Weapon */
	void StartFire();

	void StopFire();

	UFUNCTION()
	void OnCollision(AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:
	ATransendCharacter(const FObjectInitializer& ObjectInitializer);

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

};
