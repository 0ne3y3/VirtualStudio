// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HumanAnimationData.h"
#include "ModularMeshData.generated.h"

class UCustomizableObject;

/**
* Enum for body type (Male/Female)
*/
UENUM( BlueprintType )
enum class EBodyType : uint8
{
	Male,
	Female
};

/**
* Enum for body layer
*/
UENUM( BlueprintType )
enum class EBodyLayer : uint8
{
	Skin,
	Inside,
	Outer,
	Accessory
};

/**
* FMutableIrisTexture
*
* Structure containing the eye iris texture for character.
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FMutableIrisTexture : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FMutableIrisTexture();

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> IrisTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> HeightTexture;
};

/**
* FMutableHighlightTexture
*
* Structure containing the highlight texture for character.
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FMutableHighlightTexture : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FMutableHighlightTexture();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> HighlightTexture;
};

/**
* FMutableCETexture
*
* Structure containing the eye camera effect texture for character.
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FMutableCETexture : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FMutableCETexture();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> CETexture;
};

/**
* FMutableCharacterObject
*
* Structure containing the customizable object for creating mutable character.
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FMutableCharacterObject : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UCustomizableObject> CustomObject;
};

/**
* FMutableHumanHead
*
* Structure containing a head skeletal mesh and its default properties.
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FMutableHumanHead : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FMutableHumanHead();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBodyType BodyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> HeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInterface> DefaultHeadMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInterface> DefaultEyeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	TSoftObjectPtr<UTexture2D> HeadTexture;

	// !! SHOULD BE THE SAME AS HeadTexture !!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures", meta = (ToolTip = "!! SHOULD BE THE SAME AS HeadTexture !!"))
	TSoftObjectPtr<UTexture2D> HeadTextureMutable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	TSoftObjectPtr<UTexture2D> HairShaved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	TSoftObjectPtr<UTexture2D> HeadMask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	TSoftObjectPtr<UTexture2D> DefaultDetailsTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FaceEffectMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FaceEffectMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoseMaskContrast;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Position")
	float IrisDefaultPositionX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Position")
	float IrisDefaultPositionY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Position")
	FVector2D IrisMaxPositionInOut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Position")
	FVector2D IrisMaxPositionUpDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Scale")
	float EyesScaleX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Scale")
	float EyesScaleY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Highlight")
	float HighlightMaxScaleX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Highlight")
	float HighlightMaxScaleY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Highlight")
	float HighlightMaxRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|CE")
	FLinearColor CameraEffectTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Sclera")
	TSoftObjectPtr<UTexture2D> DefaultScleraTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Sclera")
	FLinearColor ScleraTextureTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Sclera")
	float ScleraDefaultDarkCornerOffsetY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Sclera")
	float ScleraDarkCornerEyeCloseWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Sclera")
	float ScleraDarkCornerEyeWideWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Pupil")
	float PupilOffsetMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Pupil")
	float PupilOffsetMax;
};

/**
* FMutableHumanHeadProperties
*
* Structure containing a mostly head properties you want to change per character.
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FMutableHumanHeadProperties : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FMutableHumanHeadProperties();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
	int32 EyeColorCurveIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
	float EyesMinimumScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
	float EyeMetallic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
	float IrisSpecular;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
	float IrisRoughness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
	float IrisHueShift;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Sclera")
	float ScleraDarkCornerHardness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Sclera")
	float ScleraSpecular;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Sclera")
	float ScleraRoughness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Sclera")
	FLinearColor ScleraColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Sclera")
	FLinearColor ScleraDarkCornerColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Sclera")
	FLinearColor ScleraTextureColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Highlight")
	FLinearColor HighlightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|Highlight")
	FLinearColor HighlightBorderColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|CE")
	float CameraEffectOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|CE")
	float CameraEffectSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|CE")
	FLinearColor CameraEffectTopColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|CE")
	FLinearColor CameraEffectMiddleColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes|CE")
	FLinearColor CameraEffectBottomColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouth")
	FLinearColor TeethColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouth")
	FLinearColor TongueColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouth")
	FLinearColor MouthInsideColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouth")
	FLinearColor LipsColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouth")
	float LipsLineDarkness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouth")
	float MouthAndTongueSpecular;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mouth")
	float MouthAndTongueRoughness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	FLinearColor CheekColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	FLinearColor EyelidColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	FLinearColor NoseColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face|Eyelash")
	FLinearColor EyelashBaseColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face|Eyelash")
	FLinearColor EyelashSecondaryColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face|Eyelash")
	FLinearColor EyelashTertiaryColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face|Eyebrow")
	FLinearColor EyebrowBaseColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face|Eyebrow")
	FLinearColor EyebrowSecondaryColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face|Eyebrow")
	FLinearColor EyebrowTertiaryColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face|Beard")
	FLinearColor BeardColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face|Beard")
	float BeardAndHairShavedOpacity;
};

/**
* FMutableHumanSkin
*
* Structure containing a skin tone properties.
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FMutableHumanSkin : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FMutableHumanSkin();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkinName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( ClampMin=0, UIMin=0 ))
	float SkinColorCurveIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor SkinSubsurfaceTintColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1))
	float SkinLineartDarkness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1))
	float SkinOutlineDarkness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor FaceEffectYandereColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor FaceEffectMadColor;
};

/**
* FMutableHumanBody
*
* Structure containing a body skeletal mesh and its default properties.
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FMutableHumanBody : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FMutableHumanBody();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBodyType BodyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> BobyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInterface> BodyDefaultMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	TSoftObjectPtr<UTexture2D> GrayscaleSkinTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	TSoftObjectPtr<UTexture2D> NailsMaskTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
	TSoftObjectPtr<UTexture2D> TopHeadTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSoftObjectPtr<UPostProcessBodyPreset> BodyAnimationPP;
};

/**
* FMutableHumanBodyProperties
*
* Structure containing a mostly body properties you want to change per character.
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FMutableHumanBodyProperties : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FMutableHumanBodyProperties();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nails")
	FLinearColor NailsHandLeftColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nails")
	FLinearColor NailsHandRightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nails")
	FLinearColor NailsFootLeftColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nails")
	FLinearColor NailsFootRightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skin")
	float SkinBaseMetallic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skin")
	float SkinBaseSpecular;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skin")
	float SkinBaseRoughness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skin")
	float SkinSubsurfaceOpacity;
};

UCLASS(BlueprintType, Const, Meta = (DisplayName = "Mutable Human Head Preset", ShortTooltip = "Data asset used to define a mutable character head."))
class CHARACTERS_API UMutableHeadPreset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UMutableHeadPreset();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName PresetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName HeadMeshRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName IrisTextureRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName HighlightTextureRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CETextureRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMutableHumanHeadProperties HeadProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPoseAsset> TonguePoseAsset;
};

UCLASS(BlueprintType, Const, Meta = (DisplayName = "Mutable Human Body Preset", ShortTooltip = "Data asset used to define a mutable character body."))
class CHARACTERS_API UMutableBodyPreset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UMutableBodyPreset();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName PresetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BodyMeshRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkinColorRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMutableHumanBodyProperties BodyProperties;
};

UCLASS(BlueprintType, Const, Meta = (DisplayName = "Mutable Human Character Preset", ShortTooltip = "Data asset used to define a mutable complete character."))
class CHARACTERS_API UMutableCharacterPreset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UMutableCharacterPreset();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName PresetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMutableBodyPreset> BodyPreset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMutableHeadPreset> HeadPreset;
};