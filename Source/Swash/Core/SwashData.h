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
enum class ESwashAttackType : uint8
{
	Slash,
	Stab,
	Special
};

UENUM(BlueprintType)
enum class ESwashMovementMode : uint8
{
	None,
	Walking,
	Sprinting,
	Falling,
	Climbing
};

UENUM(BlueprintType)
enum class ESwashCombatState : uint8
{
	Attacking,
	Blocking,
	Aiming,
	Dodging,
	Hit,

};
