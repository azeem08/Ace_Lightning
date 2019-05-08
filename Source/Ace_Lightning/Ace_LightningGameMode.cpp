// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Ace_LightningGameMode.h"
#include "Ace_LightningCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"	 
#include "MagicCharacter.h"
#include "MeleeCharacter.h"
#include "Receiver.h"
#include "Sender.h"
#include "UObject/ConstructorHelpers.h"
#include "UICharacterParent.h"

AAce_LightningGameMode::AAce_LightningGameMode()
	: MenuWidget	( nullptr )
	, Message		( EMessage::NoMessage )
	, OnScreenHUD	( nullptr )
	, UIBasePointer	( nullptr )
	, MeleeButton	( nullptr )
	, MagicButton	( nullptr )
{
}

void AAce_LightningGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Logs an error is no senders have been added
	if ( ListOfSenders.Num() < 1 )
	{
		UE_LOG( LogTemp, Error, TEXT( "No senders added." ) );
	}
	// Logs an error is no senders have been added
	if ( ListOfReceivers.Num() < 1 )
	{
		UE_LOG( LogTemp, Error, TEXT( "No receivers added." ) );
	}
	// Logs an error if an invalid sender was added
	for ( auto sender : ListOfSenders )
	{
		if ( sender == nullptr )
		{
			UE_LOG( LogTemp, Error, TEXT( "Sender missing" ) );
		}
	}
	// Logs an error if an invalid receiver was added
	for ( auto receiver : ListOfReceivers )
	{
		if ( receiver == nullptr )
		{
			UE_LOG( LogTemp, Error, TEXT( "Receiver missing" ) );
		}
	}

	if ( !BP_MagicCharacter )
	{
		UE_LOG( LogTemp, Error, TEXT( "Missing blueprint instance of magic character" ) );
	}

	if ( !BP_MeleeCharacter )
	{
		UE_LOG( LogTemp, Error, TEXT( "Missing blueprint instance of melee character" ) );
	}

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
			UIBasePointer = Cast<UUICharacterParent>( OnScreenHUD );

			if ( !UIBasePointer )
			{
				UE_LOG( LogTemp, Warning, TEXT( "Couldn't find UI base class" ) );
			}
			else
			{
				MagicButton = Cast<UButton>( UIBasePointer->GetMagicButton() );

				if ( !MagicButton )
				{
					UE_LOG( LogTemp, Warning, TEXT( "Couldn't find button for magic character" ) );
				}
				else
				{
					MagicButton->OnClicked.AddDynamic( this, &AAce_LightningGameMode::LoadMagic );
				}

				MeleeButton = Cast<UButton>( UIBasePointer->GetMeleeButton() );

				if ( !MeleeButton )
				{
					UE_LOG( LogTemp, Warning, TEXT( "Couldn't find button for magic character" ) );
				}
				else
				{
					MeleeButton->OnClicked.AddDynamic( this, &AAce_LightningGameMode::LoadMelee );
				}

				OnScreenHUD->AddToViewport();

				GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
			}
		}
	}
}

void AAce_LightningGameMode::LoadMelee()
{
	// Spawns a melee character and possess it
	FVector Location = FVector( -1173, -71, 179.5 );
	FRotator Rotation = FRotator( 0.f, 0.f, 0.f );
	FActorSpawnParameters SpawnInfo;
	AMeleeCharacter* player = GetWorld()->SpawnActor<AMeleeCharacter>( BP_MeleeCharacter, Location, Rotation, SpawnInfo );

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->Possess( player );

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	OnScreenHUD->RemoveFromViewport();
}

void AAce_LightningGameMode::LoadMagic()
{
	// Spawns a magic character and possess it
	FVector Location = FVector( -1173, -71, 179.5 );
	FRotator Rotation = FRotator( 0.f, 0.f, 0.f );
	FActorSpawnParameters SpawnInfo;
	AMagicCharacter* player = GetWorld()->SpawnActor<AMagicCharacter>( BP_MagicCharacter, Location, Rotation, SpawnInfo );

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->Possess( player );

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	OnScreenHUD->RemoveFromViewport();
}

