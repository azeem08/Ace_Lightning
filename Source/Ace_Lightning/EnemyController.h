// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MeleeCharacter.h"
#include "MagicCharacter.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class ACE_LIGHTNING_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	
	//
	UFUNCTION()
	void										OnHearNoise( APawn *OtherActor, const FVector &Location, float Volume );
	UFUNCTION()
	void										OnSeePawn( APawn *OtherPawn );

private:

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

};
