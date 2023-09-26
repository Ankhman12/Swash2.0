// Copyright Epic Games, Inc. All Rights Reserved.

#include "SwashCharacter.h"
//#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Components/SwashAbilitySystemComponent.h"
#include "../Core/Abilities/SwashAttributeSet.h"
#include "../Core/Abilities/SwashGameplayAbility.h"
//#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"



//////////////////////////////////////////////////////////////////////////
// ASwashCharacter

ASwashCharacter::ASwashCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	//Initialize Ability System props 
	bAbilitesInitialized = false;

	AbilitySystemComponent = CreateDefaultSubobject<USwashAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<USwashAttributeSet>(TEXT("Attributes"));

}

void ASwashCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

void ASwashCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	// Remove Reference to UI
	//if (PlayerHUD) {
	//	PlayerHUD->RemoveFromParent();
	//	PlayerHUD = nullptr;
	//}

	Super::EndPlay(EndPlayReason);
}

void ASwashCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//Lock Actor on X axis
	FVector currentLocation = GetCapsuleComponent()->GetComponentLocation();
	FVector newLocation = FVector(0.0, currentLocation.Y, currentLocation.Z);
	SetActorLocation(newLocation, false, nullptr, ETeleportType::TeleportPhysics);

	//Ledge Hang Updates
	if (IsHanging)
	{
		LedgeHangUpdate();
	}
	else if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		LedgeCheck();
	}

}

//Ability System core and setup functions
void ASwashCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Server GAS init
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGameplayAbilities();
	}
}

void ASwashCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this); //client side init
}

UAbilitySystemComponent* ASwashCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASwashCharacter::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);

	if (GetLocalRole() == ROLE_Authority && !bAbilitesInitialized)
	{
		// Grant abilities, but only on server
		for (TSubclassOf<USwashGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}

		for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
			if (NewHandle.IsValid())
			{
				//Atm, the below variable is unused, but I'm setting the return value to a variable in case its needed later.
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}

		bAbilitesInitialized = true;
	}
}

void ASwashCharacter::SendLocalInputToASC(bool bIsPressed, const ESwashAbilityInputID AbilityInputID)
{
	if (!AbilitySystemComponent)
		return;

	if (bIsPressed)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(AbilityInputID));
	}
	else
	{
		AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(AbilityInputID));
	}
}

void ASwashCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags, ASwashCharacter* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ASwashCharacter::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

float ASwashCharacter::GetHealth()
{
	if (Attributes)
	{
		return Attributes->GetHealth();
	}
	return 1.f;
}

float ASwashCharacter::GetMaxHealth()
{
	if (Attributes)
	{
		return Attributes->GetMaxHealth();
	}
	return 1.f;
}

//Basic movement functionality
void ASwashCharacter::Move(const FVector2D& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value;

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		//AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

//void ASwashCharacter::StartJump()
//{
//	//Ability system input
//	//SendLocalInputToASC(true, ESwashAbilityInputID::Jump);
//	
//	if (IsStunned)
//		return;
//
//	//Call Character's jump function, 
//	Super::Jump();
//
//	IsHoldingJump = true;
//	EndBlock(); //Send empty input action value so I know it was called from a function, not input
//}
//
//void ASwashCharacter::EndJump()
//{
//	//Ability system input
//	//SendLocalInputToASC(false, ESwashAbilityInputID::Jump);
//	
//	//Call Character's stop jumping function,
//	Super::StopJumping();
//
//	IsHoldingJump = false;
//
//}

//void ASwashCharacter::AddKnockback(FVector impulse)
//{
//	if (GetCharacterMovement()->IsFalling()) //If the character is in the air, don't apply knockback
//		return;
//
//	LaunchCharacter(impulse, false, false); // Add instant knockback force with given impulse
//}

// Ledge Climbing Functionality
void ASwashCharacter::LedgeCheck()
{
	if (!CanLedgeCheck || (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling) || IsHanging)
		return;

	FVector traceForwardOffset = GetActorLocation() + (GetActorForwardVector() * 100.0);

	FVector grabbableStartPos = traceForwardOffset + FVector(0.0, 0.0, 30.0);
	FVector grabbableEndPos = traceForwardOffset + FVector(0.0, 0.0, -45.0);

	FVector vaultableStartPos = traceForwardOffset + FVector(0.0, 0.0, -50.0);
	FVector vaultableEndPos = traceForwardOffset + FVector(0.0, 0.0, -75.0);

	float traceRadius = 10.0;

	FHitResult hit;
	FVector upVec = FVector(0.0, 0.0, 1.0);
	
	if (GetWorld()->LineTraceSingleByObjectType(hit, grabbableStartPos, grabbableEndPos, FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic))
		&& hit.Normal.Dot(upVec) >= 1.0 
		&& (GetLastMovementInputVector().X * GetActorForwardVector().Y) > 0.0)
	{
		//We Hit a grabbable ledge!
		StartLedgeHang(hit.Location);
		DrawDebugSphere(GetWorld(), hit.ImpactPoint, traceRadius * 5, 20, FColor::Green);

	}
	else if (GetWorld()->LineTraceSingleByObjectType(hit, vaultableStartPos, vaultableEndPos, FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic))
		&& hit.Normal.Dot(upVec) >= 1.0
		&& GetLastMovementInputVector().X * GetActorForwardVector().Y > 0.0)
	{
		//We Hit a vaultable ledge!
		LedgeVault(hit.Location);
		DrawDebugSphere(GetWorld(), hit.ImpactPoint, traceRadius * 5, 20, FColor::Green);
	}

	DrawDebugSphere(GetWorld(), hit.ImpactPoint, traceRadius, 20, FColor::Red);
	
}

