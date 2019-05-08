// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageClass.h"

/**
 *
 */

class ACE_LIGHTNING_API Receiver
{
public:
					Receiver();
					~Receiver();

	// Reads the message and carries out the specific functions
	virtual void	ReadMessage( EMessage message ) = 0;
	virtual void	ReadMessage( EMessage message, int value ) = 0;
	virtual void	ReadMessage( EMessage message, float value ) = 0;
	virtual void	ReadMessage( EMessage message, FString title, FString details ) = 0;
};
