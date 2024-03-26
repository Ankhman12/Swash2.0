//Data File for declaring global enums and such

#pragma once

UENUM(BlueprintType)
enum class ESwashAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Jump,
	MeleeAttack,
	RangedAttack,
	Interact,
	Block,
	Special
};

UENUM(BlueprintType)
enum ESwashAttackType
{
	Slash,
	Stab,
	Special
};

UENUM(BlueprintType)
enum ESwashCombatState
{
	Idle,
	Attacking,
	Blocking,
	Aiming,
	Dodging,
	Hit
};
