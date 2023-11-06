// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularMeshData.h"
#include "CharacterSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC( LogCharacter, Log, All );

float FEyeData::GetCustomDataValue( int32 EyeIndex, int32 CustomDataIndex )
{
	switch( CustomDataIndex )
	{
		case 0:
		return ( BaseMetal.IsValidIndex( EyeIndex ) ) ? BaseMetal[EyeIndex] : 0;
		case 1:
		return ( BaseSpecular.IsValidIndex( EyeIndex ) ) ? BaseSpecular[EyeIndex] : 0;
		case 2:
		return ( BaseRoughness.IsValidIndex( EyeIndex ) ) ? BaseRoughness[EyeIndex] : 0;
		case 3:
		return ( ColorCurveSelection.IsValidIndex( EyeIndex ) ) ? ColorCurveSelection[EyeIndex] : 0;
		case 4:
		return ( IrisScale.IsValidIndex( EyeIndex ) ) ? IrisScale[EyeIndex].X : 0;
		case 5:
		return ( IrisScale.IsValidIndex( EyeIndex ) ) ? IrisScale[EyeIndex].Y : 0;
		case 6:
		return ( IrisOffset.IsValidIndex( EyeIndex ) ) ? IrisOffset[EyeIndex].X : 0;
		case 7:
		return ( IrisOffset.IsValidIndex( EyeIndex ) ) ? IrisOffset[EyeIndex].Y : 0;
		case 8:
		return ( ScleraColor.IsValidIndex( EyeIndex ) ) ? ScleraColor[EyeIndex].R : 0;
		case 9:
		return ( ScleraColor.IsValidIndex( EyeIndex ) ) ? ScleraColor[EyeIndex].G : 0;
		case 10:
		return ( ScleraColor.IsValidIndex( EyeIndex ) ) ? ScleraColor[EyeIndex].B : 0;
		case 11:
		return ( ScleraTextureOpacity.IsValidIndex( EyeIndex ) ) ? ScleraTextureOpacity[EyeIndex] : 0;
		case 12:
		return ( ScleraTextureColor.IsValidIndex( EyeIndex ) ) ? ScleraTextureColor[EyeIndex].R : 0;
		case 13:
		return ( ScleraTextureColor.IsValidIndex( EyeIndex ) ) ? ScleraTextureColor[EyeIndex].G : 0;
		case 14:
		return ( ScleraTextureColor.IsValidIndex( EyeIndex ) ) ? ScleraTextureColor[EyeIndex].B : 0;
		case 15:
		return ( ScleraShadowBrightness.IsValidIndex( EyeIndex ) ) ? ScleraShadowBrightness[EyeIndex] : 0;
		case 16:
		return ( ScleraShadowOffset.IsValidIndex( EyeIndex ) ) ? ScleraShadowOffset[EyeIndex].X : 0;
		case 17:
		return ( ScleraShadowOffset.IsValidIndex( EyeIndex ) ) ? ScleraShadowOffset[EyeIndex].Y : 0;
		default:
		return 0;
	}
}

bool FHumanBodyData::IsCustomData() const
{
	return PresetName == "Custom";
}

void FHumanBodyData::RemoveOneSkeletalMeshPart( EBodyPartType BodyPartToRemove )
{
	for( int32 Index = 0; Index < SkeletalMeshes.Num(); Index++ )
	{
		if( SkeletalMeshes[Index].MeshData->MeshType == BodyPartToRemove )
		{
			SkeletalMeshes.RemoveAt(Index);
		}
	}
}

FModularSkeletalMeshData& FHumanBodyData::GetSkeletalMeshData( EBodyPartType InBodyPartType )
{
	for( FModularSkeletalMeshData& SkeletalMeshData : SkeletalMeshes )
	{
		if( SkeletalMeshData.MeshData && SkeletalMeshData.MeshData->MeshType == InBodyPartType ) return SkeletalMeshData;
	}

	UCharacterSubsystem* CharacterSubsystem = GEngine->GetEngineSubsystem<UCharacterSubsystem>();
	return CharacterSubsystem->GetDefaultModularSkeletalMeshData();
}

