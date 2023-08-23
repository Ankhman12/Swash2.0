// Copyright Epic Games, Inc. All Rights Reserved.

#include "SwashCharacter.h"
//#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetSystemLibrary.h"
//#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"



//////////////////////////////////////////////////////////////////////////
// ASwashCharacter

ASwashCharacter::ASwashCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	//Setup sword components
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SwordHandSocketName); //Attach to character skeletal mesh at the specified sword hand socket
	SwordHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordHitbox"));
	SwordHitbox->AttachToComponent(SwordMesh, FAttachmentTransformRules::KeepRelativeTransform);
	SwordHitbox->SetGenerateOverlapEvents(true);
	SwordHitbox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SwordHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

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
}

void ASwashCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//Setup Collision Delegate
	SwordHitbox->OnComponentBeginOverlap.AddDynamic(this, &ASwashCharacter::OnSwordHit);
}

void ASwashCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	// Remove Reference to UI
	//if (PlayerHUD) {
	//	PlayerHUD->RemoveFromParent();
	//	PlayerHUD = nullptr;
	//}

	Super::EndPlay(EndPlayReason);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASwashCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASwashCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASwashCharacter::EndJump);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASwashCharacter::Move);

		//Melee Attacking
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &ASwashCharacter::StartMelee);

		//Ranged Attacking
		EnhancedInputComponent->BindAction(RangedAction, ETriggerEvent::Triggered, this, &ASwashCharacter::StartRanged);
		EnhancedInputComponent->BindAction(RangedAction, ETriggerEvent::Completed, this, &ASwashCharacter::EndRanged);

		//Special Ability
		EnhancedInputComponent->BindAction(SpecialAction, ETriggerEvent::Triggered, this, &ASwashCharacter::StartSpecial);
		EnhancedInputComponent->BindAction(SpecialAction, ETriggerEvent::Completed, this, &ASwashCharacter::EndSpecial);

		//Blocking
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &ASwashCharacter::StartBlock);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ASwashCharacter::EndBlock);

		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASwashCharacter::Interact);

	}

}

void ASwashCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//Lock Actor on X axis
	FVector currentLocation = GetCapsuleComponent()->GetComponentLocation();
	FVector newLocation = FVector(0.0, currentLocation.Y, currentLocation.Z);
	SetActorLocation(newLocation, false, nullptr, ETeleportType::TeleportPhysics);

	if (IsHanging)
	{
		LedgeHangUpdate();
	}
	else if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		LedgeCheck();
	}

	if (IsStunned)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Move mode: %d"), static_cast<int>(GetCharacterMovement()->MovementMode)));

}

void ASwashCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

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

void ASwashCharacter::StartJump(const FInputActionValue& Value)
{
	if (IsStunned)
		return;
	
	//Call Character's jump function, 
	Super::Jump();

	IsHoldingJump = true;
	EndBlock(FInputActionValue()); //Send empty input action value so I know it was called from a function, not input
}

void ASwashCharacter::EndJump(const FInputActionValue& Value)
{
	//Call Character's stop jumping function,
	Super::StopJumping();

	IsHoldingJump = false;

}

void ASwashCharacter::AddKnockback(FVector impulse)
{
	if (GetCharacterMovement()->IsFalling()) //If the character is in the air, don't apply knockback
		return;

	LaunchCharacter(impulse, false, false); // Add instant knockback force with given impulse
}

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

// Melee Attack Functionality
void ASwashCharacter::StartMelee(const FInputActionValue& Value) 
{
	if (IsHanging || GetCharacterMovement()->IsFalling() || IsStunned)
		return;

	EndBlock(FInputActionValue()); //Send empty input action value so I know it was called from a function, not input

	SwordHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCharacterMovement()->DisableMovement();
	GetController()->SetIgnoreMoveInput(true);

	//Play animation montage
	if (!MeleeAttackAnim)
	{
		//Montage is NULL -> ERROR!
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Melee Attack Montage is NULL!"));
		return;
	}

	if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(MeleeAttackAnim))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MeleeAttackAnim, 1.0, EMontagePlayReturnType::MontageLength, 0, true);
		
		

		FOnMontageBlendingOutStarted BlendingOutDelegate;
		//Set up delegate for when montage ends
		BlendingOutDelegate.BindUObject(this, &ASwashCharacter::EndMelee);
		GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MeleeAttackAnim);

	}
	else
	{
		AttackIndex = 1;
	}

}

void ASwashCharacter::MeleeStep()
{
	AttackIndex--;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Combo Num: %d"), ComboNumber)); //print current combo number to screen
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Idx: %d"), AttackIndex)); //print current combo number to screen
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	AddKnockback(FVector(0.0, GetActorForwardVector().Y * 350.0, 200.0));
	if (AttackIndex < 0 || IsStunned || IsBlocking)
	{
		//Stop Attacking
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2, MeleeAttackAnim); //Blend out of montage
		EndMelee(MeleeAttackAnim, true);
	}
	else
	{
		ComboNumber++;
	}
}

