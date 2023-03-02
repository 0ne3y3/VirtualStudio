// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ModularMeshData.generated.h"

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
* Enum for part of the body
*/
UENUM( BlueprintType )
enum class EBodyPartType : uint8
{
	MainBody,
	Eye,
	Head,
	Hair,
	Torso,
	Arms,
	Hands,
	Legs,
	Feet,
	Accessory,
	Special
};

/**
* FHairCustomData
*
* Structure containing hair custom data.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FHairCustomData
{

	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray<FColor> HairFrontColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray<FColor> HairBackColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float SubsurfaceIntensity = 0.5;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 0, UIMin = 0 ) )
	float SubsurfaceColorBoost = 1;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float BaseSpecular = 0.2;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float BaseRoughness = 0.9;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float BaseMetal = 0;
};

/**
* FEyeCustomData
*
* Structure containing eye custom data.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FEyeCustomData
{

	GENERATED_USTRUCT_BODY()

	public:

	static const int32 MaxCustomData = 22;
	static const int32 LeftRightCustomDataIndex = 18;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ))
	TArray<float> BaseMetal;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	TArray<float> BaseSpecular;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	TArray<float> BaseRoughness;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, UIMin = 0 ) )
	TArray<float> ColorCurveSelection;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Transform" )
	TArray<FVector2D> IrisScale;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Transform" )
	TArray<FVector2D> IrisOffset;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color" )
	TArray<FLinearColor> ScleraColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	TArray<float> ScleraTextureOpacity;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color" )
	TArray<FLinearColor> ScleraTextureColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	TArray<float> ScleraShadowBrightness;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Transform" )
	TArray<FVector2D> ScleraShadowOffset;

	float GetCustomDataValue(int32 EyeIndex, int32 CustomDataIndex);
};

/**
* FClothCustomData
*
* Structure containing eye custom data.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FClothCustomData
{

	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 0, UIMin = 0 ) )
	int32 CelshadingCurve;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 0, UIMin = 0 ) )
	TArray<FColor> ClothColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = 0, UIMin = 0 ) )
	TArray<FVector> ClothProperties;
};

/**
* FSkinFaceCustomData
*
* Structure containing a body part mesh.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FSkinFaceCustomData
{

	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FColor SkinColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FColor SkinSubsurface;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FColor CheekColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FColor EyelashColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FColor MascaraColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FColor EyebrowColor;
};

/**
 * UModularSkeletalMeshData
 *
 * Non-mutable data asset that contains properties used to define a modular mesh.
 */
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Modular skeletal mesh data", ShortTooltip = "Data asset used to define a modular skeletal mesh." ) )
class CHARACTERS_API UModularSkeletalMeshData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ValidEnumValues = "MainBody, Head, Hair, Torso, Arms, Hands, Legs, Feet" ) )
	EBodyPartType MeshType;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, UIMin = 0 ))
	int32 ZOrder = 1;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	TMap<EBodyPartType, float> CoverPart;
};

/**
 * UModularStaticMeshData
 *
 * Non-mutable data asset that contains properties used to define a modular mesh.
 */
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Modular static mesh data", ShortTooltip = "Data asset used to define a modular static mesh." ) )
class CHARACTERS_API UModularStaticMeshData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ValidEnumValues = "Eye, Accessory" ) )
	EBodyPartType MeshType = EBodyPartType::Eye;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, UIMin = 0 ) )
	int32 ZOrder = 1;
};

/**
 * UMainBodyPostProcessAnimationData
 *
 * Non-mutable data asset that contains properties used for post process animation of main body.
 */
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Main body PP animation", ShortTooltip = "Data asset for main body's post process animation." ) )
class CHARACTERS_API UMainBodyPostProcessAnimationData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float UpperarmCorrectiveRoot = 0.65;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float UpperarmTwist1 = 0.2;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float UpperarmTwist2 = 0.8;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float LowerarmCorrectiveRoot = 0.5;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float LowerarmTwist1 = 0.4;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float LowerarmTwist2 = 0.8;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float ThighCorrectiveRoot = 0.6;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float ThighTwist = 0.5;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float CalfCorrectiveRoot = 0.5;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float CalfTwist = 0.5;
};

