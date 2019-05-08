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

public:
	// public variables that are bound to the widget blueprint
	// this allows it to be changed anywhere in code

	// button to choose the melee character
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI", meta = ( BindWidget ) )
	class UButton*			MeleeButton;
	// button to choose the magic character
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI", meta = ( BindWidget ) )
	class UButton*			MagicButton;
	// button to reset save data
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI", meta = ( BindWidget ) )
	class UButton*			ResetButton;

	virtual void			NativeConstruct() override;
	class UButton*			GetMeleeButton();
	class UButton*			GetMagicButton();
	class UButton*			GetResetButton();
};
