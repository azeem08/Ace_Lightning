// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicProjectile.h"					 
#include "Ace_LightningGameMode.h"	 
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMagicProjectile::AMagicProjectile()
	: FXProjectile	( nullptr )
	, Collider		( nullptr )
	, Movement		( nullptr )
	, Damage		( 0.f )
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>( TEXT( "Sphere Collider" ) );
	RootComponent = Collider;

	FXProjectile = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "VFX Projectile" ) );
	FXProjectile->AttachTo( RootComponent );

	Movement = CreateAbstractDefaultSubobject<UProjectileMovementComponent>( TEXT( "Movement" ) );
	Movement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if ( !FXProjectile )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No VFX added" ) );
	}
}

// Called every frame
void AMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

