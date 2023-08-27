// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SwashMeleeAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SWASH_API USwashMeleeAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	USwashMeleeAnimNotifyState();

protected:

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

};
