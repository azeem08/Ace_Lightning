// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MessageClass.h"
#include "Ace_LightningGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FPickupDelegate, EStats, stats, float, value );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FQuestDelegate, FString, title, FString, details );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FExperienceDelegate, float, value );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FPopUpDelegate, EStats, stats );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FLootBagDelegate, int, value );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FActivateDelegate, EAbilities, ability );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FStopDelegate, EAbilities, ability );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FAvailabilityDelegate, int, ability );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FSaveDelegate );

UCLASS(minimalapi)
class AAce_LightningGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
											AAce_LightningGameMode();
	// delegate for pickups
	UPROPERTY( BlueprintAssignable, Category = Event )
	FPickupDelegate							PickupEvent;

	// delegate for quests
	UPROPERTY( BlueprintAssignable, Category = Event )
	FQuestDelegate							QuestEvent;

	// delegate for xp gained
	UPROPERTY( BlueprintAssignable, Category = Event )
	FExperienceDelegate						XP_Event;

	// delegate for no special popup
	UPROPERTY( BlueprintAssignable, Category = Event )
	FPopUpDelegate							PopUpEvent;

	// delegate for loot bag pick up
	UPROPERTY( BlueprintAssignable, Category = Event )
	FLootBagDelegate						LootBagEvent;

	// delegate for activating abilties
	UPROPERTY( BlueprintAssignable, Category = Event )
	FActivateDelegate						Event_ActivateAbility;

	// delegate for deactivating abilties
	UPROPERTY( BlueprintAssignable, Category = Event )
	FStopDelegate							DeactivateEvent;

	// delegate for checking availabilty of abilities
	UPROPERTY( BlueprintAssignable, Category = Event )
	FAvailabilityDelegate					AvailablityEvent;

	// delegate for saving the game
	UPROPERTY( BlueprintAssignable, Category = Event )
	FSaveDelegate							SaveEvent;

	// subclass of userwidget that displays the character select screen
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = stats )
	TSubclassOf<class UUserWidget>			MenuWidget;

	// function to broadcast pickup event
	UFUNCTION()
	void									PickUpCollected( EStats stats, float value );
	// function to broadcast quest event
	UFUNCTION()
	void									QuestUpdate( FString title, FString details );
	// function to broadcast xp event
	UFUNCTION()
	void									XP_Gained( float value );
	// function to broadcast popup events
	UFUNCTION()
	void									TriggerNotification( EStats stats );
	// function to broadcast loot bag event
	UFUNCTION()
	void									LootBagCollected( int value );
	// function to broadcast activated event
	UFUNCTION()
	void									ActivateAbility( EAbilities ability );
	// function to broadcast stop event
	UFUNCTION()
	void									StopCasting( EAbilities ability );
	// function to broadcast ability availability event
	UFUNCTION()
	void									AbilityAvailable( int ability );
	// function to broadcast save event
	UFUNCTION()
	void									SaveGame();

private:
	// Called when the game starts or when spawned
	virtual void							BeginPlay() override;

	// widget object to display the UI
	class UUserWidget*						OnScreenHUD;
};



