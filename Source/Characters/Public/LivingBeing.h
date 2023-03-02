// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LivingBeing.generated.h"

UCLASS()
class CHARACTERS_API ALivingBeing : public APawn
{
	GENERATED_BODY()

public:
	/** Default UObject constructor. */
	ALivingBeing( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
