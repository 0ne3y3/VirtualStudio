// Copyright 0ne3y3 Lucas. All Rights Reserved.

#include "CharacterSubsystem.h"
#include "CharacterSettings.h"

void UCharacterSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();

	if(!CharacterSettings) return;

	MutableCharacterTable = CharacterSettings->MutableCharacterTable.LoadSynchronous();
	MutableSkinColorTable = CharacterSettings->MutableSkinColorTable.LoadSynchronous();
	MutableSkinMaterialTable = CharacterSettings->MutableSkinMaterialTable.LoadSynchronous();
	MutableBodyMeshTable = CharacterSettings->MutableBodyMeshTable.LoadSynchronous();
	MutableHeadMeshTable = CharacterSettings->MutableHeadMeshTable.LoadSynchronous();
	MutableIrisTextureTable = CharacterSettings->MutableIrisTextureTable.LoadSynchronous();
	MutableHighlightTextureTable = CharacterSettings->MutableHighlightTextureTable.LoadSynchronous();
	MutableCETextureTable = CharacterSettings->MutableCETextureTable.LoadSynchronous();
}

void UCharacterSubsystem::Deinitialize()
{
	Super::Deinitialize();

}