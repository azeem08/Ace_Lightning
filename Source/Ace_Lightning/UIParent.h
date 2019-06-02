// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageClass.h"
#include "UIParent.generated.h"

/**
 * 
 */
UCLASS()
class ACE_LIGHTNING_API UUIParent : public UUserWidget
{
	GENERATED_BODY()

public:
	 // Icon for picked up key
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	class UTexture2D*				Key;
	 // Icon for picked up box
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	class UTexture2D*				Box;
	 // Icon for picked up wood
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	class UTexture2D*				Wood;

private:
	// public variables that are bound to the widget blueprint
	// this allows it to be changed anywhere in code
	// Health bar 
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget, AllowPrivateAccess = "True") )
	class UProgressBar*				HealthBar;
	// Stamina/Mana bar
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget, AllowPrivateAccess = "True" ) )
	class UProgressBar*				SpecialBar;
	// Text popup for out of mana/stamina
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UTextBlock*				SpecialNotification;
	// XP bar
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget, AllowPrivateAccess = "True" ) )
	class UProgressBar*				ExperienceBar;
	// Text to represent the players level
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UTextBlock*				Level1;
	// Text to represent the players level
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UTextBlock*				Level2;
	// Text to represent the players level
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UTextBlock*				Level3;
	// Image popup for levelup
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					LevelUpNotification;
	// how much the alpha of the no mana popup should change per frame
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( AllowPrivateAccess = "True" ) )
	float							AlphaRate;
	// how much gold the player has
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UTextBlock*				TotalGoldText;
	// Image for the gold icon in the loot bag
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					GoldLootBagIcon;
	// Image for the background in the loot bag
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					LootBagBG;
	// how much gold is in the loot bag
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UTextBlock*				GoldValueText;
	// Image for the item in the loot bag
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					LootItem;
	// Description of the item in the loot bag
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UTextBlock*				LootDescription;
	// button to close the loot bag
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UButton*					CloseButton;
	// Image for the background of the inventory
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					InventoryBG;
	// button to close the inventory
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UButton*					CloseInventoryButton;
	// Image for the inventory icon
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					InventoryIcon1;
	// Image for the inventory icon
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					InventoryIcon2;
	// Image for the inventory icon
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					InventoryIcon3;
	// Image for the inventory icon
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					InventoryIcon4;
	// Image for the inventory icon
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					InventoryIcon5;
	// Image for the inventory icon
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					InventoryIcon6;
	// Image for the inventory icon
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					InventoryIcon7;
	// Image for the inventory icon
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*					InventoryIcon8;
	// A bunch of progressbars for each ability's cool down progress
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UProgressBar*				Ability1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UProgressBar*				Ability2;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UProgressBar*				Ability3;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UProgressBar*				Ability4;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UProgressBar*				Ability5;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UProgressBar*				Ability6;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UProgressBar*				Ability7;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UProgressBar*				Ability8;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UProgressBar*				Ability9;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UProgressBar*				Ability10;
	// a list of cooldown rates for each ability
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	TArray<float>					CoolDownRates;
	// inventoy full notification
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UTextBlock*				InventoryNotification;
	// Text to represent quests
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	FString							QuestTitle;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	FString							QuestDetails;

	virtual void					NativeConstruct() override;
	virtual void					NativeTick( const FGeometry& MyGeometry, float DeltaTime ) override;
	virtual void					NativeDestruct() override;

	// Slowly changes the alpha back to 0
	void							NoSpecial();
	void							LevelUp();
	void							BagFull();
	// Updates the UI with data from the save data
	void							LoadGame();
	class UProgressBar*				GetAbility( int index );
	class UImage*					GetBagSlot( int index );
	class UTexture2D*				GetBagTexture( int id );
	void							SetLootDescription( int id );

	// Delegate function for when a pickup is collected
	UFUNCTION()
	void							PickUpCollected( EStats stats, float value );
	// Delegate function for when a item is collected
	UFUNCTION()
	void							ItemCollected( int value );
	// Delegate function for when XP is gained
	UFUNCTION()
	void							GainXP( float value );
	// Delegate function to trigger the no special popup
	UFUNCTION()
	void							OutOfSpecial( EStats stats );
	// Delegate function to update the total gold count
	UFUNCTION()
	void							LootCollected( int gold, int item );
	// Hides the loot bag 
	UFUNCTION()
	void							CloseLootBag();
	// Delegate function to show the inventory
	UFUNCTION()
	void							ShowInventory();
	// Hides the inventory 
	UFUNCTION()
	void							CloseInventory();
	// Delegate function to manage ability cooldown
	UFUNCTION()
	void							AbilityCooldown( EAbilities ability );
	// Delegate function to save data
	UFUNCTION()
	void							SaveGame();
	// Delegate function for when a the quest is received
	UFUNCTION()
	void							QuestUpdate( FString title, FString details );

	// pointer to the game mode
	class AAce_LightningGameMode*	GameMode;
	// pointer to the player
	class AAce_LightningCharacter*	Player;
	// pointer to the save data
	class USaveData*				SaveData;
	// timer for when the no mana popup should vanish
	FTimerHandle					PopUpTimer;
	// a bool to check if the player was found
	bool							bFoundPlayer;
	// a float to track the amount of xp needed to level up
	float							Max_XP;
	// an array of id's for inventory objects
	TArray<int>						InventoryIconID;
	// a int that needs to be converted into an FText
	int								PlayerLevel;
	// how much gold the player has
	int								TotalGold;
	// gold amount to add to the total
	int								GoldValue;	
	//
	float							specialAlpha = 1.f;

	// full alpha / cooldown value should never have to change
	const float						kFullValue = 1.f;
	// how often a timer function should be called a frame should never have to change
	const float						kAnimationTime = 0.1f;
	// max XP modifier should never have to change
	const float						kXPModifier = 1.5f;
	// empty alpha / cooldown value should never have to change
	const float						kEmptyValue = 0.f;
};
