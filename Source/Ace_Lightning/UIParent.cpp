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
	GameMode->InventoryEvent.AddDynamic( this, &UUIParent::ShowInventory );
	GameMode->ItemEvent.AddDynamic( this, &UUIParent::ItemCollected );

	CloseButton->OnClicked.AddDynamic( this, &UUIParent::CloseLootBag );
	CloseInventoryButton->OnClicked.AddDynamic( this, &UUIParent::CloseInventory );

	SpecialNotification->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kEmptyValue ) );
	LevelUpNotification->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kEmptyValue ) );

	Level2->SetColorAndOpacity( FLinearColor( kFullValue, kFullValue, kFullValue, kEmptyValue ) );

	InventoryIconID.SetNum( 8 );

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

void UUIParent::NativeDestruct()
{
	Super::NativeDestruct();

	if ( GetWorld()->GetTimerManager().IsTimerActive( PopUpTimer ) )
	{
		GetWorld()->GetTimerManager().ClearTimer( PopUpTimer );
	}

	CloseButton->OnClicked.RemoveDynamic( this, &UUIParent::CloseLootBag );
	CloseInventoryButton->OnClicked.RemoveDynamic( this, &UUIParent::CloseInventory );

	GameMode->PickupEvent.RemoveDynamic( this, &UUIParent::PickUpCollected );
	GameMode->XP_Event.RemoveDynamic( this, &UUIParent::GainXP );
	GameMode->PopUpEvent.RemoveDynamic( this, &UUIParent::OutOfSpecial );
	GameMode->LootBagEvent.RemoveDynamic( this, &UUIParent::LootCollected );
	GameMode->Event_ActivateAbility.RemoveDynamic( this, &UUIParent::AbilityCooldown );
	GameMode->SaveEvent.RemoveDynamic( this, &UUIParent::SaveGame );
	GameMode->QuestEvent.RemoveDynamic( this, &UUIParent::QuestUpdate );
	GameMode->InventoryEvent.RemoveDynamic( this, &UUIParent::ShowInventory );
	GameMode->ItemEvent.RemoveDynamic( this, &UUIParent::ItemCollected );
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
	LootBagBG->SetVisibility( ESlateVisibility::Visible );
	GoldLootBagIcon->SetVisibility( ESlateVisibility::Visible );
	GoldValueText->SetVisibility( ESlateVisibility::Visible );
	CloseButton->SetVisibility( ESlateVisibility::Visible );
	
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	GetWorld()->GetFirstPlayerController()->SetPause( true );
}

void UUIParent::CloseLootBag()
{
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	GetWorld()->GetFirstPlayerController()->SetPause( false );

	LootBagBG->SetVisibility( ESlateVisibility::Hidden );
	GoldLootBagIcon->SetVisibility( ESlateVisibility::Hidden );
	GoldValueText->SetVisibility( ESlateVisibility::Hidden );
	CloseButton->SetVisibility( ESlateVisibility::Hidden );

	TotalGold += GoldValue;
	TotalGoldText->SetText( FText::AsCultureInvariant( FString::FromInt( TotalGold ) ) );
}

void UUIParent::ShowInventory()
{
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	InventoryBG->SetVisibility( ESlateVisibility::Visible );
	CloseInventoryButton->SetVisibility( ESlateVisibility::Visible );
	InventoryIcon1->SetVisibility( ESlateVisibility::Visible );
	InventoryIcon2->SetVisibility( ESlateVisibility::Visible );
	InventoryIcon3->SetVisibility( ESlateVisibility::Visible );
	InventoryIcon4->SetVisibility( ESlateVisibility::Visible );
	InventoryIcon5->SetVisibility( ESlateVisibility::Visible );
	InventoryIcon6->SetVisibility( ESlateVisibility::Visible );
	InventoryIcon7->SetVisibility( ESlateVisibility::Visible );
	InventoryIcon8->SetVisibility( ESlateVisibility::Visible );
}

void UUIParent::CloseInventory()
{
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	InventoryBG->SetVisibility( ESlateVisibility::Hidden );
	CloseInventoryButton->SetVisibility( ESlateVisibility::Hidden );
	InventoryIcon1->SetVisibility( ESlateVisibility::Hidden );
	InventoryIcon2->SetVisibility( ESlateVisibility::Hidden );
	InventoryIcon3->SetVisibility( ESlateVisibility::Hidden );
	InventoryIcon4->SetVisibility( ESlateVisibility::Hidden );
	InventoryIcon5->SetVisibility( ESlateVisibility::Hidden );
	InventoryIcon6->SetVisibility( ESlateVisibility::Hidden );
	InventoryIcon7->SetVisibility( ESlateVisibility::Hidden );
	InventoryIcon8->SetVisibility( ESlateVisibility::Hidden );
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

UImage* UUIParent::GetBagSlot( int index )
{
	UImage* slot = nullptr;

	switch ( index )
	{
		 case 0:
			 slot = InventoryIcon1;
			 break;
		 case 1:
			 slot = InventoryIcon2;
			 break;
		 case 2:
			 slot = InventoryIcon3;
			 break;
		 case 3:
			 slot = InventoryIcon4;
			 break;
		 case 4:
			 slot = InventoryIcon5;
			 break;
		 case 5:
			 slot = InventoryIcon6;
			 break;
		 case 6:
			 slot = InventoryIcon7;
			 break;
		 case 7:
			 slot = InventoryIcon8;
			 break;
	}

	return slot;
}

UTexture2D* UUIParent::GetBagTexture( int id )
{
	UTexture2D* item = nullptr;
	switch ( id )
	{
		case 0:
			item = nullptr;
			break;
		case 1:
			item = Key;
			break;
		case 2:
			item = Box;
			break;
		case 3:
			item = Wood;
			break;
		default:
			break;
	}

	return item;
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

void UUIParent::ItemCollected( int value )
{
	for ( auto item : InventoryIconID )
	{
		if ( item == 0 )
		{
			GetBagSlot( item )->SetBrushFromTexture( GetBagTexture( value ) );
			item = value;
			return;
		}
	}

	//for ( int i = 0; i < 7; ++i )
	//{
	//	if ( InventoryIconID[i] == 0 )
	//	{
	//		GetBagSlot( i )->SetBrushFromTexture( GetBagTexture( value ) );
	//		InventoryIconID[i] = value;
	//		return;
	//	}
	//}
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
