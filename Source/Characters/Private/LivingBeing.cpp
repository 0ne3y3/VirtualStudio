// Fill out your copyright notice in the Description page of Project Settings.


#include "LivingBeing.h"

// Sets default values
ALivingBeing::ALivingBeing( const FObjectInitializer& ObjectInitializer) : Super( ObjectInitializer )
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALivingBeing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALivingBeing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALivingBeing::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

