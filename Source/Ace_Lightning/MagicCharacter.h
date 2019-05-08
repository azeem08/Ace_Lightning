// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ace_LightningCharacter.h"
#include "Sender.h"
#include "Receiver.h"
#include "MagicCharacter.generated.h"

/**
 *
 */
UCLASS()
class ACE_LIGHTNING_API AMagicCharacter : public AAce_LightningCharacter, public Sender, public Receiver
{
	GENERATED_BODY()

public:
	AMagicCharacter();
	// Magic value
	UPROPERTY( Category = Stats, EditAnywhere )
	float										CurrentMana;

	// Magic regen rate
	UPROPERTY( Category = Stats, EditAnywhere )
	float										ManaRegenRate;

	// Heal Amount
	UPROPERTY( Category = Stats, EditAnywhere )
	float										HealAmount;

	// Healing particle system
	UPROPERTY( Category = Stats, EditAnywhere )
	class UParticleSystemComponent*				FXHeal;

	// Mana pickup particle system
	UPROPERTY( Category = Stats, EditAnywhere )
	class UParticleSystemComponent*				FXManaPickUp;

	// Magic projectile attack
	UPROPERTY( Category = Stats, EditAnywhere )
	TSubclassOf<class AMagicProjectile>			BP_MagicProjectile;

	float										GetCurrentHealth();
	float										GetCurrentMana();

private:

	virtual void								BeginPlay() override;
	virtual void								SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
	virtual void								Tick( float DeltaTime ) override;

	virtual void								SendAMessage( EMessage message ) override;
	virtual void								SendAMessage( EMessage message, int value ) override;
	virtual void								SendAMessage( EMessage message, float value ) override {};
	virtual void								SendAMessage( EMessage message, FString title, FString details ) override {};
	virtual void								ReadMessage( EMessage message ) override {};
	virtual void								ReadMessage( EMessage message, int value ) override;
	virtual void								ReadMessage( EMessage message, float value ) override;
	virtual void								ReadMessage( EMessage message, FString title, FString details ) override {};

	void										StopAnimation( FTimerHandle& timer, EMessage message );
	void										Heal();
	void										MagicAttack();
	void										StopHealing();
	void										FireMagicProjectile();
	void										StopMagicProjectile();

	// needed to manage timer functions
	FTimerHandle								TimerHandle;
	FTimerHandle								AnimationTimer;

	// value representing full mana
	float										MaxMana;
	// amount of time the casting should take
	float										CastingTime;
	// Ability 1 mana cost
	float										HealManaCost;
	// Ability 2 mana cost
	float										MagicAttackManaCost;
	// checks if the message has already been sent
	bool										bHasSentMessage;

	// position of arm bone should never have to change
	const float									kRightHandHeight = 20.f;
	// time when the projectile should spawn should never have to change
	const float									kProjectileSpawnTime = 0.8f;
	// time when the fire projectile animation should end should never have to change
	const float									kAbility2Time = 2.f;
};
