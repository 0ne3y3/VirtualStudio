// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HumanAnimationData.h"
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
	int32 HairFrontColorCurve;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	int32 HairBackColorCurve;

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
	static const int32 LeftRightCustomDataIndex = 18; // At which custom data index we tell the material this is the left or right eye

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|CustomData|General", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ))
	TArray<float> BaseMetal;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|CustomData|General", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	TArray<float> BaseSpecular;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|CustomData|General", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	TArray<float> BaseRoughness;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|CustomData|Color", meta = ( ClampMin = 0, UIMin = 0 ) )
	TArray<float> ColorCurveSelection;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|CustomData|Transform" )
	TArray<FVector2D> IrisScale;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|CustomData|Transform" )
	TArray<FVector2D> IrisOffset;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|CustomData|Color" )
	TArray<FLinearColor> ScleraColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|CustomData|Color", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	TArray<float> ScleraTextureOpacity;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|CustomData|Color" )
	TArray<FLinearColor> ScleraTextureColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|CustomData|Color", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	TArray<float> ScleraShadowBrightness;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|CustomData|Transform" )
	TArray<FVector2D> ScleraShadowOffset;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|Material", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	int32 MaterialIndex = 0;

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
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth|CustomData|General", meta = ( ClampMin = 0, UIMin = 0 ) )
	int32 CelshadingCurve;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth|CustomData|Color", meta = ( ClampMin = 0, UIMin = 0 ) )
	TArray<FColor> ClothColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth|CustomData|Properties", meta = ( ClampMin = 0, UIMin = 0 ) )
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

	static const int32 StartingIndexSkin = 9;
	static const int32 EndingIndexBody = 22;
	static const int32 EndingIndexFace = 25;

	public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin|CustomData|Color")
	int32 SkinAtlasCurve = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin|CustomData|Properties" )
	float BaseSpecular;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin|CustomData|Properties" )
	float BaseRoughness;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin|CustomData|Properties" )
	float BaseMetallic;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin|CustomData|Properties" )
	float BaseSubsurfaceOpacity;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin|CustomData|Color" )
	FLinearColor SubsurfaceColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|CustomData|Nail" )
	FLinearColor HandNailsColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|CustomData|Nail" )
	FLinearColor FeetNailsColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|CustomData|Color" )
	int32 EyelashColorCurve;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|CustomData|Color" )
	int32 EyebrowColorCurve;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|CustomData|Color" )
	FLinearColor InsideMouthColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|CustomData|Color" )
	FLinearColor CheekColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|CustomData|Color" )
	int32 MascaraColorCurve;


	float GetSkinCustomDataValue( int32 CustomDataIndex );
	float GetBodyCustomDataValue( int32 CustomDataIndex );
	float GetFaceCustomDataValue( int32 CustomDataIndex );

	TArray<float> CombineBodyData();
	TArray<float> CombineFaceData();
};

/**
 * USkeletalMeshData
 *
 * Non-mutable data asset that contains properties used to define a skeletal mesh for human body.
 */
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Skeletal mesh data", ShortTooltip = "Data asset used to define a skeletal mesh." ) )
class CHARACTERS_API USkeletalMeshData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
	FName PresetName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General", meta = ( ValidEnumValues = "MainBody, Head, Hair, Torso, Arms, Hands, Legs, Feet" ) )
	EBodyPartType MeshType;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
	TSoftObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General", meta = ( ClampMin = 0, UIMin = 0, EditCondition = "MeshType != EBodyPartType::Head && MeshType != EBodyPartType::MainBody", EditConditionHides ) )
	int32 ZOrder = 1;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General", meta = ( ClampMin = 0, UIMin = 0, EditCondition = "MeshType != EBodyPartType::Head && MeshType != EBodyPartType::MainBody", EditConditionHides ) )
	TMap<EBodyPartType, float> Covering;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
	TArray<TSoftObjectPtr<UMaterialInstance>> DefaultMaterials;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General", meta = ( EditCondition = "MeshType != EBodyPartType::Head && MeshType != EBodyPartType::Hair && MeshType != EBodyPartType::MainBody", EditConditionHides ) )
	FClothCustomData DefaultClothCustomData;
};

/**
 * UModularStaticMeshData
 *
 * Non-mutable data asset that contains properties used to define a skeletal mesh for human body.
 */
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Static mesh data", ShortTooltip = "Data asset used to define a static mesh." ) )
class CHARACTERS_API UStaticMeshData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
	FName PresetName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General", meta = ( ValidEnumValues = "Accessory, Special" ) )
	EBodyPartType MeshType = EBodyPartType::Accessory;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General" )
	TArray<FName> AvailableSocketToAttach;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General", meta = ( ClampMin = 0, UIMin = 0 ) )
	int32 ZOrder = 1;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material", meta = ( EditCondition = "MeshType!=EBodyPartType::Special", EditConditionHides ) )
	TArray<TSoftObjectPtr<UMaterialInstance>> DefaultMaterials;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "CustomData", meta = ( EditCondition = "MeshType!=EBodyPartType::Special", EditConditionHides ) )
	FClothCustomData DefaultClothCustomData;
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
* FModularSkeletalMeshData
*
* Structure containing a skeletal mesh.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FModularSkeletalMeshData
{

	GENERATED_USTRUCT_BODY()

	public:

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General" )
	TObjectPtr<USkeletalMeshData> MeshData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Custom data" )
	bool bUseCustomMaterial;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Custom data", meta = ( EditCondition = "bUseCustomMaterial", EditConditionHides ) )
	TArray<TSoftObjectPtr<UMaterialInstance>> CustomMaterials;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Custom data" )
	bool bUseCustomDataCloth;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Custom data", meta = ( EditCondition = "bUseCustomDataCloth", EditConditionHides ) )
	FClothCustomData ClothCustomData;
};

