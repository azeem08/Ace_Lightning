// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicCharacter.h"
#include "MagicProjectile.h"

AMagicCharacter::AMagicCharacter()
	: CurrentMana			( 0.5f )
	, ManaRegenRate			( 1.f )
	, HealAmount			( 0.2f )
	, FXHeal				( nullptr )
	, FXManaPickUp			( nullptr )
	, MaxMana				( 1.f )
	, CastingTime			( 2.f )
	, HealManaCost			( 0.15f )
	, MagicAttackManaCost	( 0.15f )
{
	FXHeal = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "VFX Heal" ) );
	FXHeal->AttachTo( RootComponent, "RightFoot" );
	FXHeal->bAutoActivate = false;

	FXManaPickUp = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "VFX Mana pick up" ) );
	FXManaPickUp->AttachTo( RootComponent, "RightFoot" );
	FXManaPickUp->bAutoActivate = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

float AMagicCharacter::GetCurrentHealth()
{
	return CurrentHealth;
}

float AMagicCharacter::GetCurrentMana()
{
	return CurrentMana;
}

void AMagicCharacter::BeginPlay()
{
	Super::BeginPlay();

	if ( !GameMode )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No game mode pointer" ) );
	}
	else
	{
		GameMode->AddSender( this );
		GameMode->AddReceiver( this );
	}

	if ( !FXHeal )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No heal VFX added" ) );
	}

	if ( !FXManaPickUp )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No mana pick up VFX added" ) );
	}

	if ( !BP_MagicProjectile )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No blueprint version of magic projectile found" ) );
	}
}

void AMagicCharacter::SetupPlayerInputComponent( UInputComponent * PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	// Set up gameplay key bindings
	check( PlayerInputComponent );
	PlayerInputComponent->BindAction( "Ability1", IE_Pressed, this, &AMagicCharacter::Heal );
	PlayerInputComponent->BindAction( "Ability2", IE_Pressed, this, &AMagicCharacter::MagicAttack );
}

void AMagicCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if ( CurrentMana < MaxMana )
	{
		CurrentMana += ( DeltaTime * ManaRegenRate );

		if ( CurrentMana > MaxMana )
		{
			CurrentMana = MaxMana;
		}
	}

	if ( MovementComponent->IsFalling() )
	{
		if ( !bHasSentMessage )
		{
			SendAMessage( EMessage::IsJumping );
			bHasSentMessage = true;
		}
	}
	else
	{
		if ( bHasSentMessage )
		{
			SendAMessage( EMessage::StoppedJumping );
			bHasSentMessage = false;
		}
	}
}

void AMagicCharacter::SendAMessage( EMessage message )
{
	SetMessage( message );
	GameMode->ReadMessage();
}

void AMagicCharacter::SendAMessage( EMessage message, int value )
{
	SetMessage( message );
	SetIntValue( value );
	GameMode->ReadMessage();
}

void AMagicCharacter::ReadMessage( EMessage message )
{
	if ( message == EMessage::LootPickUp )
	{
		FXLootPickUp->Activate();
	}
}

void AMagicCharacter::ReadMessage( EMessage message, int value )
{
	if ( message == EMessage::AbilityAvailable )
	{
		switch ( value )
		{
		case 1:
			bAbilityHasCooledDown[0] = true;
			break;
		case 2:
			bAbilityHasCooledDown[1] = true;
			break;
		case 3:
			bAbilityHasCooledDown[2] = true;
			break;
		case 4:
			bAbilityHasCooledDown[3] = true;
			break;
		case 5:
			bAbilityHasCooledDown[4] = true;
			break;
		case 6:
			bAbilityHasCooledDown[5] = true;
			break;
		case 7:
			bAbilityHasCooledDown[6] = true;
			break;
		case 8:
			bAbilityHasCooledDown[7] = true;
			break;
		case 9:
			bAbilityHasCooledDown[8] = true;
			break;
		case 10:
			bAbilityHasCooledDown[9] = true;
			break;
		default:
			bAbilityHasCooledDown[0] = true;
			break;
		}
	}
}

