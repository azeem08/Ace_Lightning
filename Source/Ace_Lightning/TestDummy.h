// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ace_LightningGameMode.h"
#include "GameFramework/Character.h"
#include "MagicProjectile.h"   
#include "Weapon_Sword.h"
#include "TestDummy.generated.h"

UCLASS()
class ACE_LIGHTNING_API ATestDummy : public ACharacter
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
	// Delegate function for when an ability is not active
	UFUNCTION()
	void										DeactiveAbility( EAbilities ability );

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

	// pointer to the game mode
	class AAce_LightningGameMode*				GameMode;
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
