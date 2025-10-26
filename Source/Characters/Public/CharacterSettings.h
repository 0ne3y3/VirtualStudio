#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/DeveloperSettings.h"
#include "CharacterSettings.generated.h"

class UDataTable;

/**
 * Settings for character properties.
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Character"))
class CHARACTERS_API UCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	UPROPERTY(config, EditAnywhere, Category = Meshes, meta = (DisplayName = "Default body skeletal mesh",
		ToolTip = "Default body skeletal mesh used to set default var in cpp",
		ConfigRestartRequired = false))
	TSoftObjectPtr<USkeletalMesh> DefaultMutableBodyMesh;

	UPROPERTY(config, EditAnywhere, Category = Meshes, meta = (DisplayName = "Default head skeletal mesh",
		ToolTip = "Default head skeletal mesh used to set default var in cpp",
		ConfigRestartRequired = false))
	TSoftObjectPtr<USkeletalMesh> DefaultMutableHeadMesh;

	UPROPERTY(config, EditAnywhere, Category = Materials, meta = (DisplayName = "Default skin material",
		ToolTip = "Default skin material used to set default var in cpp",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UMaterialInterface> DefaultMutableSkinMat;

	UPROPERTY(config, EditAnywhere, Category = Materials, meta = (DisplayName = "Default head material",
		ToolTip = "Default head material used to set default var in cpp",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UMaterialInterface> DefaultMutableHeadMat;

	UPROPERTY(config, EditAnywhere, Category = Materials, meta = (DisplayName = "Default eye material",
		ToolTip = "Default eye material used to set default var in cpp",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UMaterialInterface> DefaultMutableEyeMat;

	UPROPERTY(config, EditAnywhere, Category = Materials, meta = (DisplayName = "Default cloth material",
		ToolTip = "Default cloth material used to set default var in cpp",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UMaterialInterface> DefaultMutableClothMat;

	UPROPERTY(config, EditAnywhere, Category = Textures, meta = (DisplayName = "Default RGB texture",
		ToolTip = "Default texture used to set default var in cpp",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UTexture2D> DefaultTexture;

	UPROPERTY(config, EditAnywhere, Category = Textures, meta = (DisplayName = "Default Grayscale texture",
		ToolTip = "Default texture used to set default var in cpp",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UTexture2D> DefaultGrayscaleTexture;

	UPROPERTY(config, EditAnywhere, Category = Textures, meta = (DisplayName = "Default RGBA texture",
		ToolTip = "Default texture used to set default var in cpp",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UTexture2D> DefaultRGBATexture;

	UPROPERTY(config, EditAnywhere, Category = Mutable, meta = (DisplayName = "Mutable character table",
		ToolTip = "Table of mutable object for characters.",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UDataTable> MutableCharacterTable;

	UPROPERTY(config, EditAnywhere, Category = Mutable, meta = (DisplayName = "Mutable skin color table",
		ToolTip = "Table of mutable skin color for characters.",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UDataTable> MutableSkinColorTable;

	UPROPERTY(config, EditAnywhere, Category = Mutable, meta = (DisplayName = "Mutable skin material table",
		ToolTip = "Table of mutable skin material for characters.",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UDataTable> MutableSkinMaterialTable;

	UPROPERTY(config, EditAnywhere, Category = Mutable, meta = (DisplayName = "Mutable body meshes table",
		ToolTip = "Table of mutable body meshes for characters.",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UDataTable> MutableBodyMeshTable;

	UPROPERTY(config, EditAnywhere, Category = Mutable, meta = (DisplayName = "Mutable head meshes table",
		ToolTip = "Table of mutable head meshes for characters.",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UDataTable> MutableHeadMeshTable;

	UPROPERTY(config, EditAnywhere, Category = Mutable, meta = (DisplayName = "Mutable iris textures table",
		ToolTip = "Table of eye iris textures for characters.",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UDataTable> MutableIrisTextureTable;

	UPROPERTY(config, EditAnywhere, Category = Mutable, meta = (DisplayName = "Mutable highlight textures table",
		ToolTip = "Table of eye highlight textures for characters.",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UDataTable> MutableHighlightTextureTable;

	UPROPERTY(config, EditAnywhere, Category = Mutable, meta = (DisplayName = "Mutable eye CE textures table",
		ToolTip = "Table of eye camera effect textures for characters.",
		ConfigRestartRequired = false))
	TSoftObjectPtr<UDataTable> MutableCETextureTable;
};