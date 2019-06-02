// Fill out your copyright notice in the Description page of Project Settings.

#include "LootDrop.h"
#include "Ace_LightningCharacter.h"
#include "Ace_LightningGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ALootDrop::ALootDrop()
	: FXItem			( nullptr )
	, CollisionVolume	( nullptr )
	, Mesh				( nullptr )
	, GoldValue			( 25 )
	, GameMode			( nullptr )
{
	CollisionVolume = CreateDefaultSubobject<UBoxComponent>( "Trigger Volume" );
	RootComponent = CollisionVolume;
	CollisionVolume->SetRelativeScale3D( FVector( 3.f, 3.f, 3.f ) );

	FXItem = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "VFX pick up" ) );
	FXItem->SetupAttachment( RootComponent );
	FXItem->bAutoActivate = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "loot bag mesh" ) );
	Mesh->SetupAttachment( RootComponent );
	Mesh->bAutoActivate = true;
}

// Called when the game starts or when spawned
void ALootDrop::BeginPlay()
{
	Super::BeginPlay();

	// Binds the overlap function to the actor
	OnActorBeginOverlap.AddDynamic( this, &ALootDrop::OnOverlapBegin );

	// Gets a pointer to the game mode which acts as the messaging system
	GameMode = Cast<AAce_LightningGameMode>( GetWorld()->GetAuthGameMode() );

	if ( !GameMode )
	{
		UE_LOG( LogTemp, Error, TEXT( "No pointer to the game mode." ) );
	}

	if ( !FXItem )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No pick up VFX added" ) );
	}

	if ( !Mesh )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No mesh added" ) );
	}
}

// Called every frame
void ALootDrop::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ALootDrop::EndPlay( EEndPlayReason::Type EndPlayReason )
{
	Super::EndPlay( EndPlayReason );

	if ( EndPlayReason == EEndPlayReason::Quit )
	{
		OnActorBeginOverlap.RemoveDynamic( this, &ALootDrop::OnOverlapBegin );
	}
}

void ALootDrop::OnOverlapBegin( AActor * overlappedActor, AActor * otherActor )
{
	// Checks if the object is the target
	if ( otherActor && ( otherActor != this ) )
	{
		if ( otherActor->IsA( AAce_LightningCharacter::StaticClass() ) )
		{
			// Sends a message that a pickup has been collected and destroys the pickup
			GameMode->LootBagCollected( GoldValue, 1 );
			FXItem->Deactivate();
			FXItem->KillParticlesForced();
			Destroy();
		}
	}
}

