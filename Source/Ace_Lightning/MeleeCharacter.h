// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ace_LightningCharacter.h"
#include "MessageClass.h"
#include "MeleeCharacter.generated.h"

/**
 *
 */
UCLASS()
class ACE_LIGHTNING_API AMeleeCharacter : public AAce_LightningCharacter
{
	GENERATED_BODY()

public:
												AMeleeCharacter();

	// Sword object
	UPROPERTY( Category = Stats, EditAnywhere )
	TSubclassOf<class AWeapon_Sword>			BP_Sword;

private:

	virtual void								BeginPlay() override;
	virtual void								SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
	virtual void								Tick( float DeltaTime ) override;
	virtual void								Ability1() override;

	void										StopSlashing();

	// needed to manage timer functions
	FTimerHandle								AnimationTimer;
	// Ability 1 mana cost
	float										SwordAttackCost;

	// time of sword slah animation should never have to change
	const float									kSwordSlashTime = 1.5f;
};
