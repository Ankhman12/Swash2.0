// Fill out your copyright notice in the Description page of Project Settings.


#include "SwashAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "../../Actors/SwashCharacter.h"
#include "Net/UnrealNetwork.h"

USwashAttributeSet::USwashAttributeSet() {}

void USwashAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USwashAttributeSet, Health);
}

void USwashAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}

void USwashAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float DeltaValue(0.f);

	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	ASwashCharacter* TargetCharacter{ nullptr };

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetActor{ nullptr };
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<ASwashCharacter>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

		if (TargetCharacter)
		{
			TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
		}
	}
}

void USwashAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USwashAttributeSet, Health, OldValue);
}

void USwashAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USwashAttributeSet, MaxHealth, OldValue);
}

void USwashAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent(); 
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		const float CurrentValue = AffectAttribute.GetCurrentValue();
		const float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

