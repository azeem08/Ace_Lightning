// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Ace_LightningGameMode.h"
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Ace_LightningCharacter.generated.h"

UCLASS(config=Game)
class AAce_LightningCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent*					CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent*						FollowCamera;

public:
												AAce_LightningCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float										BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float										BaseLookUpRate;

	// Health value
	UPROPERTY( Category = Stats, EditAnywhere )
	float										CurrentHealth;

	// Health regen rate
	UPROPERTY( Category = Stats, EditAnywhere )
	float										HealthRegenRate;

	// Health pickup particle system
	UPROPERTY( Category = FX, EditAnywhere )
	class UParticleSystemComponent*				FXHealthPickUp;

	// Loot pickup particle system
	UPROPERTY( Category = FX, EditAnywhere )
	class UParticleSystemComponent*				FXLootPickUp;

	// Checks if the current cooldown of each ability
	bool										HasCooledDown( int ability );
	// Checks if the character is animating
	bool										IsAnimating();

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	virtual void								BeginPlay();
	virtual void								Tick( float DeltaTime );

	// pointer to the game mode
	AAce_LightningGameMode*						GameMode;
	// pointer to the movement component
	UCharacterMovementComponent*				MovementComponent;
	// Array of bools to check if ability has cooled down
	TArray<bool>								bAbilityHasCooledDown;
	// Check if the character attack animation has finished
	bool										bHasFinishedAnimating;
	// value representing full health
	float										MaxHealth;

	// empty value should never have to change
	const float									kEmptyValue = 0.f;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent );
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

