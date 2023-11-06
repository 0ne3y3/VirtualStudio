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
	Head,
	Hair,
	Torso,
	Arms,
	Hands,
	Legs,
	Feet,
	BodyMAX,
	Accessory,
	Special
};

UENUM( BlueprintType )
enum class EBodyLayer : uint8
{
	Skin,
	Inside,
	Middle,
	Outer,
	Accessory,
};

/**
* FHairData
*
* Structure containing hair data.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FHairData
{

	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 PrimaryColorCurve1 = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 PrimaryColorCurve2 = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 PrimaryColorCurve3 = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 PrimaryBackColorCurve1 = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 PrimaryBackColorCurve2 = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 PrimaryBackColorCurve3 = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 AnisotropicColorCurve1 = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 AnisotropicColorCurve2 = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 AnisotropicColorCurve3 = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 AnisotropicBackColorCurve1 = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 AnisotropicBackColorCurve2 = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Color", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 AnisotropicBackColorCurve3 = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General", meta = ( ClampMin = 0, ClampMax = 5, UIMin = 0, UIMax = 5 ) )
	float SubsurfacePrimaryColorBoost = 1.2f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General", meta = ( ClampMin = 0, ClampMax = 5, UIMin = 0, UIMax = 5 ) )
	float SubsurfaceAnisotropicColorBoost = 1.05f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float BaseMetallic = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float RoughnessMax = 0.8f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float SpecularMin = 0.1f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float MinSubsurfaceOpacity = 0.8f;

	TMap<FName, float> GetHairTMap();
};

/**
* FEyeCustomData
*
* Structure containing eye custom data.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FEyeData
{
	GENERATED_USTRUCT_BODY()

	public:

	static const int32 MaxCustomData = 35; // Last custom data index
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

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|CustomData|Transform", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float EyesMinimumScale = 0.45f;

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
	float MaxCryingEffect = 0.1f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Material", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float EyelashShadowWeightClose = 0.35f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face|Material", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float EyelashShadowWeightOpen = 0.1f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|Material" )
	TArray<FLinearColor> ScleraColorBleeding;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes|Material" )
	TArray<TSoftObjectPtr<UMaterialInterface>> CustomEyesMaterials;

	float GetCustomDataValue(int32 EyeIndex, int32 CustomDataIndex);
};

/**
* FSkinFaceData
*
* Structure containing skin and face data.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FSkinFaceData
{

	FSkinFaceData() { 
	SkinSubsurfaceColorTint = FLinearColor(0.133f, 0.052435f, 0.047348f, 1.0f); 
	FaceCheekColor = FLinearColor( 0.041667f, 0.000866f, 0.001280f, 1.000000f );
	FaceLipsColor = FLinearColor( 0.041667f, 0.007015f, 0.003906f, 1.000000f );
	FaceTongueColor = FLinearColor( 0.114583f, 0.008355f, 0.010954f, 1.000000f );
	FaceInsideMouthColor = FLinearColor( 0.099899f, 0.005182f, 0.007499f, 1.000000f );
	FaceTeethColor = FLinearColor( 0.630000f, 0.630000f, 0.630000f, 1.000000f );
	FaceEffectYandereColor = FLinearColor( 0.022461f, 0.028193f, 0.093750f, 0.000000f );
	FaceEffectMadColor = FLinearColor( 0.656250f, 0.010254f, 0.031040f, 0.000000f );
	NailsColorHandLeft = FLinearColor( 0.135633f, 0.107235f, 0.109938f, 1.000000f );
	NailsColorHandRight = FLinearColor( 0.135633f, 0.107235f, 0.109938f, 1.000000f );
	NailsColorFootLeft = FLinearColor( 0.135633f, 0.107235f, 0.109938f, 1.000000f );
	NailsColorFootRight = FLinearColor( 0.135633f, 0.107235f, 0.109938f, 1.000000f );
	}

	GENERATED_USTRUCT_BODY()

	public:

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 SkinColorCurve = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float SkinBaseMetallic = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float SkinBaseRoughness = 0.8f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float SkinBaseSpecular = 0.1f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float SkinSubsurfaceOpacity = 0.5f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	FLinearColor SkinSubsurfaceColorTint;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Nails", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float NailsRoughness = 0.2f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Nails", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float NailsSpecular = 0.600000f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Nails" )
	FLinearColor NailsColorHandLeft;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Nails" )
	FLinearColor NailsColorHandRight;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Nails" )
	FLinearColor NailsColorFootLeft;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Nails" )
	FLinearColor NailsColorFootRight;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face" )
	FLinearColor FaceCheekColor;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face" )
	FLinearColor FaceLipsColor;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 FaceMascaraColorCurve = 0;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face" )
	FLinearColor FaceTongueColor;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face" )
	FLinearColor FaceInsideMouthColor;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face" )
	FLinearColor FaceTeethColor;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 FaceEyelashColorCurve = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face", meta = ( ClampMin = 0, ClampMax = 32, UIMin = 0, UIMax = 32 ) )
	int32 FaceEyebrowColorCurve = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float FaceEyebrowAndEyelashRoughness = 1.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float FaceInsideMouthAndTongueRoughness = 0.41f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float FaceEyebrowAndEyelashSpecular = 0.2f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float FaceInsideMouthAndTongueSpecular = 0.2f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "FaceEffect" )
	FLinearColor FaceEffectYandereColor;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "FaceEffect" )
	FLinearColor FaceEffectMadColor;

	TMap<FName, float> GetSkinFloatTMap();
	TMap<FName, FVector4> GetSkinVectorTMap();
};

/**
* FClothData
*
* Structure containing cloth data.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FClothData
{
	GENERATED_USTRUCT_BODY()

	FClothData()
	{
		ClothMaterialBaseProperties1 = FVector4(0.0f, 0.1f, 0.95f, 0.0f);
		ClothMaterialBaseProperties2 = FVector4( 0.0f, 0.5f, 0.4f, 0.0f );
		ClothMaterialBaseProperties3 = FVector4( 0.0f, 0.6f, 0.4f, 0.0f );
		ClothMaterialBaseProperties4 = FVector4( 0.0f, 0.1f, 0.95f, 0.0f );
	};

	public:

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Skin")
	bool bCanHaveSkin = true;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float ClothDetailTexture1Opacity = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth" )
	float ClothDetailTexture1Scale = 128.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float ClothDetailTexture2Opacity = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth" )
	float ClothDetailTexture2Scale = 128.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float ClothDetailTexture3Opacity = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth" )
	float ClothDetailTexture3Scale = 128.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float ClothDetailTexture4Opacity = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth" )
	float ClothDetailTexture4Scale = 128.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	FVector4 ClothMaterialBaseProperties1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	FVector4 ClothMaterialBaseProperties2;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	FVector4 ClothMaterialBaseProperties3;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	FVector4 ClothMaterialBaseProperties4;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = ( ClampMin = 0, ClampMax = 64, UIMin = 0, UIMax = 64 ) )
	int32 ClothColor1 = 2;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = ( ClampMin = 0, ClampMax = 64, UIMin = 0, UIMax = 64 ) )
	int32 ClothColor2 = 0;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = ( ClampMin = 0, ClampMax = 64, UIMin = 0, UIMax = 64 ) )
	int32 ClothColor3 = 1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = ( ClampMin = 0, ClampMax = 64, UIMin = 0, UIMax = 64 ) )
	int32 ClothColor4 = 1;

	TMap<FName, float> GetClothFloatTMap();
	TMap<FName, FVector4> GetClothVector4TMap();
};

/**
* FChildBodyPart
*
* Structure containing modular parent-child relation. Contained in the parent mesh, it's used to force skin or cloth properties for child cloth material index 0
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FChildBodyPart
{
	GENERATED_USTRUCT_BODY()

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	EBodyPartType MeshType = EBodyPartType::MainBody;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	bool bSkinMaterialUsed = true;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( EditCondition = "bSkinMaterialUsed == false", EditConditionHides ) )
	bool bForceUsingCloth = false;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = ( EditCondition = "bForceUsingCloth", EditConditionHides, ClampMin = 1, ClampMax = 4, UIMin = 1, UIMax = 4 ) )
	int32 ClothPropertiesIndex = 0;
};

/**
 * UMaterialData
 *
 * Non-mutable data asset used for cloth materials.
 */
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Skeletal mesh data", ShortTooltip = "Data asset used for cloth materials." ) )
class CHARACTERS_API UMaterialData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	TSoftObjectPtr<UMaterialInterface> PrimaryMaterial;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	TSoftObjectPtr<UMaterialInterface> SkinMaterial;
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
	EBodyPartType MeshType = EBodyPartType::Torso;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General", meta = ( EditCondition = "MeshType != EBodyPartType::Head && MeshType != EBodyPartType::MainBody && MeshType != EBodyPartType::Hair", EditConditionHides ) )
	EBodyLayer MeshLayer = EBodyLayer::Inside;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
	TSoftObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
	TArray<TObjectPtr<UMaterialData>> DefaultMaterials;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	float RainDropsSize = 30.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	float RainDropsTopMask = 2.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	float RainDropsSpeed = 1.f;

	TMap<FName, float> GetRainFloatTMap();

	FORCEINLINE bool operator==( const USkeletalMeshData* Rhs ) const
	{
		return MeshType == Rhs->MeshType && Rhs->Mesh == Mesh;
	}

	FORCEINLINE bool operator==(TYPE_OF_NULLPTR) const
	{
		return Mesh == nullptr;
	}
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

	UMainBodyMeshData() { MeshType = EBodyPartType::MainBody; MeshLayer = EBodyLayer::Skin;};

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "AnimationBP" )
	TObjectPtr<UMainBodyPostProcessAnimationData> PPAnimationData;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "AnimationBP" )
	TSubclassOf<UAnimInstance> BodyAnimationBlueprint;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	TSoftObjectPtr<UTexture2D> SkinBodyTexture;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	TSoftObjectPtr<UTexture2D> MorphBodyTexture;
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

	UHeadMeshData() {MeshType = EBodyPartType::Head ; MeshLayer = EBodyLayer::Skin;};

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Eye" )
	TSoftObjectPtr<UStaticMesh> EyeMesh;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Eye" )
	TArray<TSoftObjectPtr<UMaterialInterface>> DefaultEyesMaterials;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Eye", meta = ( ClampMin = 0, UIMin = 0 ) )
	int32 NumberOfEyes = 2;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Eye" )
	TArray<FTransform> EyesTransform;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eye" )
	FVector2D IrisDefaultPosition;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eye" )
	FVector2D IrisMaxPositionIn;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eye" )
	FVector2D IrisMaxPositionOut;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eye" )
	FVector2D IrisMaxPositionUp;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eye" )
	FVector2D IrisMaxPositionDown;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Tears" )
	TSoftObjectPtr<UStaticMesh> TearsMesh;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Tears" )
	TSoftObjectPtr<UMaterialInterface> TearsMaterial;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Animation" )
	TObjectPtr<UARKitPresetData> AnimationData;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	TSoftObjectPtr<UTexture2D> FaceTexture;
};

