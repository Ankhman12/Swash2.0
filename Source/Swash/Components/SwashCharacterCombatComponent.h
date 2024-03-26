//By Mitchell Dunning 2024

#pragma once

#include "CoreMinimal.h"
#include "../Core/SwashData.h"
#include "Components/ActorComponent.h"
#include "SwashCharacterCombatComponent.generated.h"

UCLASS()
class SWASH_API USwashCharacterCombatComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	
	/**
	 * Default UObject constructor.
	 */
	USwashCharacterCombatComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());




};