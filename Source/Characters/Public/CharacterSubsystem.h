// Copyright 0ne3y3 Lucas. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "CharacterSubsystem.generated.h"

class UDataTable;

/**
 * UCharacterSubsystem
 * Character subsytem.
 */
UCLASS()
class CHARACTERS_API UCharacterSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	public:

	// Begin USubsystem
	
	// Initialize the subsystem, to setup default project values
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
	// End USubsystem

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> MutableCharacterTable;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> MutableSkinColorTable;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> MutableSkinMaterialTable;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> MutableBodyMeshTable;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> MutableHeadMeshTable;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> MutableIrisTextureTable;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> MutableHighlightTextureTable;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> MutableCETextureTable;
};
