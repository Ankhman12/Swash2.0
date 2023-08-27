// Fill out your copyright notice in the Description page of Project Settings.


#include "SwashDummy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASwashDummy::ASwashDummy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASwashDummy::BeginPlay()
{
	Super::BeginPlay();

	EndBlock();
}

// Called every frame
void ASwashDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector currentLocation = GetCapsuleComponent()->GetComponentLocation();
	FVector newLocation = FVector(0.0, currentLocation.Y, currentLocation.Z);
	SetActorLocation(newLocation, false, nullptr, ETeleportType::TeleportPhysics);

}

void ASwashDummy::AddKnockback(FVector Impulse)
{
	if (GetCharacterMovement()->IsFalling()) //If the character is in the air, don't apply knockback
		return;

	LaunchCharacter(Impulse, false, false); // Add instant knockback force with given impulse
}

void ASwashDummy::StartBlock()
{
	IsBlocking = true;
	GetWorldTimerManager().ClearTimer(SwitchTimer);
	GetWorldTimerManager().SetTimer(BlockTimer, this, &ASwashDummy::EndBlock, 2.0, true); //Loop this timer (inbLoop == true) while incrementing it (rate == -X.0)
}

void ASwashDummy::EndBlock()
{
	IsBlocking = false;
	GetWorldTimerManager().ClearTimer(BlockTimer);
	GetWorldTimerManager().SetTimer(SwitchTimer, this, &ASwashDummy::StartBlock, 2.0, true); //Loop this timer (inbLoop == true) while incrementing it (rate == -X.0)
}

bool ASwashDummy::GetBlocking()
{
	return IsBlocking;
}

float ASwashDummy::GetBlockTime()
{
	return GetWorldTimerManager().GetTimerElapsed(BlockTimer);
}

// Called to bind functionality to input
//void ASwashDummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