TMap<FName, float> FSkinFaceData::GetSkinFloatTMap()
{
	TMap<FName, float> FloatMap;

	FloatMap.Add( TTuple<FName, float>( TEXT( "SkinColorCurve" ), (float)SkinColorCurve ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "SkinBaseMetallic" ), SkinBaseMetallic )  );
	FloatMap.Add( TTuple<FName, float>( TEXT( "SkinBaseRoughness" ), SkinBaseRoughness ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "SkinBaseSpecular" ), SkinBaseSpecular ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "SkinSubsurfaceOpacity" ), SkinSubsurfaceOpacity ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "NailsRoughness" ), NailsRoughness ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "NailsSpecular" ), NailsSpecular ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "FaceMascaraColorCurve" ), (float)FaceMascaraColorCurve));
	FloatMap.Add( TTuple<FName, float>( TEXT( "FaceEyelashColorCurve" ), (float)FaceEyelashColorCurve ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "FaceEyebrowColorCurve" ), (float)FaceEyebrowColorCurve ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "FaceEyebrowAndEyelashRoughness" ), FaceEyebrowAndEyelashRoughness ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "FaceInsideMouthAndTongueRoughness" ), FaceInsideMouthAndTongueRoughness ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "FaceEyebrowAndEyelashSpecular" ), FaceEyebrowAndEyelashSpecular ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "FaceInsideMouthAndTongueSpecular" ), FaceInsideMouthAndTongueSpecular ) );

	return FloatMap;
}

TMap<FName, FVector4> FSkinFaceData::GetSkinVectorTMap()
{
	TMap<FName, FVector4> FVector4Map;

	FVector4Map.Add(TTuple<FName, FVector4>(TEXT("SkinSubsurfaceColorTint"), SkinSubsurfaceColorTint) );
	FVector4Map.Add(TTuple<FName, FVector4>(TEXT("NailsColorHandLeft"), NailsColorHandLeft) );
	FVector4Map.Add(TTuple<FName, FVector4>(TEXT("NailsColorHandRight"), NailsColorHandRight) );
	FVector4Map.Add(TTuple<FName, FVector4>(TEXT("NailsColorFootLeft"), NailsColorFootLeft) );
	FVector4Map.Add(TTuple<FName, FVector4>(TEXT("NailsColorFootRight"), NailsColorFootRight) );
	FVector4Map.Add(TTuple<FName, FVector4>(TEXT("FaceCheekColor"), FaceCheekColor) );
	FVector4Map.Add(TTuple<FName, FVector4>(TEXT("FaceLipsColor"), FaceLipsColor) );
	FVector4Map.Add(TTuple<FName, FVector4>(TEXT("FaceTongueColor"), FaceTongueColor) );
	FVector4Map.Add(TTuple<FName, FVector4>(TEXT("FaceInsideMouthColor"), FaceInsideMouthColor) );
	FVector4Map.Add(TTuple<FName, FVector4>(TEXT("FaceTeethColor"), FaceTeethColor) );
	FVector4Map.Add(TTuple<FName, FVector4>(TEXT("FaceEffectYandereColor"), FaceEffectYandereColor) );
	FVector4Map.Add(TTuple<FName, FVector4>(TEXT("FaceEffectMadColor"), FaceEffectMadColor) );

	return FVector4Map;
}

TMap<FName, float> FHumanBodyMorphology::GetBodyMorphologyTMap()
{
	TMap<FName, float> FloatMap;

	FloatMap.Add( TTuple<FName, float>( TEXT( "MorphBodyFat" ), BodyFat ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "MorphBodyMuscle" ), BodyMuscle ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "MorphBoobsSize" ), Boobs ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "MorphArmsMuscle" ), ArmsMuscles ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "MorphLegsFat" ), LegsFat ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "MorphLegsMuscle" ), LegsMuscle ) );

	return FloatMap;
}

TMap<FName, float> USkeletalMeshData::GetRainFloatTMap()
{
	TMap<FName, float> FloatMap;

	FloatMap.Add( TTuple<FName, float>( TEXT( "RainDropsSize" ), RainDropsSize ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "RainDropsTopMask" ), RainDropsTopMask ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "RainDropsSpeed" ), RainDropsSpeed ) );

	return FloatMap;
}

TMap<FName, float> FHairData::GetHairTMap()
{
	TMap<FName, float> FloatMap;

	FloatMap.Add( TTuple<FName, float>( TEXT( "PrimaryColorCurve1" ), (float)PrimaryColorCurve1) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "PrimaryColorCurve2" ), (float)PrimaryColorCurve2) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "PrimaryColorCurve3" ), (float)PrimaryColorCurve3) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "PrimaryBackColorCurve1" ), (float)PrimaryBackColorCurve1) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "PrimaryBackColorCurve2" ), (float)PrimaryBackColorCurve2) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "PrimaryBackColorCurve3" ), (float)PrimaryBackColorCurve3) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicColorCurve1" ), (float)AnisotropicColorCurve1) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicColorCurve2" ), (float)AnisotropicColorCurve2) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicColorCurve3" ), (float)AnisotropicColorCurve3) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicBackColorCurve1" ), (float)AnisotropicBackColorCurve1) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicBackColorCurve2" ), (float)AnisotropicBackColorCurve2) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicBackColorCurve3" ), (float)AnisotropicBackColorCurve3) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "SubsurfacePrimaryColorBoost" ), SubsurfacePrimaryColorBoost) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "SubsurfaceAnisotropicColorBoost" ), SubsurfaceAnisotropicColorBoost) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "BaseMetallic" ), BaseMetallic) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "RoughnessMax" ), RoughnessMax) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "SpecularMin" ), SpecularMin) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "MinSubsurfaceOpacity" ), MinSubsurfaceOpacity) );

	return FloatMap;
}

