// Fill out your copyright notice in the Description page of Project Settings.


#include "SwashCharacterAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Actors/SwashCharacter.h"


USwashCharacterAnimInstance::USwashCharacterAnimInstance()
{
	//set variable defaults? Could also do in header
}

void USwashCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerReference = Cast<ASwashCharacter>(TryGetPawnOwner());
}

void USwashCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (PlayerReference)
	{
		UpdateAnimProperties();
	}
	else
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASwashCharacter::StaticClass(), OutActors);

		for (AActor* actor : OutActors)
		{
			PlayerReference = Cast<ASwashCharacter>(actor);
			if (PlayerReference)
			{
				UpdateAnimProperties();
				break;
			}
		}
	}

}

void USwashCharacterAnimInstance::UpdateAnimProperties()
{
	if (!PlayerReference->IsValidLowLevel())
		return;
	
	Velocity = PlayerReference->GetCharacterMovement()->Velocity;
	GroundSpeed = FMath::Sqrt(FMath::Pow(Velocity.X, 2) + FMath::Pow(Velocity.Y, 2));
	ShouldMove = GroundSpeed > 3.0 && PlayerReference->GetCharacterMovement()->GetCurrentAcceleration() != FVector::ZeroVector;
	IsFalling = PlayerReference->GetCharacterMovement()->IsFalling();
	IsHanging = PlayerReference->GetHanging();
	IsBlocking = PlayerReference->GetBlocking();
	IsStunned = PlayerReference->GetStunned();
}