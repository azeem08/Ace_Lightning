// Fill out your copyright notice in the Description page of Project Settings.

#include "Waypoint.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWaypoint::AWaypoint()
	: Mesh	( nullptr )
	, ID	( 0 )
{
	USceneComponent* scene = CreateDefaultSubobject<USceneComponent>( TEXT( "scene" ) );
	RootComponent = scene;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Mesh" ) );
	Mesh->SetupAttachment( RootComponent );
}

// Called when the game starts or when spawned
void AWaypoint::BeginPlay()
{
	Super::BeginPlay();

	Mesh->DestroyComponent();
	
}
	  
