// Fill out your copyright notice in the Description page of Project Settings.

#include "UICharacterParent.h"
#include "Components/Button.h"	
#include "Components/TextBlock.h"
#include "kismet/GameplayStatics.h"
#include "MagicCharacter.h"
#include "MeleeCharacter.h"
#include "SaveData.h"

void UUICharacterParent::NativeConstruct()
{
	Super::NativeConstruct();

	if ( !BP_MagicCharacter )
	{
		UE_LOG( LogTemp, Error, TEXT( "Missing blueprint instance of magic character" ) );
	}

	if ( !BP_MeleeCharacter )
	{
		UE_LOG( LogTemp, Error, TEXT( "Missing blueprint instance of melee character" ) );
	}

	MeleeButton->OnClicked.AddDynamic( this, &UUICharacterParent::LoadMelee );
	MagicButton->OnClicked.AddDynamic( this, &UUICharacterParent::LoadMagic );
	ResetButton->OnClicked.AddDynamic( this, &UUICharacterParent::ClearSave );
}

void UUICharacterParent::LoadMelee()
{
	// Spawns a melee character and possess it
	FVector Location = FVector( -1173, -71, 179.5 );
	FRotator Rotation = FRotator( 0.f, 0.f, 0.f );
	FActorSpawnParameters SpawnInfo;
	AMeleeCharacter* player = GetWorld()->SpawnActor<AMeleeCharacter>( BP_MeleeCharacter, Location, Rotation, SpawnInfo );

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->Possess( player );

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	RemoveFromViewport();
}

void UUICharacterParent::LoadMagic()
{
	// Spawns a magic character and possess it
	FVector Location = FVector( -1173, -71, 179.5 );
	FRotator Rotation = FRotator( 0.f, 0.f, 0.f );
	FActorSpawnParameters SpawnInfo;
	AMagicCharacter* player = GetWorld()->SpawnActor<AMagicCharacter>( BP_MagicCharacter, Location, Rotation, SpawnInfo );

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->Possess( player );

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	RemoveFromViewport();
}

void UUICharacterParent::ClearSave()
{
	if ( UGameplayStatics::DoesSaveGameExist( "Slot1", 0 ) )
	{
		USaveData* LoadGameInstance;
		LoadGameInstance = Cast<USaveData>( UGameplayStatics::LoadGameFromSlot( "Slot1", 0 ) );

		LoadGameInstance->XP_Progress = 0.f;
		LoadGameInstance->MaxXP = 1.f;
		LoadGameInstance->CurrentXP = 0.f;
		LoadGameInstance->CurrentLevel = 1;
		LoadGameInstance->TotalGold = 0;

		UGameplayStatics::SaveGameToSlot( LoadGameInstance, "Slot1", 0 );
	}
}
