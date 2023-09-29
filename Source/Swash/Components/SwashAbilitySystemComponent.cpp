// Fill out your copyright notice in the Description page of Project Settings.


#include "SwashAbilitySystemComponent.h"

USwashAbilitySystemComponent::USwashAbilitySystemComponent() {}

void USwashAbilitySystemComponent::BP_GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.Reset();
	GetOwnedGameplayTags(TagContainer);
}

//void USwashAbilitySystemComponent::AddLooseGameplayTag(const FGameplayTag& GameplayTag, bool Replicated, int Count)
//{
//	if (Replicated)
//	{
//		Super::AddReplicatedLooseGameplayTag(GameplayTag);
//	}
//	else
//	{
//		Super::AddLooseGameplayTag(GameplayTag, Count);
//	}
//}
//
//void USwashAbilitySystemComponent::RemoveLooseGameplayTag(const FGameplayTag& GameplayTag, bool Replicated, int Count)
//{
//	if (Replicated)
//	{
//		Super::RemoveReplicatedLooseGameplayTag(GameplayTag);
//	}
//	else
//	{
//		Super::RemoveLooseGameplayTag(GameplayTag, Count);
//	}
//}