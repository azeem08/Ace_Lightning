// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Ace_LightningCharacter.h"
#include "Ace_LightningGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"

//////////////////////////////////////////////////////////////////////////
// AAce_LightningCharacter

AAce_LightningCharacter::AAce_LightningCharacter()
	: CurrentHealth			( 0.5f )
	, HealthRegenRate		( 1.f )
	, CurrentSpecial		( 0.5f )
	, FXHealthPickUp		( nullptr )
	, FXSpecialPickUp		( nullptr )
	, FXLootPickUp			( nullptr )
	, GameMode				( nullptr )
	, bHasFinishedAnimating	( true )
	, MaxHealth				( 1.f )
	, MaxSpecial			( 1.f )
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FXHealthPickUp = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "VFX Health pick up" ) );
	FXHealthPickUp->SetupAttachment( RootComponent, "RightFoot" );
	FXHealthPickUp->bAutoActivate = false;

	FXSpecialPickUp = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "VFX Special pick up" ) );
	FXSpecialPickUp->SetupAttachment( RootComponent, "RightFoot" );
	FXSpecialPickUp->bAutoActivate = false;

	FXLootPickUp = CreateDefaultSubobject<UParticleSystemComponent>( TEXT( "VFX Loot pick up" ) );
	FXLootPickUp->SetupAttachment( RootComponent, "RightFoot" );
	FXLootPickUp->bAutoActivate = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bAbilityHasCooledDown.SetNum( 10 );

	for ( auto ability : bAbilityHasCooledDown )
	{
		ability = true;
	}

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

bool AAce_LightningCharacter::HasCooledDown( int ability )
{
	bool bIsAvailable = false;

	switch ( ability )
	{
		case 0:
			bIsAvailable = bAbilityHasCooledDown[0];
			break;
		case 1:
			bIsAvailable = bAbilityHasCooledDown[1];
			break;
		case 2:
			bIsAvailable = bAbilityHasCooledDown[2];
			break;
		case 3:
			bIsAvailable = bAbilityHasCooledDown[3];
			break;
		case 4:
			bIsAvailable = bAbilityHasCooledDown[4];
			break;
		case 5:
			bIsAvailable = bAbilityHasCooledDown[5];
			break;
		case 6:
			bIsAvailable = bAbilityHasCooledDown[6];
			break;
		case 7:
			bIsAvailable = bAbilityHasCooledDown[7];
			break;
		case 8:
			bIsAvailable = bAbilityHasCooledDown[8];
			break;
		case 9:
			bIsAvailable = bAbilityHasCooledDown[9];
			break;
		default:
			bIsAvailable = bAbilityHasCooledDown[0];
			break;
	}

	return bIsAvailable;
}

bool AAce_LightningCharacter::IsAnimating()
{
	return bHasFinishedAnimating;
}

void AAce_LightningCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AAce_LightningGameMode>( GetWorld()->GetAuthGameMode() );

	if ( !GameMode )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No game mode pointer" ) );
	}
	else
	{
		GameMode->PickupEvent.AddDynamic( this, &AAce_LightningCharacter::PickUpCollected );
		GameMode->AvailablityEvent.AddDynamic( this, &AAce_LightningCharacter::AbilityAvailable );
		GameMode->LootBagEvent.AddDynamic( this, &AAce_LightningCharacter::LootBagCollected );

		for ( int i = 0; i < 10; ++i )
		{
			GameMode->AbilityAvailable( i );
		}
	}
	
	if ( !FXHealthPickUp )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No heal pick up VFX added" ) );
	}

	if ( !FXSpecialPickUp )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No mana/stamina pick up VFX added" ) );
	}

	if ( !FXLootPickUp )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No loot pick up VFX added" ) );
	}

	MovementComponent = Cast<UCharacterMovementComponent>( GetComponentByClass( UCharacterMovementComponent::StaticClass() ) );

	if( !MovementComponent )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No movement component found" ) );
	}
}