/**
 * UHairMeshData
 *
 * Non-mutable data asset that contains properties used to define the hair of a human.
 */
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Hair skeletal mesh data", ShortTooltip = "Data asset used to define hairs of a human." ) )
class CHARACTERS_API UHairMeshData : public USkeletalMeshData
{
	GENERATED_BODY()

	UHairMeshData() { MeshType = EBodyPartType::Hair; MeshLayer = EBodyLayer::Skin; RainVariationTextureScaleX = FVector4(0.028799f, -0.068f, -0.0575f); RainVariationTextureScaleY = FVector4(0.078f, -0.02f, -0.0545f); AnisotropicTextureScale = FVector4(8.f, 0.05f, 1.f, 0.15f); };

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	float AnisotropicStrength = 2.2f;
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	FVector4 AnisotropicTextureScale;
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	float AnisotropicHFRange = 0.09f;
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	float AnisotropicHFOffset = -0.32f;
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	float AnisotropicLFRange = 0.04f;
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	float AnisotropicLFOffset = -0.1f;
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	float AnisotropicBreakTextureScaleX = 200.f;
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	float AnisotropicBreakTextureScaleY = 0.25f;
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	float AnisotropicBreakTextureContrast = 8.f;
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material", meta = ( ClampMin = 0, UIMin = 0 ) )
	int32 AnisotropicHFCurve = 0.f;
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material", meta = ( ClampMin = 0, UIMin = 0 ) )
	int32 AnisotropicLFCurve = 0.f;
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	FVector4 RainVariationTextureScaleX;
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	FVector4 RainVariationTextureScaleY;

