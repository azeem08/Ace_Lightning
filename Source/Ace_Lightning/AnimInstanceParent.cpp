// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstanceParent.h"
#include "Ace_LightningCharacter.h"
#include "Ace_LightningGameMode.h"

UAnimInstanceParent::UAnimInstanceParent()
	: Direction		( 0.f )
	, Speed			( 0.f )
	, bIsAbility1	( false )
	, bIsAbility2	( false )
{
}

void UAnimInstanceParent::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<AAce_LightningCharacter>( TryGetPawnOwner() );

	if ( !Player )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No player pointer" ) );
	}

	GameMode = Cast<AAce_LightningGameMode>( GetWorld()->GetAuthGameMode() );

	if ( !GameMode )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No game mode pointer" ) );
	}
	else
	{
		GameMode->Event_ActivateAbility.AddDynamic( this, &UAnimInstanceParent::StartAnimating );
		GameMode->DeactivateEvent.AddDynamic( this, &UAnimInstanceParent::StopAnimating );
	}
}

void UAnimInstanceParent::NativeUpdateAnimation( float DeltaTime )
{
	// Is called before the game is playing

	Super::NativeUpdateAnimation( DeltaTime );

	if ( !Player )
	{
		return;
	}

	// Calculates the direction and speed based on the players velocity and rotation
	float angle1 = FMath::Atan2( Player->GetVelocity().X, Player->GetVelocity().Y );
	float angle2 = FMath::Atan2( Player->GetActorRotation().Pitch, Player->GetActorRotation().Yaw );
	float ang = FMath::RadiansToDegrees( angle2 - angle1 );
	float speed = FMath::Clamp( Player->GetVelocity().Size(), 0.f, 600.f );

	if ( ang > 180.0f ) ang -= 360.0f; else if ( ang < -180.0f ) ang += 360.0f;

	SetDirection( ang );
	SetSpeed( speed );
}

float UAnimInstanceParent::GetDirection()
{
	return Direction;
}

void UAnimInstanceParent::SetDirection( float newDirection )
{
	Direction = newDirection;
}

float UAnimInstanceParent::GetSpeed()
{
	return Speed;
}

void UAnimInstanceParent::SetSpeed( float newSpeed )
{
	Speed = newSpeed;
}

bool UAnimInstanceParent::IsFalling()
{
	return bIsFalling;
}

void UAnimInstanceParent::SetFalling( bool newValue )
{
	bIsFalling = newValue;
}

bool UAnimInstanceParent::IsCastingAbility1()
{
	return bIsAbility1;
}

void UAnimInstanceParent::ActivateAbility1( bool newValue )
{
	bIsAbility1 = newValue;
}

bool UAnimInstanceParent::IsCastingAbility2()
{
	return bIsAbility2;
}

void UAnimInstanceParent::ActivateAbility2( bool newValue )
{
	bIsAbility2 = newValue;
}

void UAnimInstanceParent::StartAnimating( EAbilities ability )
{
	if ( ability == EAbilities::Jumping )
	{
		if ( !IsFalling() )
		{
			SetFalling( true );
		}
	}

	if ( ability == EAbilities::First )
	{
		if ( !IsCastingAbility1() )
		{
			ActivateAbility1( true );
		}
	}

	if ( ability == EAbilities::Second )
	{
		if ( !IsCastingAbility2() )
		{
			ActivateAbility2( true );
		}
	}
}

void UAnimInstanceParent::StopAnimating( EAbilities ability )
{
	if ( ability == EAbilities::Jumping )
	{
		SetFalling( false );
	}

	if ( ability == EAbilities::First )
	{
		ActivateAbility1( false );
	}

	if ( ability == EAbilities::Second )
	{
		ActivateAbility2( false );
	}

	if ( ability == EAbilities::End )
	{
		GameMode->Event_ActivateAbility.RemoveDynamic( this, &UAnimInstanceParent::StartAnimating );
		GameMode->DeactivateEvent.RemoveDynamic( this, &UAnimInstanceParent::StopAnimating );
	}
}
