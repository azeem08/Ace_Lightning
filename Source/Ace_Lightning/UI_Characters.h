// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UI_Characters.generated.h"

UCLASS()
class ACE_LIGHTNING_API AUI_Characters : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
											AUI_Characters();
	// To act as the root component
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Actor )
	class UBoxComponent*					Box;
	// Magic character render
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Actor )
	class USkeletalMeshComponent*			MagicCharacter;
	// Melee character render
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Actor )
	class USkeletalMeshComponent*			MeleeCharacter;	
	// Camera to capture the scene
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Actor )
	class UCameraComponent*					Camera;	
	// 2D render of the capture
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Actor )
	class USceneCaptureComponent2D*			Capture2D;	

private:	
	// Called when the game starts or when spawned
	virtual void							BeginPlay() override;
	// Called every frame
	virtual void							Tick(float DeltaTime) override;

	// Delegate function for when a character is chosen
	UFUNCTION()
	void									CharacterChosen();

	// pointer to messaging system
	class AAce_LightningGameMode*			GameMode;

};