	TMap<FName, float> GetHairMeshFloatTMap();
	TMap<FName, FVector4> GetHairMeshVector4TMap();
};

/**
 * UClothMeshData
 *
 * Non-mutable data asset that contains properties used to define the cloth of a human.
 */
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Cloth skeletal mesh data", ShortTooltip = "Data asset used to define cloth of a human." ) )
class CHARACTERS_API UClothMeshData : public USkeletalMeshData
{
	GENERATED_BODY()

	public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Cloth" )
	FClothData DefaultClothData;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Cloth" )
	TArray<EBodyPartType> ParentHierarchy;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Cloth", meta = ( TitleProperty="{MeshType}" ) )
	TArray<FChildBodyPart> ChildBodyParts;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Cloth" )
	bool bDefaultUseSkin;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	TSoftObjectPtr<UTexture2D> ClothDetailTexture1;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	TSoftObjectPtr<UTexture2D> ClothDetailTexture2;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	TSoftObjectPtr<UTexture2D> ClothDetailTexture3;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Material" )
	TSoftObjectPtr<UTexture2D> ClothDetailTexture4;

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

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material", meta = ( EditCondition = "MeshType!=EBodyPartType::Special", EditConditionHides ) )
	TArray<TObjectPtr<UMaterialData>> DefaultMaterials;
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

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material" )
	bool bUseCustomMaterial;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material" )
	bool bUseSkin;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material", meta = ( EditCondition = "bUseCustomMaterial", EditConditionHides ) )
	TArray<TObjectPtr<UMaterialData>> CustomMaterials;

