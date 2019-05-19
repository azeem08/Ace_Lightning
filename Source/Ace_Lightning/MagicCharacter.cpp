// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicCharacter.h"
#include "Ace_LightningGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MagicProjectile.h"
#include "Particles/ParticleSystemComponent.h"

AMagicCharacter::AMagicCharacter()
	: HealAmount			( 0.2f )
	, FXHeal				( nullptr )
	, CastingTime			( 2.f )
	, HealManaCost			( 0.15f )
	, MagicAttackManaCost	( 0.15f )
{
	FXHeal = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "VFX Heal" ) );
	FXHeal->SetupAttachment( RootComponent, "RightFoot" );
	FXHeal->bAutoActivate = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMagicCharacter::BeginPlay()
{
	Super::BeginPlay();

	if ( !FXHeal )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No heal VFX added" ) );
	}

	if ( !BP_MagicProjectile )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No blueprint version of magic projectile found" ) );
	}
}

void AMagicCharacter::SetupPlayerInputComponent( UInputComponent * PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );
}

void AMagicCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AMagicCharacter::Ability1()
{
	if ( !FXHeal->IsActive() )
	{
		// Checks if the ability has cooled down and isn't in the middle of an animation
		if ( bAbilityHasCooledDown[0] && bHasFinishedAnimating )
		{
			if ( CurrentHealth < MaxHealth && CurrentSpecial >= HealManaCost )
			{
				CurrentSpecial -= HealManaCost;
				CurrentHealth += HealAmount;

				if ( CurrentHealth > MaxHealth )
				{
					CurrentHealth = MaxHealth;
				}

				bAbilityHasCooledDown[0] = false;
				bHasFinishedAnimating = false;

				// Heals the player and takes away some mana
				FXHeal->Activate();
				GameMode->ActivateAbility( EAbilities::First );

				// Starts a countdown timer and calls the stop healing function after 2 seconds
				GetWorldTimerManager().SetTimer( TimerHandle, this, &AMagicCharacter::StopHealing, CastingTime, false );

			}
			else if ( CurrentSpecial < HealManaCost )
			{
				GameMode->TriggerNotification( EStats::Special );
			}
		}
	}
}

void AMagicCharacter::Ability2()
{
	// Checks if the ability has cooled down and isn't in the middle of an animation
	if ( bAbilityHasCooledDown[1] && bHasFinishedAnimating )
	{
		if ( CurrentSpecial >= MagicAttackManaCost )
		{
			CurrentSpecial -= MagicAttackManaCost;

			bAbilityHasCooledDown[1] = false;
			bHasFinishedAnimating = false;

			GameMode->ActivateAbility( EAbilities::Second );

			// Starts a countdown timer and calls the function to fire a projectile before stopping the animation
			GetWorldTimerManager().SetTimer( TimerHandle, this, &AMagicCharacter::FireMagicProjectile, kProjectileSpawnTime, false );
			GetWorldTimerManager().SetTimer( AnimationTimer, this, &AMagicCharacter::StopMagicProjectile, kAbility2Time, false );
		}
		else
		{
			GameMode->TriggerNotification( EStats::Special );
		}
	}
}

void AMagicCharacter::StopHealing()
{
	// Deactivates the heal VFX and removes all the particles
	FXHeal->Deactivate();
	FXHeal->KillParticlesForced();

	GameMode->StopCasting( EAbilities::First );

	bHasFinishedAnimating = true;

	// Clears the timer
	GetWorldTimerManager().ClearTimer( TimerHandle );
}

void AMagicCharacter::FireMagicProjectile()
{
	// Spawns a magic projectile at the right hands location
	FVector Location = RootComponent->GetSocketLocation( "RightHand" ) + FVector( kEmptyValue, kEmptyValue, kRightHandHeight );
	FRotator Rotation = RootComponent->GetSocketRotation( "RightHand" );
	FActorSpawnParameters SpawnInfo;
	GetWorld()->SpawnActor<AMagicProjectile>( BP_MagicProjectile, Location, Rotation, SpawnInfo );
}

void AMagicCharacter::StopMagicProjectile()
{
	GameMode->StopCasting( EAbilities::Second );
	bHasFinishedAnimating = true;

	// Clears the timer
	GetWorldTimerManager().ClearTimer( AnimationTimer );
}
