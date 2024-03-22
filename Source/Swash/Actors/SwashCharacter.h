//By Mitchell Dunning, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "../Components/SwashCharacterMovementComponent.h"
#include "../Actors/Weapons/MeleeWeapon.h"
#include "../Core/SwashData.h"
#include "SwashDummy.h"
#include "SwashCharacter.generated.h"


UCLASS(config=Game)
class ASwashCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	/** Default UObject constructor. */
	explicit ASwashCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:

	//Jumping properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climbing", meta = (AllowPrivateAccess = "true"))
	bool IsHanging = false;

	//Climbing properties
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

public:

	//Health & Damage props
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int health;

	//Movement Props
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsHoldingJump;

	//Block & Dodge props
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool IsBlocking = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool IsDodging = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool IsRolling = false;

	//Parry & Stun props
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool CanParry = false;
	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FTimerHandle ParryTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float ParryTime = 0.3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool IsStunned = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FTimerHandle StunCooldownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float ParryStunTime = 1.0;

	//Attack and Hit props
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FTimerHandle AttackCooldownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float SlashCooldown = 0.15;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float StabCooldown = 0.3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float StabStunTime = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float KickCooldown = 0.45;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool IsHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FTimerHandle HitCooldownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float DefaultHitCooldown = 2.0;

	//Hitboxes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> StabHitbox;

	UFUNCTION()
	void OnStabOverlapBegin(class UPrimitiveComponent* OtherComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool someBool, const FHitResult& hitResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SlashHitbox;

	UFUNCTION()
	void OnSlashOverlapBegin(class UPrimitiveComponent* OtherComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool someBool, const FHitResult& hitResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> KickHitbox;

	UFUNCTION()
	void OnKickOverlapBegin(class UPrimitiveComponent* OtherComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool someBool, const FHitResult& hitResult);


public:

	//INPUT FUNCTIONS

	/** Called for movement input */
	void Move(const FVector2D& Value);

	/** Called for jump input */
	//UFUNCTION(BlueprintCallable)
	void StartJump();
	//UFUNCTION(BlueprintCallable)
	void EndJump();

	/** Called for Slash input */
	//UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void StartSlash();
	//UFUNCTION(BlueprintCallable)
	void EndSlash();

	/** Called for Stab input */
	//UFUNCTION(BlueprintCallable)
	void StartStab();
	//UFUNCTION(BlueprintCallable)
	void EndStab();

	/** Called for Special/Use Item input */
	//UFUNCTION(BlueprintCallable)
	void StartSpecial();
	//UFUNCTION(BlueprintCallable)
	void EndSpecial();

	/** Called for block input */
	//UFUNCTION(BlueprintCallable)
	void StartBlock();
	//UFUNCTION(BlueprintCallable)
	void EndBlock();

	/** Called for interact/climb input */
	//UFUNCTION(BlueprintCallable)
	void StartInteract();
	//UFUNCTION(BlueprintCallable)
	void EndInteract();

protected:

	void BeginPlay() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Tick(float DeltaSeconds) override;

private:

	// === Ledge Hanging Functions ===

	void StartLedgeHang(FVector LedgePos);

	void EndLedgeHang();

	void LedgeCheck();

	void LedgeHangUpdate();

	UFUNCTION()
	void SetCanLedgeCheck(bool canCheck);
	

public: 

	// === Combat functions ===

	UFUNCTION(BlueprintCallable)
	void HitPlayer(ASwashCharacter* playerRef, ESwashAttackType attackType);

	//UFUNCTION(BlueprintCallable)
	//void MeleeHitDummy(ASwashDummy* dummyRef);

	UFUNCTION()
	void SetHit(bool newHit);

	UFUNCTION()
	void SetCanParry(bool newParry);

	UFUNCTION(BlueprintCallable)
	void StartStun(float stunTime);

	UFUNCTION(BlueprintCallable)
	void EndStun();

	UFUNCTION(BlueprintCallable)
	void AddKnockback(FVector impulse);

};