	UPROPERTY( VisibleInstanceOnly, BlueprintReadWrite )
	FClothData CurrentClothData;

	void CopyPropertiesFromParent( FModularSkeletalMeshData& ParentData, bool bIsParentSetup );
	
	FORCEINLINE bool operator==( const FModularSkeletalMeshData& Rhs ) const
	{
		return Rhs.MeshData == MeshData;
	}
};

/**
* FModularMainBodyData
*
* Structure containing a skeletal mesh.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FModularMainBodyData
{

	GENERATED_USTRUCT_BODY()

	public:

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General" )
	TObjectPtr<UMainBodyMeshData> MeshData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material" )
	bool bUseCustomMaterial;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material", meta = ( EditCondition = "bUseCustomMaterial", EditConditionHides ) )
	TArray<TObjectPtr<UMaterialData>> CustomMaterials;

	FORCEINLINE bool operator==( const FModularSkeletalMeshData& Rhs ) const
	{
		return Rhs.MeshData == MeshData;
	}
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
	TArray<TSoftObjectPtr<UMaterialInterface>> CustomMaterials;
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

	TMap<FName, float> GetBodyMorphologyTMap();
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

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General" )
	FHumanBodyMorphology BodyMorphology;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mesh" )
	FModularMainBodyData MainBody;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=( TitleProperty= "{MeshData}" ))
	TArray<FModularSkeletalMeshData> SkeletalMeshes;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mesh" )
	TArray<FModularStaticMeshData> StaticMeshes;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Data" )
	FSkinFaceData SkinAndFaceData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Data" )
	FHairData HairData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Data" )
	FEyeData EyeData;

	bool IsCustomData() const;
	FModularSkeletalMeshData& GetSkeletalMeshData(EBodyPartType InBodyPartType);
	void RemoveOneSkeletalMeshPart(EBodyPartType BodyPartToRemove);
	EBodyPartType GetNextBodyPartToLoad( EBodyPartType CurrentBodyPart );
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