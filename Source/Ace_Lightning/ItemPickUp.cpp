// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickUp.h"
#include "Ace_LightningCharacter.h"
#include "Ace_LightningGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItemPickUp::AItemPickUp()
	: CollisionVolume	( nullptr )
	, GameMode			( nullptr )
{
	CollisionVolume = CreateDefaultSubobject<UBoxComponent>( "Trigger Volume" );
	RootComponent = CollisionVolume;

	Item = CreateDefaultSubobject<UStaticMeshComponent>( "Mesh" );
	Item->SetupAttachment( RootComponent );
}

// Called when the game starts or when spawned
void AItemPickUp::BeginPlay()
{
	Super::BeginPlay();
	
	// Binds the overlap function to the actor
	OnActorBeginOverlap.AddDynamic( this, &AItemPickUp::OnOverlapBegin );

	// Gets a pointer to the game mode which acts as the messaging system
	GameMode = Cast<AAce_LightningGameMode>( GetWorld()->GetAuthGameMode() );

	if ( !GameMode )
	{
		UE_LOG( LogTemp, Error, TEXT( "No pointer to the game mode." ) );
	}
}

void AItemPickUp::OnOverlapBegin( AActor * overlappedActor, AActor * otherActor )
{
	// Checks if the object is the target
	if ( otherActor && ( otherActor != this ) && otherActor->IsA( AAce_LightningCharacter::StaticClass() ) )
	{
		// Sends a message that a pickup has been collected and destroys the pickup
		GameMode->ItemCollected( CurrentType );
		Destroy();
	}
}