/**
* FModularStaticMeshData
*
* Structure containing data for a static mesh.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FModularStaticMeshData
{
	GENERATED_USTRUCT_BODY()

	public:

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Mesh")
	TObjectPtr<UStaticMeshData> MeshData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material" )
	bool bUseCustomMaterial;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material", meta = ( EditCondition = "bUseCustomMaterial", EditConditionHides ) )
	TArray<TSoftObjectPtr<UMaterialInstance>> CustomMaterials;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Custom data" )
	bool bUseCustomDataCloth;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Custom data", meta = ( EditCondition = "bUseCustomDataCloth", EditConditionHides ) )
	FClothCustomData ClothCustomData;
};

/**
 * UMainBodyMeshData
 *
 * Non-mutable data asset that contains properties used to define the main body of a human.
 */
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Main body skeletal mesh data", ShortTooltip = "Data asset used to define the main body of a human." ) )
class CHARACTERS_API UMainBodyMeshData : public USkeletalMeshData
{
	GENERATED_BODY()

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "AnimationBP" )
	TObjectPtr<UMainBodyPostProcessAnimationData> PPAnimationData;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "AnimationBP" )
	TSubclassOf<UAnimInstance> BodyAnimationBlueprint;
};

/**
 * UHeadMeshData
 *
 * Non-mutable data asset that contains properties used to define the head of a human.
 */
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Head skeletal mesh data", ShortTooltip = "Data asset used to define the head of a human." ) )
class CHARACTERS_API UHeadMeshData : public USkeletalMeshData
{
	GENERATED_BODY()

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Eye" )
	TSoftObjectPtr<UStaticMesh> EyeMesh;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Eye" )
	TArray<TSoftObjectPtr<UMaterialInstance>> EyeMaterials;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Eye", meta = ( ClampMin = 0, UIMin = 0 ) )
	int32 NumberOfEyes = 2;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eye|Animation", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float EyesMinimumScale = 0.1f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eye|Animation")
	FVector2D IrisDefaultPosition;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eye|Animation")
	FVector2D IrisMaxPositionIn;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eye|Animation")
	FVector2D IrisMaxPositionOut;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eye|Animation")
	FVector2D IrisMaxPositionUp;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eye|Animation")
	FVector2D IrisMaxPositionDown;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Animation" )
	TObjectPtr<UARKitPresetData> AnimationData;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Animation" )
	TSubclassOf<UAnimInstance> HeadAnimationBlueprint;
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

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|General" )
	EBodyType BodyType;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|General", meta = ( ClampMin = 0, UIMin = 0, ToolTip = "Height of the character" ) )
	float Height = 1.85f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase legs fat, bigger legs with roundish shape" ) )
	float LegsFat = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase legs muscles, bigger legs but the shape feel less fat" ) )
	float LegsMuscle = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase stomach and hips fat" ) )
	float BodyFat = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase stomach muscles" ) )
	float BodyMuscle = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase stomach and hips size" ) )
	float BodyPregnant = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase boob size and their physics" ) )
	float Boobs = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase arms muscles" ) )
	float ArmsMuscles = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Make the mouth goes :3" ) )
	float CatMouth = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Fix the CatMouth's bottom lip when the mouth is open. See it as a weight." ) )
	float CatMouthFixWhenOpen = 1.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase top canines size, for vampire/animal characters" ) )
	float CanineTop = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "Increase bottom canines size, for animal characters" ) )
	float CanineBottom = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "When the top lip is opening, top teeth will follow it, used for anime character. See it as a weight." ) )
	float TeethTop = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Morph", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1, ToolTip = "When the top lip is opening, take the average of both morphtarget (left and right). After being superior to this threshold, start to move teeth up." ) )
	float TeethTopThreshold = 0.f;
};

/**
* FHumanBodyData
*
* Structure containing a human body properties.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FHumanBodyData
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
	FName PresetName = "Custom";

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "GeneralData", meta = ( ShowOnlyInnerProperties ) )
	FHumanBodyMorphology BodyMorphology;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mesh" )
	FModularSkeletalMeshData MainBody;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mesh" )
	TArray<FModularSkeletalMeshData> SkeletalMeshes;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mesh" )
	TArray<FModularStaticMeshData> StaticMeshes;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "GeneralData", meta = ( ShowOnlyInnerProperties ) )
	FSkinFaceCustomData SkinAndFaceCustomData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "GeneralData", meta = ( ShowOnlyInnerProperties ) )
	FHairCustomData HairCustomData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "GeneralData", meta = ( ShowOnlyInnerProperties ) )
	FEyeCustomData EyeCustomData;

	bool IsCustomData() const;

	FModularSkeletalMeshData GetHeadData();
};

/**
* UHumanBodyPresetData
*
* Mutable data asset that contains all human body data. Create preset for character. /!\ Do not change values at runtime.
*/
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Human body data", ShortTooltip = "Data asset used to define a human body." ) )
class CHARACTERS_API UHumanBodyPresetData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( ShowOnlyInnerProperties ) )
	FHumanBodyData BodyData;
};