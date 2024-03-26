
#include "SwashCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/SwashCharacterMovementComponent.h"
#include "../Components/SwashCharacterCombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"



//////////////////////////////////////////////////////////////////////////
// ASwashCharacter

ASwashCharacter::ASwashCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<USwashCharacterMovementComponent>(CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Create combat hitboxes
	StabHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("StabHitbox"));
	StabHitbox->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	StabHitbox->SetGenerateOverlapEvents(true);
	StabHitbox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	StabHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StabHitbox->OnComponentBeginOverlap.AddDynamic(this, &ASwashCharacter::OnStabOverlapBegin);

	SlashHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("SlashHitbox"));
	SlashHitbox->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SlashHitbox->SetGenerateOverlapEvents(true);
	SlashHitbox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SlashHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SlashHitbox->OnComponentBeginOverlap.AddDynamic(this, &ASwashCharacter::OnSlashOverlapBegin);

	KickHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("KickHitbox"));
	KickHitbox->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	KickHitbox->SetGenerateOverlapEvents(true);
	KickHitbox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	KickHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	KickHitbox->OnComponentBeginOverlap.AddDynamic(this, &ASwashCharacter::OnKickOverlapBegin);
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
}

void ASwashCharacter::AddKnockback(FVector impulse)
{
	if (GetCharacterMovement()->IsFalling()) //If the character is in the air, don't apply knockback
		return;

	LaunchCharacter(impulse, false, false); // Add instant knockback force with given impulse
}

//Movement actions
void ASwashCharacter::Move(const FVector2D& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value;

	if (Controller != nullptr)
	{
		//Determine if stick input triggered sprint
		//...

		//Determine if should climb (stick up + next to climbable)
		//...

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

void ASwashCharacter::StartJump()
{
	if (IsStunned)
		return;

	//IsHoldingJump = true;
}

void ASwashCharacter::EndJump()
{

	//IsHoldingJump = false;
}


//Interact Action
void ASwashCharacter::StartInteract()
{

}
void ASwashCharacter::EndInteract()
{

}

// === Attacks ===

//Slashes
void ASwashCharacter::StartSlash()
{
	if (IsAttacking)
		return;

	IsAttacking = true;

	//Enable slash hitbox
	SlashHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//Do attack based movement
	GetController()->SetIgnoreMoveInput(true);


}
void ASwashCharacter::OnSlashOverlapBegin(class UPrimitiveComponent* OtherComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool someBool, const FHitResult& hitResult)
{
	if (Cast<ASwashCharacter>(OtherActor))
	{
		//...
	}
	//if (Cast<something else>(OtherActor)){...}
}
void ASwashCharacter::EndSlash()
{
	//atm do nothing
}

//Stabs
void ASwashCharacter::StartStab()
{
	if (IsAttacking)
		return;

	IsAttacking = true;

	//Enable slash hitbox
	StabHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//Do attack based movement

	//Set up timer for end stab
	FTimerDelegate stabDelegate;
	stabDelegate.BindUFunction(this, "EndStab");
	GetWorldTimerManager().SetTimer(AttackCooldownTimer, stabDelegate, StabCooldown, false);
	
}
void ASwashCharacter::OnStabOverlapBegin(class UPrimitiveComponent* OtherComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool someBool, const FHitResult& hitResult)
{
	
	if (Cast<ASwashCharacter>(OtherActor))
	{
		ASwashCharacter* hitCharacter = Cast<ASwashCharacter>(OtherActor);
		HitPlayer(hitCharacter, ESwashAttackType::Stab);
	}
	if (Cast<ASwashDummy>(OtherActor))
	{
		ASwashDummy* hitDummy = Cast<ASwashDummy>(OtherActor);
		//HitDummy(hitDummy, ESwashAttackType::Stab);
	}
	//if (Cast<something else>(OtherActor)){...}
}
void ASwashCharacter::EndStab()
{
	//atm do nothing
}

//Specials
void ASwashCharacter::StartSpecial()
{
	//Need to do some extra stuff here for handling different items, default special is Kick tho
	//...

	//==Kick!==

	if (IsAttacking)
		return;

	IsAttacking = true;

	//Enable slash hitbox
	KickHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//Do attack based movement
	//...
}
void ASwashCharacter::OnKickOverlapBegin(class UPrimitiveComponent* OtherComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool someBool, const FHitResult& hitResult)
{
	if (Cast<ASwashCharacter>(OtherActor))
	{
		//...
	}
	//if (Cast<something else>(OtherActor)){...}
}
void ASwashCharacter::EndSpecial()
{

}


//Melee Hit Handling
void ASwashCharacter::HitPlayer(ASwashCharacter* playerRef, ESwashAttackType attackType)
{
	if (false)//playerRef->GetCombatState())
		return;

	if (true) //playerRef->GetCombatState() == ESwashCombatState::Blocking && playerRef->GetActorForwardVector().Dot(GetActorForwardVector()) < 0)
	{
		if (playerRef->CanParry)
		{
			//Get parried lelz
			StartStun(ParryStunTime);
		}
		else
		{
			//Ya got blocked chump
			playerRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * 300.0, 200.0));
		}
	}
	else
	{
		//Ayooooo you hit somebody
		switch (attackType)
		{
			case ESwashAttackType::Slash:
				playerRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * 30.0, 100.0));
				break;
			case ESwashAttackType::Stab:
				playerRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * 120.0, 120.0));
				StartStun(StabStunTime);
				break;
			case ESwashAttackType::Special: //If special attack happens here it will always be a kick
				playerRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * 300.0, 200.0));
				//Kicks apply lots of knockback but don't deal a heart of damage
				break;
		}
	
	}

	//Set player to hitstun state
	playerRef->SetHit(false);
}

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