TMap<FName, float> UHairMeshData::GetHairMeshFloatTMap()
{
	TMap<FName, float> FloatMap;

	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicStrength" ), AnisotropicStrength) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicHFRange" ), AnisotropicHFRange) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicHFOffset" ), AnisotropicHFOffset) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicLFRange" ), AnisotropicLFRange) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicLFOffset" ), AnisotropicLFOffset) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicBreakTextureScaleX" ), AnisotropicBreakTextureScaleX) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicBreakTextureScaleY" ), AnisotropicBreakTextureScaleY) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicBreakTextureContrast" ), AnisotropicBreakTextureContrast) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicHFCurve" ), (float)AnisotropicHFCurve) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "AnisotropicLFCurve" ), (float)AnisotropicLFCurve) );

	return FloatMap;
}

TMap<FName, FVector4> UHairMeshData::GetHairMeshVector4TMap()
{
	TMap<FName, FVector4> FVectorMap;

	FVectorMap.Add( TTuple<FName, FVector4>( TEXT( "RainVariationTextureScaleX" ), RainVariationTextureScaleX ) );
	FVectorMap.Add( TTuple<FName, FVector4>( TEXT( "RainVariationTextureScaleY" ), RainVariationTextureScaleY ) );
	FVectorMap.Add( TTuple<FName, FVector4>( TEXT( "AnisotropicTextureScale" ), AnisotropicTextureScale ) );

	return FVectorMap;
}

TMap<FName, float> FClothData::GetClothFloatTMap()
{
	TMap<FName, float> FloatMap;

	FloatMap.Add( TTuple<FName, float>( TEXT( "ClothDetailTexture1Opacity" ), ClothDetailTexture1Opacity ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "ClothDetailTexture1Scale" ), ClothDetailTexture1Scale ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "ClothColor1" ), (float)ClothColor1 ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "ClothDetailTexture2Opacity" ), ClothDetailTexture2Opacity ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "ClothDetailTexture2Scale" ), ClothDetailTexture2Scale ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "ClothColor2" ), (float)ClothColor2 ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "ClothDetailTexture3Opacity" ), ClothDetailTexture3Opacity ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "ClothDetailTexture3Scale" ), ClothDetailTexture3Scale ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "ClothColor3" ), (float)ClothColor3 ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "ClothDetailTexture4Opacity" ), ClothDetailTexture4Opacity ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "ClothDetailTexture4Scale" ), ClothDetailTexture4Scale ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "ClothColor4" ), (float)ClothColor4 ) );

	return FloatMap;
}

TMap<FName, FVector4> FClothData::GetClothVector4TMap()
{
	TMap<FName, FVector4> FVectorMap;

	FVectorMap.Add( TTuple<FName, FVector4>( TEXT( "ClothMaterialBaseProperties1" ), ClothMaterialBaseProperties1 ) );
	FVectorMap.Add( TTuple<FName, FVector4>( TEXT( "ClothMaterialBaseProperties2" ), ClothMaterialBaseProperties2 ) );
	FVectorMap.Add( TTuple<FName, FVector4>( TEXT( "ClothMaterialBaseProperties3" ), ClothMaterialBaseProperties3 ) );
	FVectorMap.Add( TTuple<FName, FVector4>( TEXT( "ClothMaterialBaseProperties4" ), ClothMaterialBaseProperties4 ) );

	return FVectorMap;
}

