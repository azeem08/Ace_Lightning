// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Characters.h"
#include "Ace_LightningGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AUI_Characters::AUI_Characters()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>( TEXT( "Root Component" ) );
	RootComponent = Box;

	MagicCharacter = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "Magic Character" ) );
	MagicCharacter->SetupAttachment( RootComponent );

	MeleeCharacter = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "Melee Character" ) );
	MeleeCharacter->SetupAttachment( RootComponent );

	Camera = CreateDefaultSubobject<UCameraComponent>( TEXT( "Camera" ) );
	Camera->SetupAttachment( RootComponent );

	Capture2D = CreateDefaultSubobject<USceneCaptureComponent2D>( TEXT( "Capture Component 2D" ) );
	Capture2D->SetupAttachment( Camera );
}

// Called when the game starts or when spawned
void AUI_Characters::BeginPlay()
{
	Super::BeginPlay();

	// Gets a pointer to the game mode which acts as the messaging system
	GameMode = Cast<AAce_LightningGameMode>( GetWorld()->GetAuthGameMode() );

	if ( !GameMode )
	{
		UE_LOG( LogTemp, Error, TEXT( "No pointer to the game mode." ) );
	}
	else
	{
		GameMode->ChosenCharacterEvent.AddDynamic( this, &AUI_Characters::CharacterChosen );
	}
	
}

// Called every frame
void AUI_Characters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUI_Characters::CharacterChosen()
{
	Destroy();
}

