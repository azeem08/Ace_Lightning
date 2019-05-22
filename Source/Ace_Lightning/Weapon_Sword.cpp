// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon_Sword.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeapon_Sword::AWeapon_Sword()
	: CollisionVolume	( nullptr )
	, Mesh				( nullptr )
	, Damage			( 0.f )
	, bIsActive			( false )
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "sword mesh" ) );
	RootComponent = Mesh;
	Mesh->bAutoActivate = true;

	CollisionVolume = CreateDefaultSubobject<UBoxComponent>( "Trigger Volume" );
	CollisionVolume->SetupAttachment( RootComponent );
}

// Called when the game starts or when spawned
void AWeapon_Sword::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AAce_LightningGameMode>( GetWorld()->GetAuthGameMode() );

	if ( !GameMode )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No game mode pointer" ) );
	}
	else
	{
		GameMode->Event_ActivateAbility.AddDynamic( this, &AWeapon_Sword::AbilityActive );
		GameMode->DeactivateEvent.AddDynamic( this, &AWeapon_Sword::DeactiveAbility );
	}

	SetActorEnableCollision( false );
}

void AWeapon_Sword::EndPlay( EEndPlayReason::Type EndPlayReason )
{
	Super::EndPlay( EndPlayReason );

	if ( EndPlayReason == EEndPlayReason::Quit )
	{
		GameMode->Event_ActivateAbility.RemoveDynamic( this, &AWeapon_Sword::AbilityActive );
		GameMode->DeactivateEvent.RemoveDynamic( this, &AWeapon_Sword::DeactiveAbility );
	}
}

void AWeapon_Sword::AbilityActive( EAbilities ability )
{
	if ( ability == EAbilities::First )
	{
		SetActorEnableCollision( true );
		bIsActive = true;
	}
}

void AWeapon_Sword::DeactiveAbility( EAbilities ability )
{
	if ( ability == EAbilities::First )
	{
		SetActorEnableCollision( false );
		bIsActive = false;
	}
}

bool AWeapon_Sword::IsActive()
{
	return bIsActive;
}

