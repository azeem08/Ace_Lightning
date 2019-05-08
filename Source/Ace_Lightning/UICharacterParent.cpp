// Fill out your copyright notice in the Description page of Project Settings.

#include "UICharacterParent.h"
#include "Components/Button.h"	

void UUICharacterParent::NativeConstruct()
{
	Super::NativeConstruct();
}

FString UUICharacterParent::GetText() const
{
	return Text;
}

void UUICharacterParent::SetText( FString newText )
{
	Text = newText;
}

float UUICharacterParent::GetHealthProgress() const
{
	return HealthProgress;
}

void UUICharacterParent::SetHealthProgress( float newHealth )
{
	HealthProgress = newHealth;
}

FLinearColor UUICharacterParent::GetManaNotification() const
{
	return SpecialNotification;
}

void UUICharacterParent::SetManaNotification( float newRed, float newGreen, float newBlue, float newAlpha )
{
	SpecialNotification.R = newRed;
	SpecialNotification.G = newGreen;
	SpecialNotification.B = newBlue;
	SpecialNotification.A = newAlpha;
}

float UUICharacterParent::GetSpecialProgress() const
{
	return SpecialProgress;
}

void UUICharacterParent::SetSpecialProgress( float newMana )
{
	SpecialProgress = newMana;
}

float UUICharacterParent::GetAbilityCooldown( int ability ) const
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

void UUICharacterParent::SetAbilityCooldown( int ability, float newCooldown )
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

UButton* UUICharacterParent::GetMeleeButton()
{
	return MeleeButton;
}

UButton* UUICharacterParent::GetMagicButton()
{
	return MagicButton;
}
