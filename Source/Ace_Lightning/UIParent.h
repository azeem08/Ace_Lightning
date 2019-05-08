// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIParent.generated.h"

/**
 * 
 */
UCLASS()
class ACE_LIGHTNING_API UUIParent : public UUserWidget
{
	GENERATED_BODY()

public:
	// public variables that are bound to the widget blueprint
	// this allows it to be changed anywhere in code
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	FString					QuestTitle;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	FString					QuestDetails;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					HealthProgress;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	float					SpecialProgress;
	// xp progress to bind to the UI
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	float					XP_Progress;
	// how much experience the player has
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	float					CurrentXP;
	// how much experience the player needs to level up
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	float					MaxXP;
	// what level the player is
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	int						CurrentLevel;
	// how much gold the player has
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	int						GoldValue;
	// gold text for binding to the UI
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	FText					GoldText;
	// level text for binding to the UI
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = UI )
	FText					Level;
	// Colour and opacity for no special popup
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	FLinearColor			SpecialNotification;
	// Colour and opacity for level popup
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "UI" )
	FLinearColor			LevelUpNotification;
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
	FString					GetQuestTitle() const;
	void					SetQuestTitle( FString newText );
	FString					GetQuestDetails() const;
	void					SetQuestDetails( FString newText );
	float					GetHealthProgress() const;
	void					SetHealthProgress( float newHealth );
	FLinearColor			GetManaNotification() const;
	void					SetManaNotification( float newRed, float newGreen, float newBlue, float newAlpha );
	FLinearColor			GetLevelNotification() const;
	void					SetLevelNotification( float newRed, float newGreen, float newBlue, float newAlpha );
	float					GetSpecialProgress() const;
	void					SetSpecialProgress( float newMana );
	float					GetProgressXP() const;
	void					SetProgressXP( float newXP );
	float					GetCurrentXP() const;
	void					SetCurrentXP( float newXP );
	float					GetMaxXP() const;
	void					SetMaxXP( float newXP );
	int						GetCurrentLevel() const;
	void					SetCurrentLevel( int newLevel );
	float					GetAbilityCooldown( int ability ) const;
	void					SetAbilityCooldown( int ability, float newCooldown );
	int						GetGoldAmount() const;
	void					SetGoldAmount( int newValue );

private:
	void					UpdateLevelText();
	void					UpdateGoldText();
};
