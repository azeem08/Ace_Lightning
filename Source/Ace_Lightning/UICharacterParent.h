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
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	FString					Text;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					HealthProgress;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					SpecialProgress;
	// Colour and opacity for no special popup
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	FLinearColor			SpecialNotification;
	// button to choose the melee character
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI", meta = ( BindWidget ) )
	class UButton*			MeleeButton;
	// button to choose the magic character
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI", meta = ( BindWidget ) )
	class UButton*			MagicButton;
	// A bunch of floats for each ability's cool down progress
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					Ability1Progress;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					Ability2Progress;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					Ability3Progress;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					Ability4Progress;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					Ability5Progress;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					Ability6Progress;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					Ability7Progress;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					Ability8Progress;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					Ability9Progress;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					Ability10Progress;

	virtual void			NativeConstruct() override;
	FString					GetText() const;
	void					SetText( FString newText );
	float					GetHealthProgress() const;
	void					SetHealthProgress( float newHealth );
	FLinearColor			GetManaNotification() const;
	void					SetManaNotification( float newRed, float newGreen, float newBlue, float newAlpha );
	float					GetSpecialProgress() const;
	void					SetSpecialProgress( float newMana );
	float					GetAbilityCooldown( int ability ) const;
	void					SetAbilityCooldown( int ability, float newCooldown );
	class UButton*			GetMeleeButton();
	class UButton*			GetMagicButton();
};
