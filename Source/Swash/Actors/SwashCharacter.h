//By Mitchell Dunning, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/BoxComponent.h"
#include "SwashDummy.h"
#include "SwashCharacter.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMontagePlayDelegate, FName, NotifyName);

UCLASS(config=Game)
class ASwashCharacter : public ACharacter
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Melee Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MeleeAction;

	/** Ranged Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RangedAction;
	
	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Block Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BlockAction;

	/** Special Ability Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpecialAction;

	/** Look Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> SwordHitbox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SwordMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FName SwordHandSocketName = "RightHandSocket";

	//UPROPERTY(BlueprintAssignable)
	//FOnMontagePlayDelegate OnBlendOut;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool IsBlocking = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool IsStunned = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FTimerHandle BlockTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FTimerHandle StunTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FTimerHandle HitCooldownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool CanHit = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MeleeAttackAnim;

	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int AttackIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int ComboNumber = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float DefaultStunTime = 2.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float DefaultHitCooldown = 0.15;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ParryTimeWindow = 0.2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	bool IsHanging = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	bool CanLedgeCheck = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	FTimerHandle HangCooldownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	FTimerHandle HangTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	float LedgeJumpOffForce = 1800.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	float DefaultHangCooldown = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	float MinHangTime = 0.15;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsHoldingJump;

public:
	
	//Constructor
	ASwashCharacter();

protected:

	//INPUT FUNCTIONS

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for jump input */
	void StartJump(const FInputActionValue& Value);
	void EndJump(const FInputActionValue& Value);

	/** Called for melee attack input */
	void StartMelee(const FInputActionValue& Value);
	UFUNCTION()
	void EndMelee(UAnimMontage* Montage, bool bInterrupted); //Parameters are for setting up end of montage delegate

	/** Called for ranged attack input */
	void StartRanged(const FInputActionValue& Value);
	void EndRanged(const FInputActionValue& Value);

	/** Called for special ability input */
	void StartSpecial(const FInputActionValue& Value);
	void EndSpecial(const FInputActionValue& Value);

	/** Called for block input */
	void StartBlock(const FInputActionValue& Value);
	void EndBlock(const FInputActionValue& Value);

	/** Called for interact input */
	void Interact(const FInputActionValue& Value);

protected:

	void BeginPlay() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Tick(float DeltaSeconds) override;

	//void Jump() override;

	//void Pause();

	// APawn interface
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

private:

	// === Ledge Hanging Functions ===

	void StartLedgeHang(FVector LedgePos);

	void EndLedgeHang();

	void LedgeCheck();

	void LedgeHangUpdate();

	UFUNCTION()
	void SetCanLedgeCheck(bool canCheck);

	void LedgeVault(FVector LedgePos);

	// === Combat functions ===

	UFUNCTION()
	void SetCanHit(bool newCanHit);

	UFUNCTION()
	void OnSwordHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public: 

	void MeleeStep(); //Called at the end of each individual strike within a combo attack animation

	void MeleeHitPlayer(ASwashCharacter* playerRef);

	void MeleeHitDummy(ASwashDummy* dummyRef);

	UFUNCTION(BlueprintCallable)
	void StartStun();

	UFUNCTION(BlueprintCallable)
	void EndStun();

	bool GetStunned();

	bool GetBlocking();

	bool GetHanging();

	UFUNCTION(BlueprintCallable)
	void AddKnockback(FVector impulse);

};

