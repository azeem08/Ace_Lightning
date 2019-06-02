// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterHUD.h"
#include "Ace_LightningGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "MagicCharacter.h"
#include "MeleeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ACharacterHUD::ACharacterHUD()
	: WidgetMagic	( nullptr )
	, WidgetMelee	( nullptr )
	, Player		( nullptr )
	, OnScreenHUD	( nullptr )
{
}

void ACharacterHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ACharacterHUD::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ACharacterHUD::Initialize()
{
	Player = GetPlayerType( UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 ) );

	// Logs an arror if there is no widget class, widget object or parent pointer
	if ( !Player )
	{
		bFoundPlayer = false;
	}
	else
	{
		bFoundPlayer = true;

		if ( !WidgetMagic || !WidgetMelee )
		{
			UE_LOG( LogTemp, Warning, TEXT( "No widget class found" ) );
		}
		else
		{
			TSubclassOf<UUserWidget> widgetClass = GetWidgetClass( Player );

			// Creates a widget based on the widget class and attaches it to the player
			OnScreenHUD = CreateWidget<UUserWidget>( GetWorld()->GetFirstPlayerController(), widgetClass );

			if ( !OnScreenHUD )
			{
				UE_LOG( LogTemp, Error, TEXT( "Failed to create widget." ) );
			}
			else
			{
				// Adds the widget to the viewport
				OnScreenHUD->AddToViewport();

				GameMode = Cast<AAce_LightningGameMode>( GetWorld()->GetAuthGameMode() );
			}
		}
	}
}

void ACharacterHUD::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if ( !bFoundPlayer )
	{
		Initialize();
	}
}