void ASwashCharacter::StartLedgeHang(FVector LedgePos)
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement(); // Movement mode set to MOVE_None
	FVector hangPos = LedgePos + FVector(0.0, -65.0 * GetActorForwardVector().Y, -75.0);
	SetActorLocation(hangPos, false, nullptr, ETeleportType::ResetPhysics);
	IsHanging = true;

	//Start hang timer
	FTimerDelegate emptyDelegate;
	emptyDelegate.BindLambda([]() {}); //Empty function
	GetWorldTimerManager().SetTimer(HangTimer, emptyDelegate, 1.0, true); //Loop this timer (inbLoop == true) while incrementing it (rate == -X.0) 

}

void ASwashCharacter::LedgeHangUpdate()
{
	bool didMoveAwayFromLedge = (GetLastMovementInputVector().X * GetActorForwardVector().Y) < 0.0;
	if (didMoveAwayFromLedge)
	{
		EndLedgeHang();
	}

	if (IsHoldingJump && GetWorldTimerManager().GetTimerElapsed(HangTimer) > MinHangTime)
	{
		EndLedgeHang();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Should Jump Off Ledge"));
		LaunchCharacter(FVector(0.0, 0.0, LedgeJumpOffForce),false, false);
	}
}

void ASwashCharacter::EndLedgeHang()
{
	IsHanging = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	GetWorldTimerManager().ClearTimer(HangTimer);


	//Ledge Hang Cooldown
	SetCanLedgeCheck(false);
	FTimerDelegate hangCooldownDelegate;
	hangCooldownDelegate.BindUFunction(this, "SetCanLedgeCheck", true); //Set up delegate to pass bool parameter to function after timer ends
	GetWorldTimerManager().SetTimer(HangCooldownTimer, hangCooldownDelegate, DefaultHangCooldown, false);
}

void ASwashCharacter::SetCanLedgeCheck(bool canCheck)
{
	GetWorldTimerManager().ClearTimer(HangCooldownTimer);
	CanLedgeCheck = canCheck;
}

void ASwashCharacter::LedgeVault(FVector LedgePos)
{
	GetCharacterMovement()->StopMovementImmediately();
	FVector newPos = LedgePos + FVector(0.0, 0.0, 75.0);
	SetActorLocation(newPos, false, nullptr, ETeleportType::ResetPhysics);
}

bool ASwashCharacter::GetHanging()
{
	return IsHanging;
}

