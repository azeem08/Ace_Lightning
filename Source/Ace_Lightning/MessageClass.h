// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM()
enum class EMessage
{
	NoMessage,
	IsJumping,
	StoppedJumping,
	AbilityAvailable,
	AbilityNotAvailable,
	NoSpecial,
	CastAbility1,
	StopAbility1,
	CastAbility2,
	StopAbility2,
	LootPickUp,
	HealthPickUp,
	SpecialPickUp,
	NewQuest,
	EnemyKilled
};

class ACE_LIGHTNING_API MessageClass
{
public:
	MessageClass();
	~MessageClass();
};
