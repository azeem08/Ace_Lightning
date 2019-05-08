// Fill out your copyright notice in the Description page of Project Settings.

#include "UICharacterParent.h"
#include "Components/Button.h"	

void UUICharacterParent::NativeConstruct()
{
	Super::NativeConstruct();
}

UButton* UUICharacterParent::GetMeleeButton()
{
	return MeleeButton;
}

UButton* UUICharacterParent::GetMagicButton()
{
	return MagicButton;
}

UButton* UUICharacterParent::GetResetButton()
{
	return ResetButton;
}
