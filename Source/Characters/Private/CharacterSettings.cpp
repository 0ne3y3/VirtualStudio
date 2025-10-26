#include "CharacterSettings.h"
#include "CharacterSubsystem.h"

#if WITH_EDITOR
void UCharacterSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if(PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterSettings, MutableCharacterTable))
	{
		UCharacterSubsystem* CharacterSubsystem = GEngine->GetEngineSubsystem<UCharacterSubsystem>();
		if(CharacterSubsystem)
		{
			CharacterSubsystem->MutableCharacterTable = MutableCharacterTable.LoadSynchronous();
		}
	}
	if(PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterSettings, MutableSkinColorTable))
	{
		UCharacterSubsystem* CharacterSubsystem = GEngine->GetEngineSubsystem<UCharacterSubsystem>();
		if(CharacterSubsystem)
		{
			CharacterSubsystem->MutableSkinColorTable = MutableSkinColorTable.LoadSynchronous();
		}
	}
	if(PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterSettings, MutableSkinMaterialTable))
	{
		UCharacterSubsystem* CharacterSubsystem = GEngine->GetEngineSubsystem<UCharacterSubsystem>();
		if(CharacterSubsystem)
		{
			CharacterSubsystem->MutableSkinMaterialTable = MutableSkinMaterialTable.LoadSynchronous();
		}
	}
	if(PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterSettings, MutableBodyMeshTable))
	{
		UCharacterSubsystem* CharacterSubsystem = GEngine->GetEngineSubsystem<UCharacterSubsystem>();
		if(CharacterSubsystem)
		{
			CharacterSubsystem->MutableBodyMeshTable = MutableBodyMeshTable.LoadSynchronous();
		}
	}
	if(PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterSettings, MutableHeadMeshTable))
	{
		UCharacterSubsystem* CharacterSubsystem = GEngine->GetEngineSubsystem<UCharacterSubsystem>();
		if(CharacterSubsystem)
		{
			CharacterSubsystem->MutableHeadMeshTable = MutableHeadMeshTable.LoadSynchronous();
		}
	}
	if(PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterSettings, MutableIrisTextureTable))
	{
		UCharacterSubsystem* CharacterSubsystem = GEngine->GetEngineSubsystem<UCharacterSubsystem>();
		if(CharacterSubsystem)
		{
			CharacterSubsystem->MutableIrisTextureTable = MutableIrisTextureTable.LoadSynchronous();
		}
	}
	if(PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterSettings, MutableHighlightTextureTable))
	{
		UCharacterSubsystem* CharacterSubsystem = GEngine->GetEngineSubsystem<UCharacterSubsystem>();
		if(CharacterSubsystem)
		{
			CharacterSubsystem->MutableHighlightTextureTable = MutableHighlightTextureTable.LoadSynchronous();
		}
	}
	if(PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterSettings, MutableHighlightTextureTable))
	{
		UCharacterSubsystem* CharacterSubsystem = GEngine->GetEngineSubsystem<UCharacterSubsystem>();
		if(CharacterSubsystem)
		{
			CharacterSubsystem->MutableCETextureTable = MutableCETextureTable.LoadSynchronous();
		}
	}
}
#endif