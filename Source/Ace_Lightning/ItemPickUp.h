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
	UPROPERTY( Category = Effect, EditAnywhere )
	int											CurrentType;

	// item mesh
	UPROPERTY( Category = Effect, EditAnywhere )
	class UStaticMeshComponent*					Item;

	// trigger volume for item
	UPROPERTY( Category = Effect, EditAnywhere )
	class UBoxComponent*						CollisionVolume;

private:
	// Called when the game starts or when spawned
	virtual void								BeginPlay() override;
	UFUNCTION()
	void										OnOverlapBegin( AActor * overlappedActor, AActor * otherActor );

	// pointer to messaging system
	class AAce_LightningGameMode*				GameMode;
};
