//By Mitchell Dunning 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"

UCLASS()
class SWASH_API USwashCharacterMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
	
	/**
	 * Default UObject constructor.
	 */
	USwashCharacterMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	/** Character movement component belongs to */
	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<ACharacter> CharacterOwner;

private:

	/**
	 * Max angle in degrees of a walkable surface. Any greater than this and it is too steep to be walkable.
	 */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "90.0", UIMin = "0.0", UIMax = "90.0", ForceUnits = "degrees"))
	float WalkableFloorAngle;

	/**
	 * Minimum Z value for floor normal. If less, not a walkable surface. Computed from WalkableFloorAngle.
	 */
	UPROPERTY(Category = "Character Movement: Walking", VisibleAnywhere)
	float WalkableFloorZ;

public:
	
	/** Custom gravity scale. Gravity is multiplied by this amount for the character. */
	UPROPERTY(Category = "Swash Movement (General Settings)", EditAnywhere, BlueprintReadWrite)
	float GravityScale;

	/** Initial velocity (instantaneous vertical acceleration) when jumping. */
	UPROPERTY(Category = "Swash Movement: Jumping / Falling", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Jump Z Velocity", ClampMin = "0", UIMin = "0", ForceUnits = "cm/s"))
	float JumpZVelocity;

	/** Fraction of JumpZVelocity to use when automatically "jumping off" of a base actor that's not allowed to be a base for a character. (For example, if you're not allowed to stand on other players.) */
	UPROPERTY(Category = "Swash Movement: Jumping / Falling", EditAnywhere, BlueprintReadWrite, AdvancedDisplay, meta = (ClampMin = "0", UIMin = "0"))
	float JumpOffJumpZFactor;

	/**
	 * Character's current movement mode (walking, falling, etc).
	 *    - walking
	 *    - sprinting
	 *	  - falling
	 *    - flying
	 *    - climbing
	 *    - none
	 */
	UPROPERTY(Category = "Swash Movement: MovementMode", BlueprintReadOnly)
	TEnumAsByte<enum ESwashMovementMode> MovementMode;

	/** The maximum ground speed when walking. Also determines maximum lateral speed when falling. */
	UPROPERTY(Category = "Swash Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = "cm/s"))
	float WalkSpeed;

	/** The maximum ground speed when walking. Also determines maximum lateral speed when falling. */
	UPROPERTY(Category = "Swash Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = "cm/s"))
	float SprintSpeed;

	/**
	 * When falling, amount of lateral movement control available to the character.
	 * 0 = no control, 1 = full control at WalkSpeed.
	 */
	UPROPERTY(Category = "Swash Movement: Jumping / Falling", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float AirControl;

	/** Collision half-height when crouching (component scale is applied separately) */
	UPROPERTY(Category = "Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetCrouchedHalfHeight, BlueprintGetter = GetCrouchedHalfHeight, meta = (ClampMin = "0", UIMin = "0", ForceUnits = cm))
	float CrouchedHalfHeight;

protected:

	/**
	 * True during movement update.
	 * Used internally so that attempts to change CharacterOwner and UpdatedComponent are deferred until after an update.
	 * @see IsMovementInProgress()
	 */
	UPROPERTY()
	uint8 bMovementInProgress : 1;

public:



};