void AAce_LightningGameMode::AddSender( Sender * sender )
{
	if ( ListOfSenders.Num() != 0 )
	{
		// Loops through the array of senders and if it hasn't been added add it to the list
		for ( int iLoop = 0; iLoop < ListOfSenders.Num(); iLoop++ )
		{
			// Checks the sender isn't already added before
			if ( ListOfSenders[iLoop] == sender )
			{
				UE_LOG( LogTemp, Error, TEXT( "Sender already exists" ) );
				return;
			}
		}

		//ListOfSenders[iLoop] = sender;
		ListOfSenders.Add( sender );
		return;
	}
	else
	{
		ListOfSenders.Add( sender );
	}
}

void AAce_LightningGameMode::RemoveSender( Sender * sender )
{
	// Loops through the array of senders and removes the sender from the list
	for ( int iLoop = 0; iLoop < ListOfSenders.Num(); iLoop++ )
	{
		if ( sender == ListOfSenders[iLoop] )
		{
			ListOfSenders[iLoop] = nullptr;
			return;
		}
	}
}

void AAce_LightningGameMode::AddReceiver( Receiver * receiver )
{
	if ( ListOfReceivers.Num() != 0 )
	{
		// Loops through the array of receivers and if it hasn't been added add it to the list
		for ( int iLoop = 0; iLoop < ListOfReceivers.Num(); iLoop++ )
		{
			// Checks the receiver isn't already added before
			if ( ListOfReceivers[iLoop] == receiver )
			{
				UE_LOG( LogTemp, Error, TEXT( "Receiver already exists" ) );
				return;
			}
		}

		//ListOfReceivers[iLoop] = receiver;
		ListOfReceivers.Add( receiver );
		return;
	}
	else
	{
		ListOfReceivers.Add( receiver );
	}
}

void AAce_LightningGameMode::RemoveReceiver( Receiver * receiver )
{
	// Loops through the array of receivers and removes the receiver from the list
	for ( int iLoop = 0; iLoop < ListOfReceivers.Num(); iLoop++ )
	{
		if ( receiver == ListOfReceivers[iLoop] )
		{
			ListOfReceivers[iLoop] = nullptr;
			return;
		}
	}
}

void AAce_LightningGameMode::ReadMessage()
{
	for ( int iLoop = 0; iLoop < ListOfReceivers.Num(); iLoop++ )
	{
		if ( nullptr != ListOfReceivers[iLoop] )
		{
			for ( int iSecondLoop = 0; iSecondLoop < ListOfSenders.Num(); iSecondLoop++ )
			{
				if ( nullptr != ListOfSenders[iSecondLoop] )
				{
					Message = ListOfSenders[iSecondLoop]->GetTheMessage();
					int intValue = ListOfSenders[iSecondLoop]->GetIntValue();
					float floatValue = ListOfSenders[iSecondLoop]->GetFloatValue();
					FString stringValue = ListOfSenders[iSecondLoop]->GetTitleValue();
					FString detailsValue = ListOfSenders[iSecondLoop]->GetDetailsValue();

					if ( intValue > 0 )
					{
						// Calls the int value version
						ListOfReceivers[iLoop]->ReadMessage( Message, intValue );
					}
					else if ( floatValue > 0 )
					{
						// Calls the float value version
						ListOfReceivers[iLoop]->ReadMessage( Message, floatValue );
					}
					else if ( stringValue.Compare( "" ) != 0 )
					{
						// Calls the string value version
						ListOfReceivers[iLoop]->ReadMessage( Message, stringValue, detailsValue );
					}
					else if( Message != EMessage::NoMessage )
					{
						// Loops through each receiver's and reads the message from the sender
						ListOfReceivers[iLoop]->ReadMessage( Message );
					}
				}
			}
		}
	}

	ResetMessage();
}

void AAce_LightningGameMode::ResetMessage()
{
	for ( int iLoop = 0; iLoop < ListOfSenders.Num(); iLoop++ )
	{
		if ( nullptr != ListOfSenders[iLoop] )
		{
			// Loops through each sender and resets their message
			ListOfSenders[iLoop]->SetMessage( EMessage::NoMessage );
			// Loops through each sender and resets their int value
			ListOfSenders[iLoop]->SetIntValue( 0 );
			// Loops through each sender and resets their float value
			ListOfSenders[iLoop]->SetFloatValue( 0.f );
			// Loops through each sender and resets their string value
			ListOfSenders[iLoop]->SetTitleValue( "" );
			// Loops through each sender and resets their string value
			ListOfSenders[iLoop]->SetDetailsValue( "" );
		}
	}
}