void ASwashCharacter::SetHit(bool newHit)
{
	IsHit = newHit;

	if (IsHit)
	{
		FTimerDelegate hitCooldownDelegate;
		hitCooldownDelegate.BindUFunction(this, "SetHit", false); //Set up delegate to pass bool parameter to function after timer ends
		GetWorldTimerManager().SetTimer(HitCooldownTimer, hitCooldownDelegate, DefaultHitCooldown, false);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(HitCooldownTimer);
	}
}


// === Block Dodge Parry ===

// Blocking & Parrying Functionality
void ASwashCharacter::StartBlock() 
{
	if (IsStunned || GetCharacterMovement()->IsFalling())
		return;

	//IsBlocking = true;
	GetCharacterMovement()->StopMovementImmediately();
	GetController()->SetIgnoreMoveInput(true);
	//GetCharacterMovement()->DisableMovement();
	
	//Enable abitlity to parry
	SetCanParry(true);
}
void ASwashCharacter::EndBlock()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("End Block"));

	//IsBlocking = false;
	GetController()->ResetIgnoreMoveInput();
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

	//Disable ability to parry
	SetCanParry(false);
}
void ASwashCharacter::SetCanParry(bool newParry)
{
	CanParry = newParry;

	if (CanParry)
	{
		FTimerDelegate parryDelegate;
		parryDelegate.BindUFunction(this, "SetCanParry", false); //Set up delegate to pass bool parameter to function after timer ends
		GetWorldTimerManager().SetTimer(ParryTimer, parryDelegate, ParryTime, false);
	}
	else
	{	
		GetWorldTimerManager().ClearTimer(ParryTimer);
	}
}

// Stun Functionality
void ASwashCharacter::StartStun(float stunTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Start Stun"));
	
	IsStunned = true;
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetController()->SetIgnoreMoveInput(true);
	
	//Stun Timer
	GetWorldTimerManager().SetTimer(StunCooldownTimer, this, &ASwashCharacter::EndStun, stunTime, false);
}

void ASwashCharacter::EndStun()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("End Stun"));
	
	IsStunned = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetController()->ResetIgnoreMoveInput();

	//Stun Timer
	GetWorldTimerManager().ClearTimer(StunCooldownTimer);
}







