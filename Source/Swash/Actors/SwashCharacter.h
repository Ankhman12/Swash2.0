//By Mitchell Dunning, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Components/BoxComponent.h"
#include "SwashDummy.h"
#include "SwashCharacter.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMontagePlayDelegate, FName, NotifyName);
class USwashGameplayAbility;
class USwashAbilitySystemComponent;
class USwashAttributeSet;


UCLASS(config=Game)
class ASwashCharacter : public ACharacter, public IAbilitySystemInterface
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

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USwashAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USwashAttributeSet> Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> SwordHitbox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SwordMesh;


	//Combat properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FName SwordHandSocketName = "RightHandSocket";

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


	//Climbing properties
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

	//Ability system properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<USwashGameplayAbility>> GameplayAbilities;

	UPROPERTY()
	uint8 bAbilitesInitialized:1;

	//Other props
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsHoldingJump;

public:
	
	//Constructor
	ASwashCharacter();

	friend USwashAttributeSet;

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

	void PossessedBy(AController* NewController) override;

	void OnRep_PlayerState() override;

	//void Jump() override;

	//void Pause();

	// APawn interface
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Gameplay Ability Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End of Ability System Interface

	
	// Two below events are from RPG example project, that showcases the Gameplay Ability System (I'm stealing them)

	/**
	 * Called when character takes damage, which may have killed them
	 *
	 * @param DamageAmount Amount of damage that was done, not clamped vased on current health
	 * @param HitInfo The hit info that generated this damage
	 * @param DamageTags The gameplay tags of the event that did the damage
	 * @param InstigatorCharacter The character the initiated this damage
	 * @param DamageCauser The actual actor that did the damage, might be a weapon or projectile
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASwashCharacter* InstigatorCharacter, AActor* DamageCauser);

	/**
	 * Called when health is changed, either from healing or being damaged
	 * For famage this is called in addition to OnDamaged/OnKilled
	 *
	 * @param DeltaValue Change in health value, positive for heal, negative for cost. If 0 the delta is unknown
	 * @param EventTags The gameplay tags of the event that changed mana
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASwashCharacter* InstigatorCharacter, AActor* DamageCauser);

	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	

private:

	
	// === Ability System Functions ===

	void AddStartupGameplayAbilities();

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

