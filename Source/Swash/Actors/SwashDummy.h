// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SwashDummy.generated.h"

UCLASS()
class SWASH_API ASwashDummy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASwashDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FTimerHandle BlockTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FTimerHandle SwitchTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DefaultSwitchTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AddKnockback(FVector Impulse);

	void StartBlock();

	void EndBlock();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetBlocking();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetBlockTime();

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
