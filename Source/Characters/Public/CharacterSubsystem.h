// Copyright 0ne3y3 Lucas. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "ModularMeshData.h"
#include "CharacterSubsystem.generated.h"

/**
 * UCharacterSubsystem
 * Character subsytem.
 */
UCLASS()
class CHARACTERS_API UCharacterSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	public:

	// Get a reference to the base FModularSkeletalMeshData
	FModularSkeletalMeshData& GetDefaultModularSkeletalMeshData();

	// Get a reference to the base FModularMainBodyData
	FModularMainBodyData& GetDefaultModularMainBodyData();

	// Get a reference to the base FModularStaticMeshData
	FModularStaticMeshData& GetDefaultModularStaticMeshData();

	protected:
	FModularSkeletalMeshData ModularSkeletalMeshData;

	FModularMainBodyData ModularMainBodyData;

	FModularStaticMeshData ModularStaticMeshData;
};
