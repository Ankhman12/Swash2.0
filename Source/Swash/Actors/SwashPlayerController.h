// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "SwashCharacter.h"
#include "SwashPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SWASH_API ASwashPlayerController : public APlayerController
{
	GENERATED_BODY()

public: 

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Melee Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MeleeAction;

	/** Ranged Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RangedAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Block Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BlockAction;

	/** Special Ability Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpecialAction;

	/** Menu/Spawn Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MenuAction;


	ASwashCharacter* possessedPawn;


	ASwashPlayerController();

	void SetupInputComponent() override;

	void BeginPlay() override;

	void PlayerTick(float DeltaTime) override;

	void OnPossess(APawn* InPawn) override;


	/** Spawn input */
	UFUNCTION(BlueprintImplementableEvent)
	void CallSpawn(const FInputActionValue& Value);

	/** Movement input */
	void CallMove(const FInputActionValue& Value);

	/** Jump input */
	void CallStartJump(const FInputActionValue& Value);
	void CallEndJump(const FInputActionValue& Value);

	/** Melee attack input */
	void CallStartMelee(const FInputActionValue& Value);
	//void CallEndMelee(const FInputActionValue& Value);

	/** Ranged attack input */
	void CallStartRanged(const FInputActionValue& Value);
	void CallEndRanged(const FInputActionValue& Value);

	/** Special ability input */
	void CallStartSpecial(const FInputActionValue& Value);
	//void CallEndSpecial(const FInputActionValue& Value);

	/** Block input */
	void CallStartBlock(const FInputActionValue& Value);
	void CallEndBlock(const FInputActionValue& Value);

	/** Interact input */
	void CallStartInteract(const FInputActionValue& Value);
	void CallEndInteract(const FInputActionValue& Value);


};
