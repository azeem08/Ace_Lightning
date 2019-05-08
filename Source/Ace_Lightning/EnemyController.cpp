// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"
#include "Waypoint.h"

void AEnemyController::OnHearNoise( APawn* OtherActor, const FVector& Location, float Volume )
{
}

void AEnemyController::OnSeePawn( APawn* OtherPawn )
{
	if ( IsPlayer( OtherPawn ) )
	{
		MoveToActor( OtherPawn, 100.f );
	}
	else
	{
		MoveToActor( OtherPawn, 1.f );
	}
}
