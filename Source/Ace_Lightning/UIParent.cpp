// Fill out your copyright notice in the Description page of Project Settings.

#include "UIParent.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"	

void UUIParent::NativeConstruct()
{
	Super::NativeConstruct();
}

FString UUIParent::GetQuestTitle() const
{
	return QuestTitle;
}

void UUIParent::SetQuestTitle( FString newText )
{
	QuestTitle = newText;
}

FString UUIParent::GetQuestDetails() const
{
	return QuestDetails;
}

void UUIParent::SetQuestDetails( FString newText )
{
	QuestDetails = newText;
}

float UUIParent::GetHealthProgress() const
{
	return HealthProgress;
}

void UUIParent::SetHealthProgress( float newHealth )
{
	HealthProgress = newHealth;
}

FLinearColor UUIParent::GetManaNotification() const
{
	return SpecialNotification;
}

void UUIParent::SetManaNotification( float newRed, float newGreen, float newBlue, float newAlpha )
{
	SpecialNotification.R = newRed;
	SpecialNotification.G = newGreen;
	SpecialNotification.B = newBlue;
	SpecialNotification.A = newAlpha;
}

FLinearColor UUIParent::GetLevelNotification() const
{
	return LevelUpNotification;
}

void UUIParent::SetLevelNotification( float newRed, float newGreen, float newBlue, float newAlpha )
{
	LevelUpNotification.R = newRed;
	LevelUpNotification.G = newGreen;
	LevelUpNotification.B = newBlue;
	LevelUpNotification.A = newAlpha;
}

float UUIParent::GetSpecialProgress() const
{
	return SpecialProgress;
}

void UUIParent::SetSpecialProgress( float newMana )
{
	SpecialProgress = newMana;
}

float UUIParent::GetProgressXP() const
{
	return XP_Progress;
}

void UUIParent::SetProgressXP( float newXP )
{
	XP_Progress = newXP;
}

float UUIParent::GetCurrentXP() const
{
	return CurrentXP;
}

void UUIParent::SetCurrentXP( float newXP )
{
	CurrentXP = newXP;
}

float UUIParent::GetMaxXP() const
{
	return MaxXP;
}

void UUIParent::SetMaxXP( float newXP )
{
	MaxXP = newXP;
}

int UUIParent::GetCurrentLevel() const
{
	return CurrentLevel;
}

void UUIParent::SetCurrentLevel( int newLevel )
{
	CurrentLevel = newLevel;
	UpdateLevelText();
}

float UUIParent::GetAbilityCooldown( int ability ) const
{
	float cooldownRate = 0.f;

	switch ( ability )
	{
		case 1:
			cooldownRate = Ability1Progress;
			break;
		case 2:
			cooldownRate = Ability2Progress;
			break;
		case 3:
			cooldownRate = Ability3Progress;
			break;
		case 4:
			cooldownRate = Ability4Progress;
			break;
		case 5:
			cooldownRate = Ability5Progress;
			break;
		case 6:
			cooldownRate = Ability6Progress;
			break;
		case 7:
			cooldownRate = Ability7Progress;
			break;
		case 8:
			cooldownRate = Ability8Progress;
			break;
		case 9:	  
			cooldownRate = Ability9Progress;
			break;
		case 10:
			cooldownRate = Ability10Progress;
			break;
		default:
			cooldownRate = Ability1Progress;
			break;
	}

	return cooldownRate;
}

void UUIParent::SetAbilityCooldown( int ability, float newCooldown )
{
	switch ( ability )
	{
		case 1:
			Ability1Progress = newCooldown;
			break;
		case 2:
			Ability2Progress = newCooldown;
			break;
		case 3:
			Ability3Progress = newCooldown;
			break;
		case 4:
			Ability4Progress = newCooldown;
			break;
		case 5:
			Ability5Progress = newCooldown;
			break;
		case 6:
			Ability6Progress = newCooldown;
			break;
		case 7:
			Ability7Progress = newCooldown;
			break;
		case 8:
			Ability8Progress = newCooldown;
			break;
		case 9:
			Ability9Progress = newCooldown;
			break;
		case 10:
			Ability10Progress = newCooldown;
			break;
		default:
			Ability1Progress = newCooldown;
			break;
	}
}

void UUIParent::UpdateLevelText()
{
	Level = FText::AsCultureInvariant( FString::FromInt( CurrentLevel ) );
}
