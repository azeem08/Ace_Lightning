// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageClass.h"

/**
 *
 */
class ACE_LIGHTNING_API Sender
{
private:

	// a variable to hold a message
	EMessage		Message;
	// int variable to pass along with the message
	int				IntValue;
	// float variable to pass along with the message
	float			FloatValue;
	// string variable to pass along with the message
	FString			TitleValue;
	// string variable to pass along with the message
	FString			DetailsValue;

public:
					Sender();
					~Sender();

	// Public function to send a message
	virtual void	SendAMessage( EMessage message ) = 0;
	virtual void	SendAMessage( EMessage message, int value ) = 0;
	virtual void	SendAMessage( EMessage message, float value ) = 0;
	virtual void	SendAMessage( EMessage message, FString title, FString details ) = 0;
	// Returns the message for use with the event manager
	EMessage		GetTheMessage();
	// Sets the message for use with the event manager
	void			SetMessage( EMessage message );
	// Returns the value of any ints that are needed with the message
	int				GetIntValue();
	// Sets the value of any ints that are needed with the message
	void			SetIntValue( int newValue );
	// Returns the value of any floats that are needed with the message
	float			GetFloatValue();
	// Sets the value of any floats that are needed with the message
	void			SetFloatValue( float newValue );
	// Returns the value of any Fstrings that are needed with the message
	FString			GetTitleValue();
	// Sets the value of any FStrings that are needed with the message
	void			SetTitleValue( FString newValue );
	// Returns the value of any Fstrings that are needed with the message
	FString			GetDetailsValue();
	// Sets the value of any FStrings that are needed with the message
	void			SetDetailsValue( FString newValue );
};
