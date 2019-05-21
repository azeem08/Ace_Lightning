// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UICharacterParent.generated.h"

/**
 * 
 */
UCLASS()
class ACE_LIGHTNING_API UUICharacterParent : public UUserWidget
{
	GENERATED_BODY()

private:
	// public variables that are bound to the widget blueprint
	// this allows it to be changed anywhere in code
	// button to choose the melee character
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UButton*							MeleeButton;
	// text for the melee button
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UTextBlock*						MeleeText;
	// button to choose the magic character
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UButton*							MagicButton;
	// text for the magic button
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UTextBlock*						MagicText;
	// 2D capture render
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UImage*							CharacterRenders;
	// button to reset save data
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UButton*							ResetButton;
	// text for the reset button
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI, meta = ( BindWidget, AllowPrivateAccess = "True" ) )
	class UTextBlock*						ResetText;
	// magic character blueprint
	UPROPERTY( Category = Stats, EditAnywhere )
	TSubclassOf<class AMagicCharacter>		BP_MagicCharacter;
	// melee character blueprint
	UPROPERTY( Category = Stats, EditAnywhere )
	TSubclassOf<class AMeleeCharacter>		BP_MeleeCharacter;

	virtual void							NativeConstruct() override;

	UFUNCTION()
	void									LoadMelee();
	UFUNCTION()
	void									LoadMagic();
	UFUNCTION()
	void									ClearSave();

	// pointer to messaging system
	class AAce_LightningGameMode*				GameMode;
};
