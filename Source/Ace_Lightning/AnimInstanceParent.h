// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MessageClass.h"
#include "AnimInstanceParent.generated.h"

/**
 *
 */
UCLASS()
class ACE_LIGHTNING_API UAnimInstanceParent : public UAnimInstance
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
	UFUNCTION()
	void								StartAnimating( EAbilities ability );
	// Delegate function for finishing an ability animation
	UFUNCTION()
	void								StopAnimating( EAbilities ability );

	class AAce_LightningGameMode*		GameMode;
	class AAce_LightningCharacter*		Player;
};
