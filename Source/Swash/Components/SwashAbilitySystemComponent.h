// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SwashAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class SWASH_API USwashAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	USwashAbilitySystemComponent();
	
	UFUNCTION(BlueprintCallable)
	void BP_GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;

	/*UFUNCTION(BlueprintCallable)
	void AddLooseGameplayTag(const FGameplayTag& GameplayTag, bool Replicated, int Count=1);

	UFUNCTION(BlueprintCallable)
	void RemoveLooseGameplayTag(const FGameplayTag& GameplayTag, bool Replicated, int Count=1);*/
};