void FModularSkeletalMeshData::CopyPropertiesFromParent( FModularSkeletalMeshData& ParentData, bool bIsParentSetup )
{
	UClothMeshData* ClothMeshData = Cast<UClothMeshData>( ParentData.MeshData );

	if( !ClothMeshData || !MeshData) return;

	EBodyPartType MeshType = MeshData->MeshType;
	FChildBodyPart* ChildMeshData = ClothMeshData->ChildBodyParts.FindByPredicate([MeshType](const FChildBodyPart& ChildBody){ return ChildBody.MeshType == MeshType; });
	if( ChildMeshData )
	{
		switch( ChildMeshData->ClothPropertiesIndex )
		{
			case 1:
			if( bIsParentSetup )
			{
				CurrentClothData.ClothDetailTexture1Opacity = ParentData.CurrentClothData.ClothDetailTexture1Opacity;
				CurrentClothData.ClothDetailTexture1Scale = ParentData.CurrentClothData.ClothDetailTexture1Scale;
				CurrentClothData.ClothMaterialBaseProperties1 = ParentData.CurrentClothData.ClothMaterialBaseProperties1;
				CurrentClothData.ClothColor1 = ParentData.CurrentClothData.ClothColor1;
			}
			else
			{
				CurrentClothData.ClothDetailTexture1Opacity = ClothMeshData->DefaultClothData.ClothDetailTexture1Opacity;
				CurrentClothData.ClothDetailTexture1Scale = ClothMeshData->DefaultClothData.ClothDetailTexture1Scale;
				CurrentClothData.ClothMaterialBaseProperties1 = ClothMeshData->DefaultClothData.ClothMaterialBaseProperties1;
				CurrentClothData.ClothColor1 = ClothMeshData->DefaultClothData.ClothColor1;
			}
			break;
			case 2:
			if( bIsParentSetup )
			{
				CurrentClothData.ClothDetailTexture2Opacity = ParentData.CurrentClothData.ClothDetailTexture2Opacity;
				CurrentClothData.ClothDetailTexture2Scale = ParentData.CurrentClothData.ClothDetailTexture2Scale;
				CurrentClothData.ClothMaterialBaseProperties2 = ParentData.CurrentClothData.ClothMaterialBaseProperties2;
				CurrentClothData.ClothColor2 = ParentData.CurrentClothData.ClothColor2;
			}
			else
			{
				CurrentClothData.ClothDetailTexture2Opacity = ClothMeshData->DefaultClothData.ClothDetailTexture2Opacity;
				CurrentClothData.ClothDetailTexture2Scale = ClothMeshData->DefaultClothData.ClothDetailTexture2Scale;
				CurrentClothData.ClothMaterialBaseProperties2 = ClothMeshData->DefaultClothData.ClothMaterialBaseProperties2;
				CurrentClothData.ClothColor2 = ClothMeshData->DefaultClothData.ClothColor2;
			}
			break;
			case 3:
			if( bIsParentSetup )
			{
				CurrentClothData.ClothDetailTexture3Opacity = ParentData.CurrentClothData.ClothDetailTexture3Opacity;
				CurrentClothData.ClothDetailTexture3Scale = ParentData.CurrentClothData.ClothDetailTexture3Scale;
				CurrentClothData.ClothMaterialBaseProperties3 = ParentData.CurrentClothData.ClothMaterialBaseProperties3;
				CurrentClothData.ClothColor3 = ParentData.CurrentClothData.ClothColor3;
			}
			else
			{
				CurrentClothData.ClothDetailTexture3Opacity = ClothMeshData->DefaultClothData.ClothDetailTexture3Opacity;
				CurrentClothData.ClothDetailTexture3Scale = ClothMeshData->DefaultClothData.ClothDetailTexture3Scale;
				CurrentClothData.ClothMaterialBaseProperties3 = ClothMeshData->DefaultClothData.ClothMaterialBaseProperties3;
				CurrentClothData.ClothColor3 = ClothMeshData->DefaultClothData.ClothColor3;
			}
			break;
			case 4:
			if( bIsParentSetup )
			{
				CurrentClothData.ClothDetailTexture4Opacity = ParentData.CurrentClothData.ClothDetailTexture4Opacity;
				CurrentClothData.ClothDetailTexture4Scale = ParentData.CurrentClothData.ClothDetailTexture4Scale;
				CurrentClothData.ClothMaterialBaseProperties4 = ParentData.CurrentClothData.ClothMaterialBaseProperties4;
				CurrentClothData.ClothColor4 = ParentData.CurrentClothData.ClothColor4;
			}
			else
			{
				CurrentClothData.ClothDetailTexture4Opacity = ClothMeshData->DefaultClothData.ClothDetailTexture4Opacity;
				CurrentClothData.ClothDetailTexture4Scale = ClothMeshData->DefaultClothData.ClothDetailTexture4Scale;
				CurrentClothData.ClothMaterialBaseProperties4 = ClothMeshData->DefaultClothData.ClothMaterialBaseProperties4;
				CurrentClothData.ClothColor4 = ClothMeshData->DefaultClothData.ClothColor4;
			}
			break;
			default:
			return;
		}
	}
}

EBodyPartType FHumanBodyData::GetNextBodyPartToLoad( EBodyPartType CurrentBodyPart )
{
	switch( CurrentBodyPart )
	{
		case EBodyPartType::MainBody:
		return EBodyPartType::Head;
		case EBodyPartType::Head:
		return EBodyPartType::Hair;
		case EBodyPartType::Hair:
		return EBodyPartType::Torso;
		case EBodyPartType::Torso:
		return EBodyPartType::Arms;
		case EBodyPartType::Arms:
		return EBodyPartType::Hands;
		case EBodyPartType::Hands:
		return EBodyPartType::Legs;
		case EBodyPartType::Legs:
		return EBodyPartType::Feet;
		case EBodyPartType::Feet:
		return EBodyPartType::BodyMAX;
		default:
		return EBodyPartType::BodyMAX;
	}
}
