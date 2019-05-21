// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Ace_LightningGameMode.h"
#include "Ace_LightningCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"	 
#include "UObject/ConstructorHelpers.h"
#include "UICharacterParent.h"

AAce_LightningGameMode::AAce_LightningGameMode()
	: MenuWidget	( nullptr )
	, OnScreenHUD	( nullptr )
{
}

void AAce_LightningGameMode::BeginPlay()
{
	Super::BeginPlay();

	if ( !MenuWidget )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No Menu widget pointer" ) );
	}
	else
	{
		// Creates a widget based on the widget class and attaches it to the player
		OnScreenHUD = CreateWidget<UUserWidget>( GetWorld()->GetFirstPlayerController(), MenuWidget );

		if ( !OnScreenHUD )
		{
			UE_LOG( LogTemp, Warning, TEXT( "Couldn't make a widget" ) );
		}
		else
		{
			OnScreenHUD->AddToViewport();

			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		}
	}
}

void AAce_LightningGameMode::PickUpCollected( EStats stats, float value )
{
	PickupEvent.Broadcast( stats, value );
}

void AAce_LightningGameMode::QuestUpdate( FString title, FString details )
{
	QuestEvent.Broadcast( title, details );
}

void AAce_LightningGameMode::XP_Gained( float value )
{
	XP_Event.Broadcast( value );
}

void AAce_LightningGameMode::TriggerNotification( EStats stats )
{
	PopUpEvent.Broadcast( stats );
}

void AAce_LightningGameMode::LootBagCollected( int value )
{
	LootBagEvent.Broadcast( value );
}

void AAce_LightningGameMode::ActivateAbility( EAbilities ability )
{
	Event_ActivateAbility.Broadcast( ability );
}

void AAce_LightningGameMode::StopCasting( EAbilities ability )
{
	DeactivateEvent.Broadcast( ability );
}

void AAce_LightningGameMode::AbilityAvailable( int ability )
{
	AvailablityEvent.Broadcast( ability );
}

void AAce_LightningGameMode::ItemCollected( int id )
{
	ItemEvent.Broadcast( id );
}

void AAce_LightningGameMode::SaveGame()
{
	SaveEvent.Broadcast();
}

void AAce_LightningGameMode::ShowInventory()
{
	InventoryEvent.Broadcast();
}

void AAce_LightningGameMode::StartGame()
{
	ChosenCharacterEvent.Broadcast();
}