/**
* FBodyMesh
*
* Structure containing a body part mesh.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FBodyMesh
{

	GENERATED_USTRUCT_BODY()

	public:

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FName BodyPartName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UModularSkeletalMeshData> MeshData;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray<TSoftObjectPtr<UMaterialInstance>> Materials;
};

/**
* FAccessoryMesh
*
* Structure containing a accessory mesh.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FAccessoryMesh
{
	GENERATED_USTRUCT_BODY()

	public:

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FName BodyPartName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TObjectPtr<UModularStaticMeshData> MeshData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TArray<TSoftObjectPtr<UMaterialInstance>> Materials;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int32 NumberOfEyes = 2;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FName BoneNameToAttachTo;
};

/**
 * UMainBodyMeshData
 *
 * Non-mutable data asset that contains properties used to define the main body of a modular character.
 */
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Main body skeletal mesh data", ShortTooltip = "Data asset used to define the main body of a modular character." ) )
class CHARACTERS_API UMainBodyMeshData : public UModularSkeletalMeshData
{
	GENERATED_BODY()

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	bool bHaveHead = false;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( EditCondition = "bHaveHead" ) )
	FAccessoryMesh EyeData;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TObjectPtr<UMainBodyPostProcessAnimationData> PPAnimationData;
};



/**
 * UHeadMeshData
 *
 * Non-mutable data asset that contains properties used to define the head of a modular character.
 */
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Head skeletal mesh data", ShortTooltip = "Data asset used to define the head of a modular character." ) )
class CHARACTERS_API UHeadMeshData : public UModularSkeletalMeshData
{
	GENERATED_BODY()

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FAccessoryMesh EyeData;
};

/**
* FHumanBodyMorphology
*
* Structure containing a human body morphology.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FHumanBodyMorphology
{
	GENERATED_USTRUCT_BODY()

	public:

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body" )
	EBodyType BodyType;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body", meta = ( ClampMin = 0, UIMin = 0, ToolTip = "Height of the character" ) )
	float Height = 1.85f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|Legs", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase legs fat, bigger legs with roundish shape" ) )
	float LegsFat = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|Legs", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase legs muscles, bigger legs but the shape feel less fat" ) )
	float LegsMuscle = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase stomach and hips fat" ) )
	float BodyFat = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase stomach muscles" ) )
	float BodyMuscle = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase stomach and hips size" ) )
	float BodyPregnant = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase boob size and their physics" ) )
	float Boobs = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|Arms", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase arms muscles" ) )
	float ArmsMuscles = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Mouth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Make the mouth goes :3" ) )
	float CatMouth = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Mouth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Fix the CatMouth's bottom lip when the mouth is open. See it as a weight." ) )
	float CatMouthFixWhenOpen = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Teeth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase top canines size, for vampire/animal characters" ) )
	float CanineTop = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Teeth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase bottom canines size, for animal characters" ) )
	float CanineBottom = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Teeth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "When the top lip is opening, top teeth will follow it, used for anime character. See it as a weight." ) )
	float TeethTop = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Teeth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "When the top lip is opening, take the average of both morphtarget (left and right). After being superior to this threshold, start to move teeth up." ) )
	float TeethTopThreshold = 0;
};


/**
* UHumanBodyData
*
* Mutable data asset that contains all human body data. Create preset for character. /!\ Do not change values directly inside preset as it actually save it. Create a new custom one.
*/
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Human body data", ShortTooltip = "Data asset used to define a human." ) )
class CHARACTERS_API UHumanBodyData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
	FName PresetName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = ( ShowOnlyInnerProperties ) )
	FHumanBodyMorphology BodyMorphology;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh" )
	TArray<FBodyMesh> BodyMeshes;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh" )
	TArray<FAccessoryMesh> BodyAccessories;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Custom data" )
	FSkinFaceCustomData SkinAndFaceCustomData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Custom data" )
	FHairCustomData HairCustomData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Custom data" )
	FEyeCustomData EyeCustomData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Custom data" )
	FClothCustomData ClothCustomData;

	bool bIsPreset = true;

	void CopyDataFromInterface( FHumanBodyInterface& HumanBodyInterface );

	void CopyDataFromAnotherDataAsset( UHumanBodyData* InHumanBodyData );

	void CopyDataToInterface( FHumanBodyInterface& HumanBodyInterface );

	/* Get a reference of the MainBody mesh */
	FBodyMesh* GetMainBody();
};

/**
* FHumanBodyInterface
*
* Structure containing a human body properties for UI interface.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FHumanBodyInterface
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
	FName PresetName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = ( ShowOnlyInnerProperties ) )
	FHumanBodyMorphology BodyMorphology;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh" )
	TArray<FBodyMesh> BodyMeshes;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh" )
	TArray<FAccessoryMesh> BodyAccessories;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Custom data" )
	FSkinFaceCustomData SkinAndFaceCustomData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Custom data" )
	FHairCustomData HairCustomData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Custom data" )
	FEyeCustomData EyeCustomData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Custom data" )
	FClothCustomData ClothCustomData;
};