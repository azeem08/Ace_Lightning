// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveData.generated.h"

/**
 * 
 */
UCLASS()
class ACE_LIGHTNING_API USaveData : public USaveGame
{
	GENERATED_BODY()

public:
	// xp progress to bind to the UI
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	float					XP_Progress;
	// how much experience the player has
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	float					CurrentXP;
	// how much experience the player needs to level up
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	float					MaxXP;
	// what level the player is
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	int						CurrentLevel;
	// how much gold the player has
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	int						GoldValue;
};
