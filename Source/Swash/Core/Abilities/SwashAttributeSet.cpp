// Fill out your copyright notice in the Description page of Project Settings.


#include "SwashAttributeSet.h"

#include "Net/UnrealNetwork.h"

USwashAttributeSet::USwashAttributeSet() {}

void USwashAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USwashAttributeSet, Health);
}

void USwashAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USwashAttributeSet, Health, OldValue);
}