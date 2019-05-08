// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Receiver.h"
#include "Sender.h"
#include "CharacterHUD.generated.h"

/**
 *
 */
UENUM()
enum class EAbility
{
	Ability1 = 1,
	Ability2,
	Ability3,
	Ability4,
	Ability5,
	Ability6,
	Ability7,
	Ability8,
	Ability9,
	Ability10
};

UCLASS()
class ACE_LIGHTNING_API ACharacterHUD : public AHUD, public Sender, public Receiver
{
	GENERATED_BODY()

public:
	ACharacterHUD();

	// subclass of userwidget that displays the UI for a magic character
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Widget )
	TSubclassOf<class UUserWidget>			WidgetMagic;

	// subclass of userwidget that displays the UI for a melee character
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Widget )
	TSubclassOf<class UUserWidget>			WidgetMelee;

	// a list of cooldown rates for each ability
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Widget )
	TArray<float>							CoolDownRates;

	// how much the alpha of the no mana popup should change per frame
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Widget )
	float									AlphaRate;

	/** Primary draw call for the HUD */
	virtual void							DrawHUD() override;

protected:
	virtual void							BeginPlay() override;
	void									Initialize();

private:
	virtual void							Tick( float DeltaTime ) override;
	virtual void							SendAMessage( EMessage message ) override {};
	virtual void							SendAMessage( EMessage message, int value ) override;
	virtual void							SendAMessage( EMessage message, float value ) override {};
	virtual void							SendAMessage( EMessage message, FString title, FString details ) override {};
	virtual void							ReadMessage( EMessage message ) override;
	virtual void							ReadMessage( EMessage message, int value ) override;
	virtual void							ReadMessage( EMessage message, float value ) override;
	virtual void							ReadMessage( EMessage message, FString title, FString details ) override;
	// Slowly changes the alpha back to 0
	void									ManaAvailable();
	// Slowly changes the alpha back to 0
	void									LevelUp();
	// Updates the save data with new values
	void									UpdateSaveData();

	// Template that takes an actor and returns a pointer to the correct character type
	template <class PlayerType>
	PlayerType* GetPlayerType( PlayerType* a )
	{
		AActor* actor = a;
		PlayerType* currentType = nullptr;

		if ( actor != nullptr )
		{
			if ( actor->IsA( AMagicCharacter::StaticClass() ) )
			{
				currentType = Cast<AMagicCharacter>( a );
			}
			else if ( actor->IsA( AMeleeCharacter::StaticClass() ) )
			{
				currentType = Cast<AMeleeCharacter>( a );
			}
		}

		return currentType;
	}
	// Template that returns the correct widget class based on the type of character
	template <class PlayerType>
	TSubclassOf<UUserWidget> GetWidgetClass( PlayerType* a )
	{
		AActor* actor = a;
		TSubclassOf<UUserWidget> currentWidget = nullptr;

		if ( actor->IsA( AMagicCharacter::StaticClass() ) )
		{
			currentWidget = WidgetMagic;
		}
		else if ( actor->IsA( AMeleeCharacter::StaticClass() ) )
		{
			currentWidget = WidgetMelee;
		}

		return currentWidget;
	}
	// template that gets the current progress bar value based on the character type and progress bar
	template <class PlayerType>
	float GetProgress( PlayerType* a, FString message )
	{
		AActor* actor = a;
		float currentValue = 0.f;

		if ( actor->IsA( AMagicCharacter::StaticClass() ) )
		{
			AMagicCharacter* magicCharacter = Cast<AMagicCharacter>( a );

			if ( message.Compare( "Health" ) == 0 )
			{
				currentValue = magicCharacter->GetCurrentHealth();
			}
			else if ( message.Compare( "Special" ) == 0 )
			{
				currentValue = magicCharacter->GetCurrentMana();
			}
		}
		else if ( actor->IsA( AMeleeCharacter::StaticClass() ) )
		{
			AMeleeCharacter* meleeCharacter = Cast<AMeleeCharacter>( a );

			if ( message.Compare( "Health" ) == 0 )
			{
				currentValue = meleeCharacter->GetCurrentHealth();
			}
			else if ( message.Compare( "Special" ) == 0 )
			{
				currentValue = meleeCharacter->GetCurrentStamina();
			}
		}

		return currentValue;
	}
	// template class that checks if an ability is availble or not based on the type of character
	template <class PlayerType>
	bool HasCooledDown( PlayerType* a, int ability )
	{
		AActor* actor = a;
		bool bIsAvailable = false;

		if ( actor->IsA( AMagicCharacter::StaticClass() ) )
		{
			AMagicCharacter* magicCharacter = Cast<AMagicCharacter>( a );

			bIsAvailable = magicCharacter->HasCooledDown( ability );
		}
		else if ( actor->IsA( AMeleeCharacter::StaticClass() ) )
		{
			AMeleeCharacter* meleeCharacter = Cast<AMeleeCharacter>( a );

			bIsAvailable = meleeCharacter->HasCooledDown( ability );
		}

		return bIsAvailable;
	}

	// pointer to the game mode for the messaging system
	class AAce_LightningGameMode*			GameMode;
	// pointer to the player
	AActor*									Player;
	// widget object to display the UI
	class UUserWidget*						OnScreenHUD;
	// pointer to the parent class
	class UUIParent*						UIBasePointer;
	// ability progress index
	int										AbilityIndex;
	// timer for when the no mana popup should vanish
	FTimerHandle							NoManaTimer;
	// a bool to check if the player was found in beginplay
	bool									bFoundPlayer;

	// full alpha / cooldown value should never have to change
	const float								kFullValue = 1.f;
	// how often a timer function should be called a frame should never have to change
	const float								kAnimationTime = 0.1f;
	// max XP modifier should never have to change
	const float								kXPModifier = 1.5f;
	// empty alpha / cooldown value should never have to change
	const float								kEmptyValue = 0.f;
};
