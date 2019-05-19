// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeCharacter.h"
#include "Ace_LightningGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon_Sword.h"
#include "Components/SkeletalMeshComponent.h"

AMeleeCharacter::AMeleeCharacter()
	: SwordAttackCost	( 0.15f )
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMeleeCharacter::BeginPlay()
{
	Super::BeginPlay();

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
		mesh->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket" );
	}
}

void AMeleeCharacter::SetupPlayerInputComponent( UInputComponent * PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );
}

void AMeleeCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AMeleeCharacter::Ability1()
{
	Super::Ability1(); 

	// Checks if the ability has cooled down and isn't in the middle of an animation
	if ( bAbilityHasCooledDown[0] && bHasFinishedAnimating )
	{
		if ( CurrentSpecial >= SwordAttackCost )
		{
			CurrentSpecial -= SwordAttackCost;

			bAbilityHasCooledDown[0] = false;
			bHasFinishedAnimating = false;

			GameMode->ActivateAbility( EAbilities::First );

			// Starts a countdown timer and calls the function to send a message that the ability has finished
			GetWorldTimerManager().SetTimer( AnimationTimer, this, &AMeleeCharacter::StopSlashing, kSwordSlashTime, false );
		}
		else
		{
			GameMode->TriggerNotification( EStats::Special );
		}
	}
}

void AMeleeCharacter::StopSlashing()
{
	GameMode->StopCasting( EAbilities::First );

	bHasFinishedAnimating = true;

	// Clears the timer
	GetWorldTimerManager().ClearTimer( AnimationTimer );
}