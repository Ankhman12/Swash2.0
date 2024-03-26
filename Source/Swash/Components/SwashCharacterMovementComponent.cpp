//By Mitchell Dunning 2024

#include "SwashCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "../Actors/SwashCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

USwashCharacterMovementComponent::USwashCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)	: Super(ObjectInitializer)
{
	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.0f, 3072.0f, 0.0f);
	AirControl = 1.f;
	Mass = 500.f;
	JumpZVelocity = 1450.f;
	MaxWalkSpeed = 700.f;
	GravityScale = 3.f;
	MaxAcceleration = 4096.f;
	bConstrainToPlane = true;
	bOrientRotationToMovement = true;
	UMovementComponent::SetPlaneConstraintNormal(FVector::UnitX());
	SetMovementMode(EMovementMode::MOVE_Walking);
}

void USwashCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	//SwashCharacterOwner = Cast<ASwashCharacter>(GetOwner());

}

void USwashCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
		case ECustomMovementMode::CMOVE_Climbing:
			//...
			break;
		case ECustomMovementMode::CMOVE_OnWall:
			//...
			break;
		case ECustomMovementMode::CMOVE_Hanging:
			//...
			break;
	}
}

void USwashCharacterMovementComponent::TickComponent(float DeltaSeconds, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaSeconds, TickType, ThisTickFunction);

	//Lock Actor on X axis
	//FVector currentLocation = GetCapsuleComponent()->GetComponentLocation();
	//FVector newLocation = FVector(0.0, currentLocation.Y, currentLocation.Z);
	//SetActorLocation(newLocation, false, nullptr, ETeleportType::TeleportPhysics);

	//Ledge Hang Updates
	if (IsHanging)
	{
		LedgeHangUpdate();
	}
	else if (MovementMode == EMovementMode::MOVE_Falling)
	{
		LedgeCheck();
	}
}

#pragma region "Helper Methods"
bool USwashCharacterMovementComponent::IsCustomMovementMode(const ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

bool USwashCharacterMovementComponent::CanPerfromPhysMovement() const
{
	return !CharacterOwner || !CharacterOwner->Controller && !bRunPhysicsWithNoController;
}

bool USwashCharacterMovementComponent::CanPerformFrameTickMovement(const float RemainingTime, const int32 Iterations) const
{
	return RemainingTime >= MIN_TICK_TIME && Iterations < MaxSimulationIterations && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController);
}
#pragma endregion

// Ledge Climbing Functionality
void USwashCharacterMovementComponent::LedgeCheck()
{
	//if (!CanLedgeCheck || (MovementMode != EMovementMode::MOVE_Falling) || IsHanging)
	//	return;

	//FVector traceForwardOffset = CharacterOwner->GetActorLocation() + (CharacterOwner->GetActorForwardVector() * 100.0);

	//FVector grabbableStartPos = traceForwardOffset + FVector(0.0, 0.0, 30.0);
	//FVector grabbableEndPos = traceForwardOffset + FVector(0.0, 0.0, -50.0);

	//float traceRadius = 10.0;

	//FHitResult hit;
	//FVector upVec = FVector(0.0, 0.0, 1.0);

	//if (GetWorld()->LineTraceSingleByObjectType(hit, grabbableStartPos, grabbableEndPos, FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic))
	//	&& hit.Normal.Dot(upVec) >= 1.0
	//	&& (GetLastMovementInputVector().X * CharacterOwner->GetActorForwardVector().Y) > 0.0)
	//{
	//	//We Hit a grabbable ledge!
	//	StartLedgeHang(hit.Location);
	//	DrawDebugSphere(GetWorld(), hit.ImpactPoint, traceRadius * 5, 20, FColor::Green);

	//}

	//DrawDebugSphere(GetWorld(), hit.ImpactPoint, traceRadius, 20, FColor::Red);

}

void USwashCharacterMovementComponent::StartLedgeHang(FVector LedgePos)
{
	//StopMovementImmediately();
	//DisableMovement(); // Movement mode set to MOVE_None
	//FVector hangPos = LedgePos + FVector(0.0, -65.0 * CharacterOwner->GetActorForwardVector().Y, -75.0);
	//CharacterOwner->SetActorLocation(hangPos, false, nullptr, ETeleportType::ResetPhysics);
	//IsHanging = true;
	//IsHoldingJump = false; //set jump to false so we need to press it again to jump off
}

void USwashCharacterMovementComponent::LedgeHangUpdate()
{
	//bool didMoveAwayFromLedge = (GetLastMovementInputVector().X * CharacterOwner->GetActorForwardVector().Y) < 0.0;
	//if (didMoveAwayFromLedge)
	//{
	//	EndLedgeHang();
	//}

	//if (IsHoldingJump)
	//{
	//	EndLedgeHang();
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Should Jump Off Ledge"));
	//	//LaunchCharacter(FVector(0.0, 0.0, LedgeJumpOffForce), false, false);
	//}
}

void USwashCharacterMovementComponent::EndLedgeHang()
{
	//IsHanging = false;
	//SetMovementMode(EMovementMode::MOVE_Falling);

	////Ledge Hang Cooldown
	//SetCanLedgeCheck(false);
	//FTimerDelegate hangCooldownDelegate;
	//hangCooldownDelegate.BindUFunction(this, "SetCanLedgeCheck", true); //Set up delegate to pass bool parameter to function after timer ends
	//GetWorldTimerManager().SetTimer(HangCooldownTimer, hangCooldownDelegate, DefaultHangCooldown, false);
}

void USwashCharacterMovementComponent::SetCanLedgeCheck(bool canCheck)
{
	/*GetWorldTimerManager().ClearTimer(HangCooldownTimer);
	CanLedgeCheck = canCheck;*/
}