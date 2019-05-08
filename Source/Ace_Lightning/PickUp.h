// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sender.h"
#include "PickUp.generated.h"

UENUM()
enum class EPickUpType
{
	Health,
	Special
};

UCLASS()
class ACE_LIGHTNING_API APickUp : public AActor, public Sender
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickUp();

	// The actor that the trigger volume is looking for
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Trigger )
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

	void										SendAMessage( EMessage message ) override {};
	void										SendAMessage( EMessage message, int value ) override {};
	void										SendAMessage( EMessage message, float value ) override;
	virtual void								SendAMessage( EMessage message, FString title, FString details ) override {};
	UFUNCTION()
	void										OnOverlapBegin( AActor * overlappedActor, AActor * otherActor );

	// Template that takes an actor and returns a pointer to the correct character type
	template <class PlayerType>
	PlayerType* GetPlayerType( PlayerType* a )
	{
		AActor* actor = a;
		PlayerType* currentType = nullptr;

		if ( actor->IsA( AMagicCharacter::StaticClass() ) )
		{
			currentType = Cast<AMagicCharacter>( a );
		}
		else if ( actor->IsA( AMeleeCharacter::StaticClass() ) )
		{
			currentType = Cast<AMeleeCharacter>( a );
		}

		return currentType;
	}

	// pointer to messaging system
	class AAce_LightningGameMode*				GameMode;
	// The actor that the trigger volume is looking for
	AActor*										TargetActor;
	// string to hold the message
	EMessage									Message;
};
