// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeCharacter.h"
#include "Weapon_Sword.h"
#include "Components/SkeletalMeshComponent.h"

AMeleeCharacter::AMeleeCharacter()
	: CurrentStamina	( 0.5f )
	, MaxStamina		( 1.f )
	, SwordAttackCost	( 0.15f )
{
	FXStaminaPickUp = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "VFX Mana pick up" ) );
	FXStaminaPickUp->AttachTo( RootComponent, "RightFoot" );
	FXStaminaPickUp->bAutoActivate = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

float AMeleeCharacter::GetCurrentHealth()
{
	return CurrentHealth;
}

float AMeleeCharacter::GetCurrentStamina()
{
	return CurrentStamina;
}

void AMeleeCharacter::BeginPlay()
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

	if ( !FXStaminaPickUp )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No stamina pick up VFX added" ) );
	}

	if ( !BP_Sword )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No blueprint version of sword found" ) );
	}
	else
	{
		// Spawns a sword and attaches it to the right hand socket
		FVector Location = GetMesh()->GetSocketLocation( "WeaponSocket" );
		FRotator Rotation = GetMesh()->GetSocketRotation( "WeaponSocket" );
		FActorSpawnParameters SpawnInfo;
		AWeapon_Sword* sword = GetWorld()->SpawnActor<AWeapon_Sword>( BP_Sword, Location, Rotation, SpawnInfo );
		USkeletalMeshComponent* mesh = Cast<USkeletalMeshComponent>( sword->GetComponentByClass( USkeletalMeshComponent::StaticClass() ) );
		mesh->AttachTo( GetMesh(), "WeaponSocket", EAttachLocation::SnapToTarget );
	}
}

void AMeleeCharacter::SetupPlayerInputComponent( UInputComponent * PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	check( PlayerInputComponent );
	PlayerInputComponent->BindAction( "Ability1", IE_Pressed, this, &AMeleeCharacter::SwordSlash );
}

void AMeleeCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if ( CurrentStamina < MaxStamina )
	{
		CurrentStamina += ( DeltaTime * StaminaRegenRate );

		if ( CurrentStamina > MaxStamina )
		{
			CurrentStamina = MaxStamina;
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

void AMeleeCharacter::SendAMessage( EMessage message )
{
	SetMessage( message );
	GameMode->ReadMessage();
}

void AMeleeCharacter::SendAMessage( EMessage message, int value )
{
	SetMessage( message );
	SetIntValue( value );
	GameMode->ReadMessage();
}

void AMeleeCharacter::ReadMessage( EMessage message, int value )
{
	if ( message == EMessage::LootPickUp )
	{
		FXLootPickUp->Activate();
	}

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

void AMeleeCharacter::ReadMessage( EMessage message, float value )
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
		if ( CurrentStamina < MaxStamina )
		{
			CurrentStamina += value;
			FXStaminaPickUp->Activate();

			if ( CurrentStamina > MaxStamina )
			{
				CurrentStamina = MaxStamina;
			}
		}
	}
}

void AMeleeCharacter::StopAnimation( FTimerHandle& timer, EMessage message )
{
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

void AMeleeCharacter::SwordSlash()
{
	// Checks if the ability has cooled down and isn't in the middle of an animation
	if ( bAbilityHasCooledDown[0] && bHasFinishedAnimating )
	{
		if ( CurrentStamina >= SwordAttackCost )
		{
			CurrentStamina -= SwordAttackCost;

			bAbilityHasCooledDown[0] = false;
			bHasFinishedAnimating = false;

			SendAMessage( EMessage::CastAbility1 );

			// Starts a countdown timer and calls the function to send a message that the ability has finished
			GetWorldTimerManager().SetTimer( AnimationTimer, this, &AMeleeCharacter::StopSlashing, kSwordSlashTime, false );
		}
		else
		{
			SendAMessage( EMessage::NoSpecial );
		}
	}
}

void AMeleeCharacter::StopSlashing()
{
	StopAnimation( AnimationTimer, EMessage::StopAbility1 );
}
