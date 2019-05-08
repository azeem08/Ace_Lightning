// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Waypoint.generated.h"

UCLASS()
class ACE_LIGHTNING_API AWaypoint : public APawn
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
												AWaypoint();

	// Mesh so there is a visual indication of where waypoints will be in the scene
	UPROPERTY( Category = Stats, EditAnywhere)
	class UStaticMeshComponent*					Mesh;

	// ID to differentiate between groups of waypoints
	UPROPERTY( Category = Stats, EditAnywhere)
	int											ID;

private:
	// Called when the game starts or when spawned
	virtual void								BeginPlay() override;


};
