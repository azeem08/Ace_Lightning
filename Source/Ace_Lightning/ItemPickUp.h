// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemPickUp.generated.h"

UCLASS()
class ACE_LIGHTNING_API AItemPickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemPickUp();

	// the type of item
	UPROPERTY( Category = PickUp, EditAnywhere )
	int											CurrentType;

	// item mesh
	UPROPERTY( Category = PickUp, EditAnywhere )
	class UStaticMeshComponent*					Item;

	// trigger volume for item
	UPROPERTY( Category = PickUp, EditAnywhere )
	class UBoxComponent*						CollisionVolume;

	// particle system to highlight object
	UPROPERTY( Category = PickUp, EditAnywhere )
	class UParticleSystemComponent*				FXPickUp;

private:
	// Called when the game starts or when spawned
	virtual void								BeginPlay() override;
	virtual void								EndPlay( EEndPlayReason::Type EndPlayReason ) override;
	UFUNCTION()
	void										OnOverlapBegin( AActor * overlappedActor, AActor * otherActor );
	// Delegate function to check if the item can be collected
	UFUNCTION()
	void										InventoryAvailable( bool available );

	// pointer to messaging system
	class AAce_LightningGameMode*				GameMode;
	bool										bIsCollectable;
};
