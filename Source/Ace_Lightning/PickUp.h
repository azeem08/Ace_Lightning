// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ace_LightningGameMode.h"
#include "PickUp.generated.h"

UENUM()
enum class EPickUpType
{
	Health,
	Special
};

UCLASS()
class ACE_LIGHTNING_API APickUp : public AActor
{
	GENERATED_BODY()

public:												   
	// Sets default values for this actor's properties
	APickUp();

	// the type of pickup
	UPROPERTY( Category = Effect, EditAnywhere )
	EPickUpType									CurrentType;

	// pickup VFX
	UPROPERTY( Category = Effect, EditAnywhere )
	class UParticleSystemComponent*				FXItem;

	// trigger volume for pickup
	UPROPERTY( Category = Effect, EditAnywhere )
	class UBoxComponent*						CollisionVolume;

	// value of pickup
	UPROPERTY( Category = Effect, EditAnywhere )
	float										PickUpValue;

private:
	// Called when the game starts or when spawned
	virtual void								BeginPlay() override;

	UFUNCTION()
	void										OnOverlapBegin( AActor * overlappedActor, AActor * otherActor );

	// pointer to messaging system
	class AAce_LightningGameMode*				GameMode;
	// The players stat which should change
	EStats										SType;
};
