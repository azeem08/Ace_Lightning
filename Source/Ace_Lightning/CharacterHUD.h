// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CharacterHUD.generated.h"

UCLASS()
class ACE_LIGHTNING_API ACharacterHUD : public AHUD
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

private:
	virtual void							Tick( float DeltaTime ) override;
	/** Primary draw call for the HUD */
	virtual void							DrawHUD() override;
	virtual void							BeginPlay() override;

	void									Initialize();

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

	// pointer to the game mode for the messaging system
	class AAce_LightningGameMode*			GameMode;
	// pointer to the player
	class AActor*							Player;
	// widget object to display the UI
	class UUserWidget*						OnScreenHUD;
	// a bool to check if the player was found in beginplay
	bool									bFoundPlayer;
};
