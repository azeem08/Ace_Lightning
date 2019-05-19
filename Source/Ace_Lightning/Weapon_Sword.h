// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ace_LightningGameMode.h"
#include "Weapon_Sword.generated.h"

UCLASS()
class ACE_LIGHTNING_API AWeapon_Sword : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
												AWeapon_Sword();

	// trigger volume for pickup
	UPROPERTY( Category = Item, EditAnywhere )
	class UBoxComponent*						CollisionVolume;

	// loot bag mesh
	UPROPERTY( Category = Item, EditAnywhere )
	class USkeletalMeshComponent*				Mesh;

	// Damage value
	UPROPERTY( Category = Stats, EditAnywhere )
	float										Damage;

	bool										IsActive();

private:
	// Called when the game starts or when spawned
	virtual void								BeginPlay() override;

	// Delegate function for when an ability is active
	UFUNCTION()
	void										AbilityActive( EAbilities ability );
	// Delegate function for when an ability is not active
	UFUNCTION()
	void										DeactiveAbility( EAbilities ability );

	// pointer to the game mode
	AAce_LightningGameMode*						GameMode;
	// checks if the sword is being swung
	bool										bIsActive;
};
