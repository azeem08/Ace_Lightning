// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Receiver.h"
#include "AnimInstanceParent.generated.h"

/**
 *
 */
UCLASS()
class ACE_LIGHTNING_API UAnimInstanceParent : public UAnimInstance, public Receiver
{
	GENERATED_BODY()

public:
										UAnimInstanceParent();

	// public variables that are bound to the animation blueprint
	// this allows it to be changed anywhere in code
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Animation )
	float								Direction;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Animation )
	float								Speed;
	// Checks if the player is jumping
	UPROPERTY( BlueprintReadWrite, EditAnywhere )
	bool								bIsFalling;
	// Checks if the player is casting healing magic
	UPROPERTY( BlueprintReadWrite, EditAnywhere )
	bool								bIsAbility1;
	// Checks if the player is casting a 1 handed magic attack
	UPROPERTY( BlueprintReadWrite, EditAnywhere )
	bool								bIsAbility2;

	virtual void						NativeInitializeAnimation() override;
	virtual void						NativeUpdateAnimation( float DeltaTime ) override;

	float								GetDirection();
	void								SetDirection( float newDirection );
	float								GetSpeed();
	void								SetSpeed( float newSpeed );
	bool								IsFalling();
	void								SetFalling( bool newValue );
	bool								IsCastingAbility1();
	void								ActivateAbility1( bool newValue );
	bool								IsCastingAbility2();
	void								ActivateAbility2( bool newValue );
private:
	virtual void						ReadMessage( EMessage message ) override;
	virtual void						ReadMessage( EMessage message, int value ) override {};
	virtual void						ReadMessage( EMessage message, float value ) override {};
	virtual void						ReadMessage( EMessage message, FString title, FString details ) override {};

	class AAce_LightningGameMode*		GameMode;
	class AAce_LightningCharacter*		Player;
};
