// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterHUD.h"
#include "Ace_LightningGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "MagicCharacter.h"
#include "MeleeCharacter.h"
#include "UIParent.h"
#include "Kismet/GameplayStatics.h"
#include "SaveData.h"
#include "TimerManager.h"

ACharacterHUD::ACharacterHUD()
	: WidgetMagic	( nullptr )
	, WidgetMelee	( nullptr )
	, CoolDownRates	{ 0.f }
	, AlphaRate		( 0.f )
	, Player		( nullptr )
	, OnScreenHUD	( nullptr )
	, UIBasePointer	( nullptr )
{
	CoolDownRates.SetNum( 10 );
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
	USaveData* LoadGameInstance = Cast<USaveData>( UGameplayStatics::CreateSaveGameObject( USaveData::StaticClass() ) );
	LoadGameInstance = Cast<USaveData>( UGameplayStatics::LoadGameFromSlot( "test", 0 ) );

	Player = GetPlayerType( UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 ) );

	// Logs an arror if there is no widget class, widget object or parent pointer
	if ( !Player )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No player found" ) );

		bFoundPlayer = false;
	}
	else
	{
		bFoundPlayer = true;

		UE_LOG( LogTemp, Warning, TEXT( "player has been found" ) );

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
				UIBasePointer = Cast<UUIParent>( OnScreenHUD );

				if ( !UIBasePointer )
				{
					UE_LOG( LogTemp, Error, TEXT( "Failed to get UI parent." ) );
				}
				else
				{
					if ( LoadGameInstance )
					{
						if ( UIBasePointer )
						{
							UIBasePointer->SetProgressXP( LoadGameInstance->XP_Progress );
							UIBasePointer->SetMaxXP( LoadGameInstance->MaxXP );
							UIBasePointer->SetCurrentXP( LoadGameInstance->CurrentXP );
							UIBasePointer->SetCurrentLevel( LoadGameInstance->CurrentLevel );
							UIBasePointer->SetGoldAmount( LoadGameInstance->GoldValue );
						}
					}

					// Adds the widget to the viewport
					OnScreenHUD->AddToViewport();

					GameMode = Cast<AAce_LightningGameMode>( GetWorld()->GetAuthGameMode() );

					if ( !GameMode )
					{
						UE_LOG( LogTemp, Warning, TEXT( "No game mode pointer" ) );
					}
					else
					{
						GameMode->AddReceiver( this );
						GameMode->AddSender( this );

						UIBasePointer->SetHealthProgress( GetProgress( Player, "Health" ) );
						UIBasePointer->SetSpecialProgress( GetProgress( Player, "Special" ) );

						AbilityIndex = 0;
						for ( int i = 0; i < CoolDownRates.Num(); i++ )
						{
							AbilityIndex = i + 1;
							UIBasePointer->SetAbilityCooldown( AbilityIndex, kFullValue );
						}
					}
				}
			}
		}
	}
}

void ACharacterHUD::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if ( bFoundPlayer )
	{
		UIBasePointer->SetHealthProgress( GetProgress( Player, "Health" ) );
		UIBasePointer->SetSpecialProgress( GetProgress( Player, "Special" ) );

		// Goes through every ability to check if it is available or not
		AbilityIndex = 0;
		for ( int i = 0; i < CoolDownRates.Num(); i++ )
		{
			AbilityIndex = i + 1;

			if ( UIBasePointer->GetAbilityCooldown( AbilityIndex ) < kFullValue )
			{
				// Updates the ability's cool down every frame
				float currentCooldown = UIBasePointer->GetAbilityCooldown( AbilityIndex );
				currentCooldown += ( DeltaTime * CoolDownRates[i] );
				UIBasePointer->SetAbilityCooldown( AbilityIndex, currentCooldown );
			}
			else
			{
				if ( !HasCooledDown( Player, i ) )
				{
					// Sets the ability to be available
					SendAMessage( EMessage::AbilityAvailable, AbilityIndex );
				}
			}
		}
	}
	else
	{
		Initialize();
	}
}

void ACharacterHUD::SendAMessage( EMessage message, int value )
{
	SetMessage( message );
	SetIntValue( value );
	GameMode->ReadMessage();
}

