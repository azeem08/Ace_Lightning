// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ace_LightningCharacter.h"
#include "MessageClass.h"
#include "MagicCharacter.generated.h"

/**
 *
 */
UCLASS()
class ACE_LIGHTNING_API AMagicCharacter : public AAce_LightningCharacter
{
	GENERATED_BODY()

public:
	AMagicCharacter();

	// Heal Amount
	UPROPERTY( Category = Stats, EditAnywhere )
	float										HealAmount;

	// Healing particle system
	UPROPERTY( Category = Stats, EditAnywhere )
	class UParticleSystemComponent*				FXHeal;

	// Magic projectile attack
	UPROPERTY( Category = Stats, EditAnywhere )
	TSubclassOf<class AMagicProjectile>			BP_MagicProjectile;

private:

	virtual void								BeginPlay() override;
	virtual void								SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
	virtual void								Tick( float DeltaTime ) override;
	virtual void								Ability1() override;
	virtual void								Ability2() override;

	void										StopHealing();
	void										FireMagicProjectile();
	void										StopMagicProjectile();

	// needed to manage timer functions
	FTimerHandle								TimerHandle;
	FTimerHandle								AnimationTimer;
	// amount of time the casting should take
	float										CastingTime;
	// Ability 1 mana cost
	float										HealManaCost;
	// Ability 2 mana cost
	float										MagicAttackManaCost;

	// position of arm bone should never have to change
	const float									kRightHandHeight = 20.f;
	// time when the projectile should spawn should never have to change
	const float									kProjectileSpawnTime = 0.8f;
	// time when the fire projectile animation should end should never have to change
	const float									kAbility2Time = 2.f;
};