//Melee Hits
//void ASwashCharacter::MeleeHitPlayer(ASwashCharacter* playerRef)
//{
//	if (!CanHit)
//		return;
//
//	float blockTime = GetWorldTimerManager().GetTimerElapsed(playerRef->BlockTimer);
//	if (playerRef->IsBlocking && playerRef->GetActorForwardVector().Dot(GetActorForwardVector()) < 0)
//	{
//		if (blockTime <= ParryTimeWindow)
//		{
//			//Get parried lelz
//			StartStun();
//		}
//		else
//		{
//			//Ya got blocked chump
//			playerRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * 300.0, 200.0));
//		}
//	}
//	else
//	{
//		//Ayooooo you hit somebody
//		if (ComboNumber <= 1) // changed for combo indexes of 0,1,2 instead of 1,2,3
//		{
//			playerRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * -100.0, 200.0));
//		}
//		else
//		{
//			playerRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * 600.0, 200.0));
//		}
//	}
//
//	//Set up hit cooldown timer
//	SetCanHit(false);
//	FTimerDelegate hitCooldownDelegate;
//	hitCooldownDelegate.BindUFunction(this, "SetCanHit", true); //Set up delegate to pass bool parameter to function after timer ends
//	GetWorldTimerManager().SetTimer(HitCooldownTimer, hitCooldownDelegate, DefaultHitCooldown, false);
//}

//void ASwashCharacter::MeleeHitDummy(ASwashDummy* dummyRef)
//{
//	if (!CanHit)
//		return;
//
//	float blockTime = dummyRef->GetBlockTime();
//	if (dummyRef->GetBlocking() && dummyRef->GetActorForwardVector().Dot(GetActorForwardVector()) < 0)
//	{
//		if (blockTime <= ParryTimeWindow)
//		{
//			//Get parried lelz
//			StartStun();
//		}
//		else
//		{
//			//Ya got blocked chump
//			dummyRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * 300.0, 200.0));
//		}
//	}
//	else
//	{
//		//Ayooooo you hit somebody
//		if (ComboNumber <= 1) // changed for combo indexes of 0,1,2 instead of 1,2,3
//		{
//			dummyRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * -100.0, 200.0));
//		}
//		else
//		{
//			dummyRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * 600.0, 200.0));
//		}
//	}
//
//	//Set up hit cooldown timer
//	SetCanHit(false);
//	FTimerDelegate hitCooldownDelegate;
//	hitCooldownDelegate.BindUFunction(this, "SetCanHit", true); //Set up delegate to pass bool parameter to function after timer ends
//	GetWorldTimerManager().SetTimer(HitCooldownTimer, hitCooldownDelegate, DefaultHitCooldown, false);
//}

//void ASwashCharacter::SetCanHit(bool newCanHit)
//{
//	CanHit = newCanHit;
//}

// Blocking Functionality
//void ASwashCharacter::StartBlock() 
//{
//	if (IsStunned || GetCharacterMovement()->IsFalling())
//		return;
//
//	IsBlocking = true;
//	GetCharacterMovement()->StopMovementImmediately();
//	GetController()->SetIgnoreMoveInput(true);
//	//GetCharacterMovement()->DisableMovement();
//	
//	//Start block timer
//	FTimerDelegate emptyDelegate;
//	emptyDelegate.BindLambda([]() {}); //Empty function
//	GetWorldTimerManager().SetTimer(BlockTimer, emptyDelegate, 1.0, true); //Loop this timer (inbLoop == true) while incrementing it (rate == -X.0) 
//}
//
//void ASwashCharacter::EndBlock()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("End Block"));
//
//	IsBlocking = false;
//	GetController()->ResetIgnoreMoveInput();
//	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
//
//	//Clear block timer
//	GetWorldTimerManager().ClearTimer(BlockTimer);
//}

//bool ASwashCharacter::GetBlocking()
//{
//	return IsBlocking;
//}

// Stun Functionality
//void ASwashCharacter::StartStun()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Start Stun"));
//	
//	IsStunned = true;
//	GetCharacterMovement()->StopMovementImmediately();
//	GetCharacterMovement()->DisableMovement();
//	GetController()->SetIgnoreMoveInput(true);
//	
//	//Stun Timer
//	GetWorldTimerManager().SetTimer(StunTimer, this, &ASwashCharacter::EndStun, DefaultStunTime, false);
//}

//void ASwashCharacter::EndStun()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("End Stun"));
//	
//	IsStunned = false;
//	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
//	GetController()->ResetIgnoreMoveInput();
//
//	//Stun Timer
//	GetWorldTimerManager().ClearTimer(StunTimer);
//}

//bool ASwashCharacter::GetStunned()
//{
//	return IsStunned;
//}








