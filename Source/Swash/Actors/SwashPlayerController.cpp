// Fill out your copyright notice in the Description page of Project Settings.


#include "SwashPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


ASwashPlayerController::ASwashPlayerController() : APlayerController()
{

}

void ASwashPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//retrieve initially possessed pawn
	possessedPawn = Cast<ASwashCharacter>(GetPawn());

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ASwashPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//retrieve newly possessed pawn
	possessedPawn = Cast<ASwashCharacter>(GetPawn());
}

void ASwashPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	//...
}

void ASwashPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//if (!possessedPawn)
	//	return;

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASwashPlayerController::CallStartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASwashPlayerController::CallEndJump);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASwashPlayerController::CallMove);

		//Melee Attacking
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &ASwashPlayerController::CallStartMelee);
		//EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Completed, this, &ASwashPlayerController::CallEndMelee);

		//Ranged Attacking
		EnhancedInputComponent->BindAction(RangedAction, ETriggerEvent::Started, this, &ASwashPlayerController::CallStartRanged);
		EnhancedInputComponent->BindAction(RangedAction, ETriggerEvent::Completed, this, &ASwashPlayerController::CallEndRanged);

		//Special Ability
		EnhancedInputComponent->BindAction(SpecialAction, ETriggerEvent::Started, this, &ASwashPlayerController::CallStartSpecial);
		//EnhancedInputComponent->BindAction(SpecialAction, ETriggerEvent::Completed, this, &ASwashPlayerController::CallEndSpecial);

		//Blocking
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &ASwashPlayerController::CallStartBlock);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ASwashPlayerController::CallEndBlock);

		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASwashPlayerController::CallStartInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ASwashPlayerController::CallEndInteract);

		//Menu (Spawning for now)
		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this, &ASwashPlayerController::CallSpawn);
	}	
}

void ASwashPlayerController::CallMove(const FInputActionValue& Value)
{
	if (possessedPawn)
		possessedPawn->Move(Value.Get<FVector2D>());
}

void ASwashPlayerController::CallStartJump(const FInputActionValue& Value)
{
	if (possessedPawn)
		possessedPawn->StartJump();
}

void ASwashPlayerController::CallEndJump(const FInputActionValue& Value)
{
	if (possessedPawn)
		possessedPawn->EndJump();
}

void ASwashPlayerController::CallStartMelee(const FInputActionValue& Value) 
{
	if (possessedPawn)
		possessedPawn->StartSlash();
}

//void ASwashPlayerController::CallEndMelee(const FInputActionValue& Value)
//{
//	if (possessedPawn)
//		possessedPawn->EndSlash();
//}

void ASwashPlayerController::CallStartRanged(const FInputActionValue& Value)
{
	if (possessedPawn)
		possessedPawn->StartSpecial();
}

void ASwashPlayerController::CallEndRanged(const FInputActionValue& Value)
{
	if (possessedPawn)
		possessedPawn->EndSpecial();
}

void ASwashPlayerController::CallStartSpecial(const FInputActionValue& Value)
{
	if (possessedPawn)
		possessedPawn->StartStab();
}

//void ASwashPlayerController::CallEndSpecial(const FInputActionValue& Value)
//{
//	if (possessedPawn)
//		possessedPawn->EndStab();
//}

void ASwashPlayerController::CallStartBlock(const FInputActionValue& Value)
{
	if (possessedPawn)
		possessedPawn->StartBlock();
}

void ASwashPlayerController::CallEndBlock(const FInputActionValue& Value)
{
	if (possessedPawn)
		possessedPawn->EndBlock();
}

void ASwashPlayerController::CallStartInteract(const FInputActionValue& Value)
{
	if (possessedPawn)
		possessedPawn->StartInteract();
}

void ASwashPlayerController::CallEndInteract(const FInputActionValue& Value)
{
	if (possessedPawn)
		possessedPawn->EndInteract();
}

