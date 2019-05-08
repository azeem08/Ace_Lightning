// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MessageClass.h"
#include "Ace_LightningGameMode.generated.h"

UCLASS(minimalapi)
class AAce_LightningGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
											AAce_LightningGameMode();

	// subclass of userwidget that displays the character select screen
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = stats )
	TSubclassOf<class UUserWidget>			MenuWidget;

	// magic character blueprint
	UPROPERTY( Category = Stats, EditAnywhere )
	TSubclassOf<class AMagicCharacter>		BP_MagicCharacter;

	// melee character blueprint
	UPROPERTY( Category = Stats, EditAnywhere )
	TSubclassOf<class AMeleeCharacter>		BP_MeleeCharacter;

	// Functions to register obects as senders and receivers and to manage their messages
	void									AddSender( class Sender* sender );
	void									RemoveSender( class Sender* sender );
	void									AddReceiver( class Receiver* receiver );
	void									RemoveReceiver( class Receiver* receiver );
	void									ReadMessage();
	void									ResetMessage();

private:
	// Called when the game starts or when spawned
	virtual void							BeginPlay() override;

	UFUNCTION()
	void									LoadMelee();
	UFUNCTION()
	void									LoadMagic();
	UFUNCTION()
	void									ClearSave();

	// List of all senders and receivers
	TArray< class Sender* >					ListOfSenders;
	TArray< class Receiver* >				ListOfReceivers;

	EMessage								Message;
	// widget object to display the UI
	class UUserWidget*						OnScreenHUD;
	// pointer to the UI base class
	class UUICharacterParent*				UIBasePointer;
	// button to choose the melee character
	class UButton*							MeleeButton;
	// button to choose the magic character
	class UButton*							MagicButton;
	// button to reset save data
	class UButton*							ResetButton;
};



