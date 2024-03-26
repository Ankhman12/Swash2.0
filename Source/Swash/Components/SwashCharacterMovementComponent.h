//By Mitchell Dunning 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "SwashCharacterMovementComponent.generated.h"

class ASwashCharacter;

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None UMETA(Hidden),
	CMOVE_Climbing UMETA(DisplayName = "Climbing"),
	CMOVE_OnWall UMETA(DisplayName = "On Wall"),
	CMOVE_Hanging UMETA(DisplayName = "Hanging"),
	CMOVE_MAX UMETA(Hidden)
};

UCLASS()
class SWASH_API USwashCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * Default UObject constructor.
	 */
	USwashCharacterMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void InitializeComponent() override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void TickComponent(float DeltaSeconds, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:

	UPROPERTY(Transient)
	TObjectPtr<ASwashCharacter> SwashCharacterOwner;

	//Helper Methods
	bool IsCustomMovementMode(const ECustomMovementMode InCustomMovementMode) const;
	bool CanPerfromPhysMovement() const;
	bool CanPerformFrameTickMovement(const float RemainingTime, const int32 Iterations) const;

	//Jumping properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsJumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsHoldingJump;

	//Ledge Hang properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	bool IsHanging = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	bool CanLedgeCheck = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	FTimerHandle HangCooldownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	float LedgeJumpOffForce = 1800.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	float DefaultHangCooldown = 0.5;

	// === Ledge Hanging Functions ===

	void StartLedgeHang(FVector LedgePos);

	void EndLedgeHang();

	void LedgeCheck();

	void LedgeHangUpdate();

	UFUNCTION()
	void SetCanLedgeCheck(bool canCheck);
	
public:


};