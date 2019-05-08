// Fill out your copyright notice in the Description page of Project Settings.

#include "Sender.h"

Sender::Sender()
	: Message		( EMessage::NoMessage )
	, IntValue		( 0 )
	, FloatValue	( 0.f )
	, TitleValue	( "" )
	, DetailsValue	( "" )
{
}

Sender::~Sender()
{
}

EMessage Sender::GetTheMessage()
{
	return Message;
}

void Sender::SetMessage( EMessage message )
{
	Message = message;
}

int Sender::GetIntValue()
{
	return IntValue;
}

void Sender::SetIntValue( int newValue )
{
	IntValue = newValue;
}

float Sender::GetFloatValue()
{
	return FloatValue;
}

void Sender::SetFloatValue( float newValue )
{
	FloatValue = newValue;
}

FString Sender::GetTitleValue()
{
	return TitleValue;
}

void Sender::SetTitleValue( FString newValue )
{
	TitleValue = newValue;
}

FString Sender::GetDetailsValue()
{
	return DetailsValue;
}

void Sender::SetDetailsValue( FString newValue )
{
	DetailsValue = newValue;
}
