// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetNameStringByIndex((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )
/**
 * 
 */
UENUM()
enum class EStats
{
	Health,
	Special,
	Level,
	Gold,
	XP
};

UENUM()
enum class EAbilities
{
	First,
	Second,
	Third,
	Fourth,
	Fifth,
	Sixth,
	Seventh,
	Eighth,
	Nineth,
	Tenth,
	Jumping
};

class ACE_LIGHTNING_API MessageClass
{
public:
	MessageClass();
	~MessageClass();
};