void AAce_LightningCharacter::EndPlay( EEndPlayReason::Type EndPlayReason )
{
	Super::EndPlay( EndPlayReason );

	if ( EndPlayReason == EEndPlayReason::Quit )
	{
		GameMode->PickupEvent.RemoveDynamic( this, &AAce_LightningCharacter::PickUpCollected );
		GameMode->AvailablityEvent.RemoveDynamic( this, &AAce_LightningCharacter::AbilityAvailable );
		GameMode->LootBagEvent.RemoveDynamic( this, &AAce_LightningCharacter::LootBagCollected );
	}
}

void AAce_LightningCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if ( CurrentHealth < MaxHealth )
	{
		CurrentHealth += ( DeltaTime * HealthRegenRate );

		if ( CurrentHealth > MaxHealth )
		{
			CurrentHealth = MaxHealth;
		}
	}

	if ( CurrentSpecial < MaxSpecial )
	{
		CurrentSpecial += ( DeltaTime * SpecialRegenRate );

		if ( CurrentSpecial > MaxSpecial )
		{
			CurrentSpecial = MaxSpecial;
		}
	}

	if ( MovementComponent->IsFalling() )
	{
		if ( !bHasSentMessage )
		{
			GameMode->ActivateAbility( EAbilities::Jumping );
			bHasSentMessage = true;
		}
	}
	else
	{
		if ( bHasSentMessage )
		{
			GameMode->StopCasting( EAbilities::Jumping );
			bHasSentMessage = false;
		}
	}
}

void AAce_LightningCharacter::Ability1()
{
}

void AAce_LightningCharacter::Ability2()
{
}

void AAce_LightningCharacter::SaveGame()
{
	GameMode->SaveGame();
}

void AAce_LightningCharacter::ShowInventory()
{
	GameMode->ShowInventory();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAce_LightningCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction( "Save", IE_Pressed, this, &AAce_LightningCharacter::SaveGame );
	PlayerInputComponent->BindAction( "Inventory", IE_Pressed, this, &AAce_LightningCharacter::ShowInventory );
	PlayerInputComponent->BindAction( "Ability1", IE_Pressed, this, &AAce_LightningCharacter::Ability1 );
	PlayerInputComponent->BindAction( "Ability2", IE_Pressed, this, &AAce_LightningCharacter::Ability2 );

	PlayerInputComponent->BindAxis("MoveForward", this, &AAce_LightningCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAce_LightningCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAce_LightningCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAce_LightningCharacter::LookUpAtRate);
}

void AAce_LightningCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAce_LightningCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAce_LightningCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAce_LightningCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AAce_LightningCharacter::PickUpCollected( EStats stats, float value )
{
	if ( stats == EStats::Health )
	{
		if ( CurrentHealth < MaxHealth )
		{
			CurrentHealth += value;
			FXHealthPickUp->Activate();

			if ( CurrentHealth > MaxHealth )
			{
				CurrentHealth = MaxHealth;
			}
		}
	}
	else
	{
		if ( CurrentSpecial < MaxSpecial )
		{
			CurrentSpecial += value;
			FXSpecialPickUp->Activate();

			if ( CurrentSpecial > MaxSpecial )
			{
				CurrentSpecial = MaxSpecial;
			}
		}
	}
}

void AAce_LightningCharacter::AbilityAvailable( int ability )
{
	switch ( ability )
	{
		case 1:
			bAbilityHasCooledDown[0] = true;
			break;
		case 2:
			bAbilityHasCooledDown[1] = true;
			break;
		case 3:
			bAbilityHasCooledDown[2] = true;
			break;
		case 4:
			bAbilityHasCooledDown[3] = true;
			break;
		case 5:
			bAbilityHasCooledDown[4] = true;
			break;
		case 6:
			bAbilityHasCooledDown[5] = true;
			break;
		case 7:
			bAbilityHasCooledDown[6] = true;
			break;
		case 8:
			bAbilityHasCooledDown[7] = true;
			break;
		case 9:
			bAbilityHasCooledDown[8] = true;
			break;
		case 10:
			bAbilityHasCooledDown[9] = true;
			break;
		default:
			bAbilityHasCooledDown[0] = true;
			break;
	}
}

void AAce_LightningCharacter::LootBagCollected( int value )
{
	FXLootPickUp->Activate();
}
