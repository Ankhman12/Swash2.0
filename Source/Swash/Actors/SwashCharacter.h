//By Mitchell Dunning, 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Components/BoxComponent.h"
//#include "../Actors/Weapons/Weapon.h"
#include "../Actors/Weapons/MeleeWeapon.h"
//#include "../Actors/Weapons/RangedWeapon.h"
#include "../Core/SwashData.h"
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

private:

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USwashAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USwashAttributeSet> Attributes;

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

public:

	//Other props
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHoldingJump;

public:
	
	//Constructor
	ASwashCharacter();

	friend USwashAttributeSet;

	//INPUT FUNCTIONS

	/** Called for movement input */
	void Move(const FVector2D& Value);

	/** Called for jump input */
	UFUNCTION(BlueprintImplementableEvent)
	void StartJump();
	UFUNCTION(BlueprintImplementableEvent)
	void EndJump();

	/** Called for melee attack input */
	UFUNCTION(BlueprintImplementableEvent)
	void StartMelee();
	UFUNCTION(BlueprintImplementableEvent)
	void EndMelee();

	/** Called for ranged attack input */
	UFUNCTION(BlueprintImplementableEvent)
	void StartRanged();
	UFUNCTION(BlueprintImplementableEvent)
	void EndRanged();

	/** Called for special ability input */
	UFUNCTION(BlueprintImplementableEvent)
	void StartSpecial();
	UFUNCTION(BlueprintImplementableEvent)
	void EndSpecial();

	/** Called for block input */
	UFUNCTION(BlueprintImplementableEvent)
	void StartBlock();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndBlock();

	/** Called for interact input */
	UFUNCTION(BlueprintImplementableEvent)
	void Interact();

protected:

	void BeginPlay() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Tick(float DeltaSeconds) override;

	void PossessedBy(AController* NewController) override;

	void OnRep_PlayerState() override;

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

	//The following are custom events or functions, still made for ability system
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnDeath();

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASwashCharacter* InstigatorCharacter, AActor* DamageCauser);

	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth();

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth();

	//Weapon hit delegate event
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnSwordHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	
	// === Ability System Functions ===

	void AddStartupGameplayAbilities();

	void SendLocalInputToASC(bool bIsPressed, const ESwashAbilityInputID AbilityInputID);

	// === Ledge Hanging Functions ===

	void StartLedgeHang(FVector LedgePos);

	void EndLedgeHang();

	void LedgeCheck();

	void LedgeHangUpdate();

	UFUNCTION()
	void SetCanLedgeCheck(bool canCheck);

	void LedgeVault(FVector LedgePos);
	

public: 

	// === Combat functions ===

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MeleeHitPlayer(ASwashCharacter* playerRef);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MeleeHitDummy(ASwashDummy* dummyRef);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartStun();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndStun();

	bool GetHanging();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AddKnockback(FVector impulse);

};

