// Fill out your copyright notice in the Description page of Project Settings.

#include "UIParent.h"
#include "Ace_LightningCharacter.h"
#include "Ace_LightningGameMode.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"	
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "SaveData.h"
#include "TimerManager.h"

void UUIParent::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast<AAce_LightningGameMode>( GetWorld()->GetAuthGameMode() );
	GameMode->PickupEvent.AddDynamic( this, &UUIParent::PickUpCollected );
	GameMode->XP_Event.AddDynamic( this, &UUIParent::GainXP );
	GameMode->PopUpEvent.AddDynamic( this, &UUIParent::OutOfSpecial );
	GameMode->LootBagEvent.AddDynamic( this, &UUIParent::LootCollected );
	GameMode->Event_ActivateAbility.AddDynamic( this, &UUIParent::AbilityCooldown );
	GameMode->SaveEvent.AddDynamic( this, &UUIParent::SaveGame );
	GameMode->QuestEvent.AddDynamic( this, &UUIParent::QuestUpdate );

	CloseButton->OnClicked.AddDynamic( this, &UUIParent::CloseLootBag );

	SpecialNotification->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kEmptyValue ) );
	LevelUpNotification->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kEmptyValue ) );

	Level2->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kEmptyValue ) );

	LootBagBG->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kEmptyValue ) );
	GoldLootBagIcon->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kEmptyValue ) );
	GoldValueText->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kEmptyValue ) );
	CloseButton->SetVisibility( ESlateVisibility::Hidden );

	for ( int i = 0; i < CoolDownRates.Num(); ++i )
	{
		UProgressBar* ability = GetAbility( i );
		ability->SetPercent( 1.f );
	}

	Max_XP = kFullValue;
	PlayerLevel = 1;

	Player = Cast<AAce_LightningCharacter>( UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 ) );

	LoadGame();
}

void UUIParent::NativeTick( const FGeometry &MyGeometry, float DeltaTime )
{
	Super::NativeTick( MyGeometry, DeltaTime );

	if ( bFoundPlayer )
	{
		HealthBar->SetPercent( Player->CurrentHealth );
		SpecialBar->SetPercent( Player->CurrentSpecial );

		for ( int i = 0; i < CoolDownRates.Num(); i++ )
		{
			if ( GetAbility( i )->Percent < kFullValue )
			{
				// Updates the ability's cool down every frame
				float currentCooldown = GetAbility( i )->Percent;
				currentCooldown += ( DeltaTime * CoolDownRates[i] );
				GetAbility( i )->SetPercent( currentCooldown );
			}
			else
			{
				bool bIsAbilityAvailable = Player->HasCooledDown( i );

				if ( bIsAbilityAvailable )
				{
					// Sets the ability to be available	 
					GameMode->AbilityAvailable( i );
				}
			}
		}
	}
	else
	{
		Player = Cast<AAce_LightningCharacter>( UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 ) );
		bFoundPlayer = true;
	}
}

void UUIParent::OutOfSpecial( EStats stats )
{
	specialAlpha = kFullValue;
	SpecialNotification->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kFullValue ) );
	GetWorld()->GetTimerManager().SetTimer( PopUpTimer, this, &UUIParent::NoSpecial, kAnimationTime, true );
}

void UUIParent::LootCollected( int value )
{	
	GoldValue = value;
	GoldValueText->SetText( FText::AsCultureInvariant( FString::FromInt( GoldValue ) ) );
	LootBagBG->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kFullValue ) );
	GoldLootBagIcon->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kFullValue ) );
	GoldValueText->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kFullValue ) );
	CloseButton->SetVisibility( ESlateVisibility::Visible );
	
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	GetWorld()->GetFirstPlayerController()->SetPause( true );
}

void UUIParent::CloseLootBag()
{
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	GetWorld()->GetFirstPlayerController()->SetPause( false );

	LootBagBG->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kEmptyValue ) );
	GoldLootBagIcon->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kEmptyValue ) );
	GoldValueText->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kEmptyValue ) );
	CloseButton->SetVisibility( ESlateVisibility::Hidden );

	TotalGold += GoldValue;
	TotalGoldText->SetText( FText::AsCultureInvariant( FString::FromInt( TotalGold ) ) );
}

void UUIParent::AbilityCooldown( EAbilities ability )
{
	switch ( ability )
	{
		case EAbilities::First:
			Ability1->SetPercent( 0.f );
			break;
		case EAbilities::Second:
			Ability2->SetPercent( 0.f );
			break;
		case EAbilities::Third:
			Ability3->SetPercent( 0.f );
			break;
		case EAbilities::Fourth:
			Ability4->SetPercent( 0.f );
			break;
		case EAbilities::Fifth:
			Ability5->SetPercent( 0.f );
			break;
		case EAbilities::Sixth:
			Ability6->SetPercent( 0.f );
			break;
		case EAbilities::Seventh:
			Ability7->SetPercent( 0.f );
			break;
		case EAbilities::Eighth:
			Ability8->SetPercent( 0.f );
			break;
		case EAbilities::Nineth:
			Ability9->SetPercent( 0.f );
			break;
		case EAbilities::Tenth:
			Ability10->SetPercent( 0.f );
			break;
		default:
			break;
	}
}

