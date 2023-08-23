// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "../SwashData.h"
#include "SwashGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class SWASH_API USwashGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	USwashGameplayAbility();

	// Ability will activate when input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	ESwashAbilityInputID AbilityInputID {ESwashAbilityInputID::None};
	
};
