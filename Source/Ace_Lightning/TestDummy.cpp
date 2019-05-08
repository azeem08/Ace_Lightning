// Fill out your copyright notice in the Description page of Project Settings.

#include "TestDummy.h"
#include "Components/CapsuleComponent.h"	
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyController.h"
#include "TimerManager.h"
#include "LootDrop.h"

// Sets default values
ATestDummy::ATestDummy()
	: CurrentHealth		( 1.0f )
	, CollisionVolume	( nullptr )
	, PawnSensor		( nullptr)
	, FXDamage			( nullptr )
	, TurnSpeed			( 20 )
	, XP				( 1.f )
	, AIManager			( nullptr )
	, DamageTaken		( 0.f )
	, bIsOverlapping	( false )
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<UCapsuleComponent>( TEXT( "Collision Volume" ) );
	CollisionVolume->AttachTo( RootComponent, "RightFoot" );

	FXDamage = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "VFX Damage" ) );
	FXDamage->AttachTo( RootComponent, "RightFoot" );
	FXDamage->bAutoActivate = false;

	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>( TEXT( "Pawn Sensor" ) );
}

// Called when the game starts or when spawned
void ATestDummy::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AAce_LightningGameMode>( GetWorld()->GetAuthGameMode() );

	if ( !GameMode )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No game mode pointer" ) );
	}
	else
	{
		GameMode->AddReceiver( this );
		GameMode->AddSender( this );
	}

	if ( !FXDamage )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No VFX added" ) );
	}

	if ( !PawnSensor )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No pawn sensing component" ) );
	}
	else
	{
		PawnSensor->OnHearNoise.AddDynamic( this, &ATestDummy::OnHearNoise );
		PawnSensor->OnSeePawn.AddDynamic( this, &ATestDummy::OnSeePawn );

		PawnSensor->bOnlySensePlayers = false;
	}

	AIManager = Cast<AEnemyController>( GetController() );

	if ( !AIManager )
	{
		UE_LOG( LogTemp, Warning, TEXT( "Couldn't find enemy controller" ) );
	}

	// Binds the overlap function to the actor
	OnActorBeginOverlap.AddDynamic( this, &ATestDummy::OnOverlapBegin );
}

// Called every frame
void ATestDummy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	float speed = FMath::Clamp( GetVelocity().Size(), 0.f, 600.f );

	// Checks if the AI is standing still
	if ( !bIsLookingAtPlayer )
	{
		if ( speed == 0.f )
		{
			// Rotates the AI to change its point of view
			FRotator newRotation = GetActorRotation();
			newRotation.Yaw += ( DeltaTime * TurnSpeed );

			SetActorRotation( newRotation );
		}
	}
}

void ATestDummy::OnOverlapBegin( AActor* overlappedActor, AActor* otherActor )
{
	// Checks if the object is the target
	if ( otherActor &&  otherActor != this )
	{
		if ( !bIsOverlapping )
		{
			DamageTaken = GetDamage( otherActor );

			if ( DamageTaken > 0 )
			{
				bIsOverlapping = true;
				CurrentHealth -= DamageTaken;
				if ( FXDamage->IsActive() )
				{
					FXDamage->Deactivate();
					FXDamage->KillParticlesForced();
				}

				FXDamage->Activate();

				if ( CurrentHealth <= 0.f )
				{
					SetActorEnableCollision( false );
					GetWorldTimerManager().SetTimer( TimerHandle, this, &ATestDummy::Dead, kDamageTime, false );
				}
			}
		}
	}
}

void ATestDummy::Dead()
{
	GetWorldTimerManager().ClearTimer( TimerHandle );
	FXDamage->Deactivate();
	FXDamage->KillParticlesForced();

	SendAMessage( EMessage::EnemyKilled, XP );

	// Spawns a loot drop at the dummy's location
	FVector Location = ( RootComponent->GetSocketLocation( "foot_r" ) - FVector( 0.f, 0.f, kFloorHeight ) );
	FRotator Rotation = RootComponent->GetSocketRotation( "foot_r" );
	FActorSpawnParameters SpawnInfo;
	GetWorld()->SpawnActor<ALootDrop>( BP_LootDrop, Location, Rotation, SpawnInfo );

	Destroy();
}

void ATestDummy::OnHearNoise( APawn * OtherActor, const FVector & Location, float Volume )
{
}

void ATestDummy::OnSeePawn( APawn* OtherPawn )
{
	bIsLookingAtPlayer = IsPlayer( OtherPawn );

	if ( bIsLookingAtPlayer )
	{
		PawnSensor->bOnlySensePlayers = true;
	}

	if ( OtherPawn )
	{
		AIManager->OnSeePawn( OtherPawn );
	}
}

void ATestDummy::SendAMessage( EMessage message, float value )
{
	SetMessage( message );
	SetFloatValue( value );
	GameMode->ReadMessage();
}

void ATestDummy::ReadMessage( EMessage message )
{
	if ( message == EMessage::StopAbility1 )
	{
		bIsOverlapping = false;
	}

	if ( message == EMessage::StopAbility2 )
	{
		bIsOverlapping = false;
	}
}

