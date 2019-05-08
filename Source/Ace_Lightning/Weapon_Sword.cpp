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
	CollisionVolume->AttachTo( Mesh );
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
		GameMode->AddReceiver( this );
	}

	SetActorEnableCollision( false );
}

bool AWeapon_Sword::IsActive()
{
	return bIsActive;
}

void AWeapon_Sword::ReadMessage( EMessage message )
{
	if ( message == EMessage::CastAbility1 )
	{
		SetActorEnableCollision( true );
		bIsActive = true;
	}

	if ( message == EMessage::StopAbility1 )
	{
		SetActorEnableCollision( false );
		bIsActive = false;
	}
}