void AMagicCharacter::ReadMessage( EMessage message, float value )
{
	// Checks if a pickup has been collected and increases health/mana
	// and plays the appropriate VFX
	if ( message == EMessage::HealthPickUp )
	{
		if ( CurrentHealth < MaxHealth )
		{
			CurrentHealth += value;
			FXHealthPickUp->Activate();

			if ( CurrentHealth > MaxHealth )
			{
				CurrentHealth = MaxHealth;
			}
		}
	}

	if ( message == EMessage::SpecialPickUp )
	{
		if ( CurrentMana < MaxMana )
		{
			CurrentMana += value;
			FXManaPickUp->Activate();

			if ( CurrentMana > MaxMana )
			{
				CurrentMana = MaxMana;
			}
		}
	}
}

//void AMagicCharacter::StopAnimation( FTimerHandle & timer, FString message )
void AMagicCharacter::StopAnimation( FTimerHandle& timer, EMessage message )
{
	//if ( !message.IsEmpty() )
	if ( message != EMessage::NoMessage )
	{
		SendAMessage( message );
	}

	bHasFinishedAnimating = true;

	// Clears the timer
	GetWorldTimerManager().ClearTimer( timer );
	// Enables movement
	//MovementComponent->SetMovementMode( EMovementMode::MOVE_Walking );
}

void AMagicCharacter::Heal()
{
	if ( !FXHeal->IsActive() )
	{
		// Checks if the ability has cooled down and isn't in the middle of an animation
		if ( bAbilityHasCooledDown[0] && bHasFinishedAnimating )
		{
			if ( CurrentHealth < MaxHealth && CurrentMana >= HealManaCost )
			{
				CurrentMana -= HealManaCost;
				CurrentHealth += HealAmount;

				if ( CurrentHealth > MaxHealth )
				{
					CurrentHealth = MaxHealth;
				}

				bAbilityHasCooledDown[0] = false;
				bHasFinishedAnimating = false;

				// Heals the player and takes away some mana
				FXHeal->Activate();
				SendAMessage( EMessage::CastAbility1 );

				// Starts a countdown timer and calls the stop healing function after 2 seconds
				GetWorldTimerManager().SetTimer( TimerHandle, this, &AMagicCharacter::StopHealing, CastingTime, false );

			}
			else if ( CurrentMana < HealManaCost )
			{
				SendAMessage( EMessage::NoSpecial );
			}
		}
	}
}

void AMagicCharacter::MagicAttack()
{
	// Checks if the ability has cooled down and isn't in the middle of an animation
	if ( bAbilityHasCooledDown[1] && bHasFinishedAnimating )
	{
		if ( CurrentMana >= MagicAttackManaCost )
		{
			CurrentMana -= MagicAttackManaCost;

			bAbilityHasCooledDown[1] = false;
			bHasFinishedAnimating = false;

			SendAMessage( EMessage::CastAbility2 );

			// Starts a countdown timer and calls the function to fire a projectile before stopping the animation
			GetWorldTimerManager().SetTimer( TimerHandle, this, &AMagicCharacter::FireMagicProjectile, kProjectileSpawnTime, false );
			GetWorldTimerManager().SetTimer( AnimationTimer, this, &AMagicCharacter::StopMagicProjectile, kAbility2Time, false );
		}
		else
		{
			SendAMessage( EMessage::NoSpecial );
		}
	}
}

void AMagicCharacter::StopHealing()
{
	// Deactivates the heal VFX and removes all the particles
	FXHeal->Deactivate();
	FXHeal->KillParticlesForced();

	StopAnimation( TimerHandle, EMessage::StopAbility1 );
}

void AMagicCharacter::FireMagicProjectile()
{
	// Spawns a magic projectile at the right hands location
	FVector Location = RootComponent->GetSocketLocation( "RightHand" ) + FVector( kEmptyValue, kEmptyValue, kRightHandHeight );
	FRotator Rotation = RootComponent->GetSocketRotation( "RightHand" );
	FActorSpawnParameters SpawnInfo;
	GetWorld()->SpawnActor<AMagicProjectile>( BP_MagicProjectile, Location, Rotation, SpawnInfo );

	StopAnimation( TimerHandle, EMessage::NoMessage );
}

void AMagicCharacter::StopMagicProjectile()
{
	StopAnimation( AnimationTimer, EMessage::StopAbility2 );
}
