// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MessageClass.h"
#include "LootDrop.generated.h"

UCLASS()
class ACE_LIGHTNING_API ALootDrop : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
												ALootDrop();
	// pickup VFX
	UPROPERTY( Category = Item, EditAnywhere )
	class UParticleSystemComponent*				FXItem;

	// trigger volume for pickup
	UPROPERTY( Category = Item, EditAnywhere )
	class UBoxComponent*						CollisionVolume;

	// loot bag mesh
	UPROPERTY( Category = Item, EditAnywhere )
	class UStaticMeshComponent*					Mesh;

	// Amount of gold the loot bag contains
	UPROPERTY( Category = Item, EditAnywhere )
	int											GoldValue;


private:
	// Called when the game starts or when spawned
	virtual void								BeginPlay() override;
	// Called every frame
	virtual void								Tick( float DeltaTime ) override;
	virtual void								EndPlay( EEndPlayReason::Type EndPlayReason ) override;

	UFUNCTION()
	void										OnOverlapBegin( AActor * overlappedActor, AActor * otherActor );

	// pointer to messaging system
	class AAce_LightningGameMode*				GameMode;
};
