// Fill out your copyright notice in the Description page of Project Settings.


#include "SwashMeleeAnimNotifyState.h"
#include "../Actors/SwashCharacter.h"

USwashMeleeAnimNotifyState::USwashMeleeAnimNotifyState() {}

void USwashMeleeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ASwashCharacter* playerRef = Cast<ASwashCharacter>(MeshComp->GetAnimInstance()->TryGetPawnOwner());
	if (playerRef)
	{
		//playerRef->MeleeStep();
	}
}