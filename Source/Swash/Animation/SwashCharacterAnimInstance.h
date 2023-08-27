// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SwashCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SWASH_API USwashCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	USwashCharacterAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties");
	class ASwashCharacter* PlayerReference;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties");
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties");
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties");
	bool ShouldMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties");
	bool IsFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties");
	bool IsHanging;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties");
	bool IsJumping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties");
	bool IsBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties");
	bool IsCarrying;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties");
	bool IsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties");
	bool IsStunned;

	UFUNCTION(BlueprintCallable, Category = "Animations")
	virtual void UpdateAnimProperties();

};
