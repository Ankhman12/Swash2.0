// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	Hitbox->SetGenerateOverlapEvents(true);
	Hitbox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