void ACharacterHUD::ReadMessage( EMessage message )
{
	if ( message == EMessage::CastAbility1 )
	{
		UIBasePointer->SetAbilityCooldown( static_cast<int>( EAbility::Ability1 ), 0.f );
	}

	if ( message == EMessage::CastAbility2 )
	{
		UIBasePointer->SetAbilityCooldown( static_cast<int>( EAbility::Ability2 ), 0.f );
	}

	if ( message == EMessage::NoSpecial )
	{
		float red = UIBasePointer->GetManaNotification().R;
		float green = UIBasePointer->GetManaNotification().G;
		float blue = UIBasePointer->GetManaNotification().B;

		UIBasePointer->SetManaNotification( red, green, blue, kFullValue );
		GetWorldTimerManager().SetTimer( NoManaTimer, this, &ACharacterHUD::ManaAvailable, kAnimationTime, true );
	}
}

void ACharacterHUD::ReadMessage( EMessage message, int value )
{
	if ( message == EMessage::LootPickUp )
	{
		int currentGold = UIBasePointer->GetGoldAmount();
		currentGold += value;

		UIBasePointer->SetGoldAmount( currentGold );

		UpdateSaveData();
	}
}

void ACharacterHUD::ReadMessage( EMessage message, float value )
{
	if ( message == EMessage::EnemyKilled )
	{
		float currentXP = UIBasePointer->GetCurrentXP();
		currentXP += value;
		UIBasePointer->SetCurrentXP( currentXP );
		float maxXP = UIBasePointer->GetMaxXP();
		float progress_XP = currentXP / maxXP;
		if ( maxXP > kFullValue )
		{
			float difference = maxXP - kFullValue;
			if ( progress_XP >= difference )
			{
				progress_XP -= difference;
			}
			else
			{
				progress_XP -= 0.01f;
			}
		}
		UIBasePointer->SetProgressXP( progress_XP );

		if ( UIBasePointer->GetProgressXP() >= kFullValue )
		{
			int currentLevel = UIBasePointer->GetCurrentLevel();
			currentLevel++;
			UIBasePointer->SetCurrentLevel( currentLevel );
			UIBasePointer->SetLevelNotification( kFullValue, kFullValue, kFullValue, kFullValue );
			GetWorldTimerManager().SetTimer( NoManaTimer, this, &ACharacterHUD::LevelUp, kAnimationTime, true );

			UIBasePointer->SetCurrentXP( kEmptyValue );
			maxXP *= kXPModifier;
			UIBasePointer->SetMaxXP( maxXP );
			UIBasePointer->SetProgressXP( kEmptyValue );
		}

		UpdateSaveData();
	}
}

void ACharacterHUD::ReadMessage( EMessage message, FString title, FString details )
{
	if ( message == EMessage::NewQuest )
	{
		UIBasePointer->SetQuestTitle( title );
		UIBasePointer->SetQuestDetails( details );
	}
}

void ACharacterHUD::ManaAvailable()
{
	float newAlpha = UIBasePointer->GetManaNotification().A;

	if ( newAlpha > kEmptyValue )
	{
		newAlpha -= AlphaRate;

		UIBasePointer->SetManaNotification( kFullValue, kFullValue, kFullValue, newAlpha );
	}
	else
	{
		GetWorldTimerManager().ClearTimer( NoManaTimer );
	}
}

void ACharacterHUD::LevelUp()
{
	float red = UIBasePointer->GetLevelNotification().R;
	float green = UIBasePointer->GetLevelNotification().G;
	float blue = UIBasePointer->GetLevelNotification().B;
	float newAlpha = UIBasePointer->GetLevelNotification().A;

	if ( newAlpha > kEmptyValue )
	{
		newAlpha -= AlphaRate;

		UIBasePointer->SetLevelNotification( 1.0f, 1.0f, 1.0f, newAlpha );
	}
	else
	{
		GetWorldTimerManager().ClearTimer( NoManaTimer );
	}
}

void ACharacterHUD::UpdateSaveData()
{
	USaveData* SaveGameInstance = Cast<USaveData>( UGameplayStatics::CreateSaveGameObject( USaveData::StaticClass() ) );
	SaveGameInstance->XP_Progress = UIBasePointer->GetProgressXP();
	SaveGameInstance->CurrentXP = UIBasePointer->GetCurrentXP();
	SaveGameInstance->MaxXP = UIBasePointer->GetMaxXP();
	SaveGameInstance->CurrentLevel = UIBasePointer->GetCurrentLevel();
	SaveGameInstance->GoldValue = UIBasePointer->GetGoldAmount();
	UGameplayStatics::SaveGameToSlot( SaveGameInstance, "test", 0 );
}