void ASwashCharacter::EndMelee(UAnimMontage* Montage, bool bInterrupted)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Test"));

	AttackIndex = 0;
	ComboNumber = 1;
	SwordHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (!IsStunned)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetController()->SetIgnoreMoveInput(false);
	}
}

void ASwashCharacter::OnSwordHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (OtherActor == this)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Hit!"));

	ASwashCharacter* otherPlayer = Cast<ASwashCharacter>(OtherActor);
	if (otherPlayer)
	{
		MeleeHitPlayer(otherPlayer);
	}

	//Dummy hit stuff
	ASwashDummy* otherDummy = Cast<ASwashDummy>(OtherActor);
	if (otherDummy)
	{
		MeleeHitDummy(otherDummy);
	}

}

void ASwashCharacter::MeleeHitPlayer(ASwashCharacter* playerRef)
{
	if (!CanHit)
		return;

	float blockTime = GetWorldTimerManager().GetTimerElapsed(playerRef->BlockTimer);
	if (blockTime <= ParryTimeWindow)
	{
		//Get parried lelz
		StartStun();
	}
	else if (playerRef->IsBlocking)
	{
		//Ya got blocked chump
		playerRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * 300.0, 200.0));
	}
	else
	{
		//Ayooooo you hit somebody
		playerRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * 600.0, 200.0));
	}

	//Set up hit cooldown timer
	SetCanHit(false);
	FTimerDelegate hitCooldownDelegate;
	hitCooldownDelegate.BindUFunction(this, "SetCanHit", true); //Set up delegate to pass bool parameter to function after timer ends
	GetWorldTimerManager().SetTimer(HitCooldownTimer, hitCooldownDelegate, DefaultHitCooldown, false);
}

void ASwashCharacter::MeleeHitDummy(ASwashDummy* dummyRef)
{
	if (!CanHit)
		return;

	float blockTime = dummyRef->GetBlockTime();
	if (blockTime <= ParryTimeWindow)
	{
		//Get parried lelz
		StartStun();
	}
	else if (dummyRef->GetBlocking())
	{
		//Ya got blocked chump
		dummyRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * 300.0, 200.0));
	}
	else
	{
		//Ayooooo you hit somebody
		dummyRef->AddKnockback(FVector(0.0, GetActorForwardVector().Y * 600.0, 200.0));
	}

	//Set up hit cooldown timer
	SetCanHit(false);
	FTimerDelegate hitCooldownDelegate;
	hitCooldownDelegate.BindUFunction(this, "SetCanHit", true); //Set up delegate to pass bool parameter to function after timer ends
	GetWorldTimerManager().SetTimer(HitCooldownTimer, hitCooldownDelegate, DefaultHitCooldown, false);
}

void ASwashCharacter::SetCanHit(bool newCanHit)
{
	CanHit = newCanHit;
}

// Ranged Attack Functionality
void ASwashCharacter::StartRanged(const FInputActionValue& Value) 
{
	//...
}

void ASwashCharacter::EndRanged(const FInputActionValue& Value)
{
	//...
}

// Special Ability Functionality
void ASwashCharacter::StartSpecial(const FInputActionValue& Value) 
{
	//...
}

void ASwashCharacter::EndSpecial(const FInputActionValue& Value)
{
	//...
}

// Blocking Functionality
void ASwashCharacter::StartBlock(const FInputActionValue& Value) 
{
	if (IsStunned || GetCharacterMovement()->IsFalling())
		return;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Start Block"));

	IsBlocking = true;
	GetCharacterMovement()->StopMovementImmediately();
	GetController()->SetIgnoreMoveInput(true);
	//GetCharacterMovement()->DisableMovement();
	
	//Start block timer
	FTimerDelegate emptyDelegate;
	emptyDelegate.BindLambda([]() {}); //Empty function
	GetWorldTimerManager().SetTimer(BlockTimer, emptyDelegate, 1.0, true); //Loop this timer (inbLoop == true) while incrementing it (rate == -X.0) 
}

void ASwashCharacter::EndBlock(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("End Block"));

	IsBlocking = false;
	GetController()->SetIgnoreMoveInput(false);
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

	//Clear block timer
	GetWorldTimerManager().ClearTimer(BlockTimer);
}

bool ASwashCharacter::GetBlocking()
{
	return IsBlocking;
}

// Stun Functionality
void ASwashCharacter::StartStun()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Start Stun"));
	
	IsStunned = true;
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetController()->SetIgnoreMoveInput(true);
	
	//Stun Timer
	GetWorldTimerManager().SetTimer(StunTimer, this, &ASwashCharacter::EndStun, DefaultStunTime, false);
}

void ASwashCharacter::EndStun()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("End Stun"));
	
	IsStunned = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	GetController()->SetIgnoreMoveInput(false);

	//Stun Timer
	GetWorldTimerManager().ClearTimer(StunTimer);
}

bool ASwashCharacter::GetStunned()
{
	return IsStunned;
}

// Interact Functionality
void ASwashCharacter::Interact(const FInputActionValue& Value) 
{
	//...
}








