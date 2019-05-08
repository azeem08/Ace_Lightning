// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ace_LightningGameMode.h"
#include "GameFramework/Character.h"
#include "MagicProjectile.h"   
#include "MagicCharacter.h"
#include "MeleeCharacter.h"
#include "Weapon_Sword.h"
#include "Receiver.h"
#include "Sender.h"
#include "TestDummy.generated.h"

UCLASS()
class ACE_LIGHTNING_API ATestDummy : public ACharacter, public Receiver, public Sender
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
												ATestDummy();

	// Health value
	UPROPERTY( Category = Stats, EditAnywhere )
	float										CurrentHealth;

	// cylinder collider
	UPROPERTY( Category = Stats, EditAnywhere, BlueprintReadWrite )
	class UCapsuleComponent*					CollisionVolume;

	// public variables that are bound to the animation blueprint
	// this allows it to be changed anywhere in code
	UPROPERTY( Category = Stats, EditAnywhere, BlueprintReadWrite )
	class UPawnSensingComponent*				PawnSensor;

	// Damage VFX
	UPROPERTY( Category = Stats, EditAnywhere )
	class UParticleSystemComponent*				FXDamage;

	// how much the character rotates
	UPROPERTY( Category = Stats, EditAnywhere )
	int											TurnSpeed;

	// how much XP the character is worth
	UPROPERTY( Category = Stats, EditAnywhere )
	float										XP;

	// Loot drop blueprint instance
	UPROPERTY( Category = Stats, EditAnywhere )
	TSubclassOf<class ALootDrop>				BP_LootDrop;


private:
	// Called when the game starts or when spawned
	virtual void								BeginPlay() override;
	// Called every frame
	virtual void								Tick( float DeltaTime ) override;
	UFUNCTION()
	void										OnOverlapBegin( AActor * overlappedActor, AActor * otherActor );
	void										Dead();
	UFUNCTION()
	void										OnHearNoise( APawn *OtherActor, const FVector &Location, float Volume );
	UFUNCTION()
	void										OnSeePawn( APawn *OtherPawn );

	virtual void								SendAMessage( EMessage message ) override {};
	virtual void								SendAMessage( EMessage message, int value ) override {};
	virtual void								SendAMessage( EMessage message, float value ) override;
	virtual void								SendAMessage( EMessage message, FString title, FString details ) override {};
	virtual void								ReadMessage( EMessage message ) override;
	virtual void								ReadMessage( EMessage message, int value ) override {};
	virtual void								ReadMessage( EMessage message, float value ) override {};
	virtual void								ReadMessage( EMessage message, FString title, FString details ) override {};

	// Template that takes an actor and returns a pointer to the correct character type
	template <class ObjectType>
	float GetDamage( ObjectType* a )
	{
		AMagicProjectile* magicProjectile = nullptr;
		AWeapon_Sword* sword = nullptr;
		float damage = 0.f;

		if ( a->IsA( AMagicProjectile::StaticClass() ) )
		{
			magicProjectile = Cast<AMagicProjectile>( a );
			damage = magicProjectile->Damage;
			magicProjectile->Destroy();
		}
		else if ( a->IsA( AWeapon_Sword::StaticClass() ) )
		{
			sword = Cast<AWeapon_Sword>( a );
			if ( sword->IsActive() )
			{
				damage = sword->Damage;
			}
		}

		return damage;
	}

	// Template that takes an actor and returns a bool if it is a player
	template <class PlayerType>
	bool IsPlayer( PlayerType* a )
	{
		bool player = false;

		if ( a->IsA( AMeleeCharacter::StaticClass() ) )
		{
			player = true;
		}
		else if ( a->IsA( AMagicCharacter::StaticClass() ) )
		{
			player = true;
		}

		return player;
	}

	// pointer to the game mode
	AAce_LightningGameMode*						GameMode;
	// pointer to the controller class
	class AEnemyController*						AIManager;
	// death timer
	FTimerHandle								TimerHandle;
	// Damage taken
	float										DamageTaken;
	// checks if the object is still in the trigger volume
	bool										bIsOverlapping;
	//
	bool										bIsLookingAtPlayer;

	// position of arm bone should never have to change
	const float									kFloorHeight = 40.f;
	// duration of damage vfx should never change
	const float									kDamageTime = 1.5f;
};
