// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUp.h"	
#include "Ace_LightningGameMode.h"
#include "Components/BoxComponent.h"
#include "kismet/GameplayStatics.h"
#include "MagicCharacter.h"
#include "MeleeCharacter.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APickUp::APickUp()
	: FXItem			( nullptr )
	, CollisionVolume	( nullptr )
	, PickUpValue		( 0.2f )
	, GameMode			( nullptr )
	, TargetActor		( nullptr )
	, Message			( EMessage::NoMessage )
{
	CollisionVolume = CreateDefaultSubobject<UBoxComponent>( "Trigger Volume" );
	RootComponent = CollisionVolume;
	CollisionVolume->SetRelativeScale3D( FVector( 3.f, 3.f, 3.f ) );

	FXItem = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "VFX pick up" ) );
	FXItem->AttachTo( RootComponent );
	FXItem->bAutoActivate = true;
}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();

	// Binds the overlap function to the actor
	OnActorBeginOverlap.AddDynamic( this, &APickUp::OnOverlapBegin );

	// Gets a pointer to the game mode which acts as the messaging system
	GameMode = Cast<AAce_LightningGameMode>( GetWorld()->GetAuthGameMode() );

	if ( !GameMode )
	{
		UE_LOG( LogTemp, Error, TEXT( "No pointer to the game mode." ) );
	}
	// Registers this trigger as a message sender
	GameMode->AddSender( this );

	if ( !FXItem )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No pick up VFX added" ) );
	}

	switch ( CurrentType )
	{
	case EPickUpType::Health:
		Message = EMessage::HealthPickUp;
		break;
	case EPickUpType::Special:
		Message = EMessage::SpecialPickUp;
		break;
	}
}

void APickUp::SendAMessage( EMessage message, float value )
{
	// Sends a message to the game mode and each receiver reads the message
	SetMessage( message );
	SetFloatValue( value );
	GameMode->ReadMessage();
}

void APickUp::OnOverlapBegin( AActor* overlappedActor, AActor* otherActor )
{
	TargetActor = GetPlayerType( UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 ) );

	// Checks if the object is the target
	if ( otherActor && ( otherActor != this ) && otherActor == TargetActor )
	{
		// Sends a message that a pickup has been collected and destroys the pickup
		SendAMessage( Message, PickUpValue );
		FXItem->Deactivate();
		FXItem->KillParticlesForced();
		Destroy();
	}
}