void UUIParent::NoSpecial()
{
	float newAlpha = SpecialNotification->ColorAndOpacity.GetSpecifiedColor().A;

	if ( newAlpha > kEmptyValue )
	{
		newAlpha -= AlphaRate;
		SpecialNotification->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, newAlpha ) );
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer( PopUpTimer );
	}
}

void UUIParent::LevelUp()
{
	float newAlpha = LevelUpNotification->ColorAndOpacity.A;

	if ( newAlpha > kEmptyValue )
	{
		newAlpha -= AlphaRate;
		LevelUpNotification->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, newAlpha ) );
		Level2->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, newAlpha ) );
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer( PopUpTimer );
	}
}

void UUIParent::LoadGame()
{
	if ( UGameplayStatics::DoesSaveGameExist( "Slot1", 0 ) )
	{
		SaveData = Cast<USaveData>( UGameplayStatics::LoadGameFromSlot( "Slot1", 0 ) );
		ExperienceBar->SetPercent( SaveData->XP_Progress );
		Max_XP = SaveData->MaxXP;
		PlayerLevel = SaveData->CurrentLevel;
		Level1->SetText( FText::AsCultureInvariant( FString::FromInt( PlayerLevel ) ) );
		Level2->SetText( FText::AsCultureInvariant( FString::FromInt( PlayerLevel ) ) );
		Level3->SetText( FText::AsCultureInvariant( FString::FromInt( PlayerLevel ) ) );
		TotalGold = SaveData->TotalGold;
		TotalGoldText->SetText( FText::AsCultureInvariant( FString::FromInt( TotalGold ) ) );
	}
	else
	{
		SaveData = Cast<USaveData>( UGameplayStatics::CreateSaveGameObject( USaveData::StaticClass() ) );
	}
}

void UUIParent::SaveGame()
{
	if ( UGameplayStatics::DoesSaveGameExist( "Slot1", 0 ) )
	{
		SaveData = Cast<USaveData>( UGameplayStatics::LoadGameFromSlot( "Slot1", 0 ) );
		SaveData->XP_Progress = ExperienceBar->Percent;
		SaveData->MaxXP = Max_XP;
		SaveData->CurrentLevel = PlayerLevel;
		SaveData->TotalGold = TotalGold;

		UGameplayStatics::SaveGameToSlot( SaveData, "Slot1", 0 );
	}
	else
	{
		UE_LOG( LogTemp, Error, TEXT( "No Save Data" ) );
	}
}

void UUIParent::QuestUpdate( FString title, FString details )
{
	QuestTitle = title;
	QuestDetails = details;
}

UProgressBar* UUIParent::GetAbility( int index )
{
	UProgressBar* ability = nullptr;

	switch ( index )
	{
		 case 0:
			 ability = Ability1;
			 break;
		 case 1:
			 ability = Ability2;
			 break;
		 case 2:
			 ability = Ability3;
			 break;
		 case 3:
			 ability = Ability4;
			 break;
		 case 4:
			 ability = Ability5;
			 break;
		 case 5:
			 ability = Ability6;
			 break;
		 case 6:
			 ability = Ability7;
			 break;
		 case 7:
			 ability = Ability8;
			 break;
		 case 8:
			 ability = Ability9;
			 break;
		 case 9:
			 ability = Ability10;
			 break;
	}

	return ability;
}

void UUIParent::PickUpCollected( EStats stats, float value )
{
	if ( stats == EStats::Health )
	{
		float percent = HealthBar->Percent;
		percent += value;

		if ( percent > 1.f )
		{
			percent = 1.f;
		}

		HealthBar->SetPercent( percent );
	}
	else
	{
		float percent = SpecialBar->Percent;
		percent += value;

		if ( percent > 1.f )
		{
			percent = 1.f;
		}

		SpecialBar->SetPercent( percent );
	}
}

void UUIParent::GainXP( float value )
{
	float percent = ExperienceBar->Percent;
	percent += value;

	float currentProgress = percent / Max_XP;
	if ( Max_XP > kFullValue )
	{
		float difference = Max_XP - kFullValue;
		if ( currentProgress >= difference )
		{
			currentProgress -= difference;
		}
		else
		{
			currentProgress -= 0.01f;
		}
	}

	if ( currentProgress >= kFullValue )
	{
		PlayerLevel++;
		Level1->SetText( FText::AsCultureInvariant( FString::FromInt( PlayerLevel ) ) );
		Level2->SetText( FText::AsCultureInvariant( FString::FromInt( PlayerLevel ) ) );
		Level3->SetText( FText::AsCultureInvariant( FString::FromInt( PlayerLevel ) ) );
		LevelUpNotification->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kFullValue ) );
		GetWorld()->GetTimerManager().SetTimer( PopUpTimer, this, &UUIParent::LevelUp, kAnimationTime, true );
		currentProgress = kEmptyValue;
		Max_XP *= kXPModifier;
	}

	ExperienceBar->SetPercent( currentProgress );
}
