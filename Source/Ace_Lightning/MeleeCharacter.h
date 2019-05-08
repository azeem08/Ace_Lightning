// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ace_LightningCharacter.h"
#include "Sender.h"
#include "Receiver.h"
#include "MeleeCharacter.generated.h"

/**
 *
 */
UCLASS()
class ACE_LIGHTNING_API AMeleeCharacter : public AAce_LightningCharacter, public Sender, public Receiver
{
	GENERATED_BODY()

public:
	AMeleeCharacter();
	// Stamina value
	UPROPERTY( Category = Stats, EditAnywhere )
	float										CurrentStamina;

	// Stamina regen rate
	UPROPERTY( Category = Stats, EditAnywhere )
	float										StaminaRegenRate;

	// Stamina pickup particle system
	UPROPERTY( Category = Stats, EditAnywhere )
	class UParticleSystemComponent*				FXStaminaPickUp;

	// Sword object
	UPROPERTY( Category = Stats, EditAnywhere )
	TSubclassOf<class AWeapon_Sword>			BP_Sword;

	float										GetCurrentHealth();
	float										GetCurrentStamina();

private:

	virtual void								BeginPlay() override;
	virtual void								SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
	virtual void								Tick( float DeltaTime ) override;

	virtual void								SendAMessage( EMessage message ) override;
	virtual void								SendAMessage( EMessage message, int value ) override;
	virtual void								SendAMessage( EMessage message, float value ) override {};
	virtual void								SendAMessage( EMessage message, FString title, FString details ) override {};
	virtual void								ReadMessage( EMessage message ) override;
	virtual void								ReadMessage( EMessage message, int value ) override;
	virtual void								ReadMessage( EMessage message, float value ) override;
	virtual void								ReadMessage( EMessage message, FString title, FString details ) override {};

	void										StopAnimation( FTimerHandle& timer, EMessage message );
	void										SwordSlash();
	void										StopSlashing();

	// needed to manage timer functions
	FTimerHandle								AnimationTimer;

	// value representing full stamina
	float										MaxStamina;
	// Ability 1 mana cost
	float										SwordAttackCost;
	// checks if the message has already been sent
	bool										bHasSentMessage;

	// time of sword slah animation should never have to change
	const float									kSwordSlashTime = 1.5f;
};
