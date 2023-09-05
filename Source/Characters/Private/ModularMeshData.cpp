// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularMeshData.h"

DEFINE_LOG_CATEGORY_STATIC( LogCharacter, Log, All );

float FEyeCustomData::GetCustomDataValue( int32 EyeIndex, int32 CustomDataIndex )
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
		case 19:
		return ( HighlightMaxRotation.IsValidIndex( EyeIndex ) ) ? HighlightMaxRotation[EyeIndex] : 0;
		case 20:
		return ( HighlightMaxScale.IsValidIndex( EyeIndex ) ) ? HighlightMaxScale[EyeIndex].X : 0;
		case 21:
		return ( HighlightMaxScale.IsValidIndex( EyeIndex ) ) ? HighlightMaxScale[EyeIndex].Y : 0;
		default:
		return 0;
	}
}

float FSkinFaceCustomData::GetSkinCustomDataValue( int32 CustomDataIndex )
{
	switch( CustomDataIndex )
	{
		case 10:
			return SkinAtlasCurve;
		case 11:
			return BaseSpecular;
		case 12:
			return BaseRoughness;
		case 13:
			return BaseMetallic;
		case 14:
			return BaseSubsurfaceOpacity;
		case 15:
			return SubsurfaceColor.R;
		case 16:
			return SubsurfaceColor.G;
		case 17:
			return SubsurfaceColor.B;
		default:
		return 0;
	}
}

float FSkinFaceCustomData::GetBodyCustomDataValue( int32 CustomDataIndex )
{
	switch( CustomDataIndex )
	{
		case 18:
		return HandLeftNailColorCurve;
		case 19:
		return HandRightNailColorCurve;
		case 20:
		return FootLeftNailColorCurve;
		case 21:
		return FootRightNailColorCurve;
		default:
		return 0;
	}
}

float FSkinFaceCustomData::GetFaceCustomDataValue( int32 CustomDataIndex )
{
	switch( CustomDataIndex )
	{
		case 18:
		return EyelashColorCurve;
		case 19:
		return EyebrowColorCurve;
		case 20:
		return InsideMouthCurve;
		case 21:
		return TongueCurve;
		case 22:
		return CheekColor.R;
		case 23:
		return CheekColor.G;
		case 24:
		return CheekColor.B;
		case 25:
		return LipsColor.R;
		case 26:
		return LipsColor.G;
		case 27:
		return LipsColor.B;
		case 28:
		return MascaraColorCurve;
		case 29:
		return Moles;
		default:
		return 0;
	}
}

TArray<float> FSkinFaceCustomData::CombineSkinData()
{
	TArray<float> ValueArray;

	for( uint32 DataIndex = StartingIndexSkin; DataIndex <= EndIndexSkin; DataIndex++ )
	{
		ValueArray.Add( GetSkinCustomDataValue( DataIndex ) );
	}

	return ValueArray;
}

TArray<float> FSkinFaceCustomData::CombineBodyData()
{
	TArray<float> ValueArray;

	for( uint32 DataIndex = StartingIndexSkin; DataIndex <= EndingIndexBody; DataIndex++ )
	{
		if( DataIndex <= EndIndexSkin )
		{
			ValueArray.Add( GetSkinCustomDataValue( DataIndex ) );
		}
		else
		{
			ValueArray.Add( GetBodyCustomDataValue( DataIndex ) );
		}
	}

	return ValueArray;
}

TArray<float> FSkinFaceCustomData::CombineFaceData()
{
	TArray<float> ValueArray;

	for( uint32 DataIndex = StartingIndexSkin; DataIndex <= EndingIndexFace; DataIndex++ )
	{
		if( DataIndex <= EndIndexSkin )
		{
			ValueArray.Add( GetSkinCustomDataValue( DataIndex ) );
		}
		else
		{
			ValueArray.Add( GetFaceCustomDataValue( DataIndex ) );
		}
	}

	return ValueArray;
}

bool FHumanBodyData::IsCustomData() const
{
	return PresetName == "Custom";
}

FModularSkeletalMeshData FHumanBodyData::GetHeadData()
{
	for( FModularSkeletalMeshData& SkeletalMeshData : SkeletalMeshes )
	{
		if( Cast<UHeadMeshData>( SkeletalMeshData.MeshData )) return SkeletalMeshData;
	}

	return FModularSkeletalMeshData();
}

FClothCustomData::FClothCustomData()
{
	// https://benui.ca/unreal/uproperty/#editfixedsize
	ClothColorCurve = {0, 0, 1, 1};
	ClothBaseMetallic = { 0.f, 0.f, 0.f, 0.f };
	ClothBaseRoughness = { 0.f, 0.f, 0.f, 0.f };
	ClothBaseSpecular = { 0.f, 0.f, 0.f, 0.f };
}

float FClothCustomData::GetFloatCustomDataValue( int32 CustomDataIndex )
{
	switch( CustomDataIndex )
	{
		case 10:
		return ( ClothColorCurve.IsValidIndex( 0 ) ) ? ClothColorCurve[0] : 0;
		case 11:
		return ( ClothBaseSpecular.IsValidIndex( 0 ) ) ? ClothBaseSpecular[0] : 0;
		case 12:
		return ( ClothBaseRoughness.IsValidIndex( 0 ) ) ? ClothBaseRoughness[0] : 0;
		case 13:
		return ( ClothBaseMetallic.IsValidIndex( 0 ) ) ? ClothBaseMetallic[0] : 0;
		case 18:
		return ( ClothColorCurve.IsValidIndex( 1 ) ) ? ClothColorCurve[1] : 0;
		case 19:
		return ( ClothColorCurve.IsValidIndex( 2 ) ) ? ClothColorCurve[2] : 0;
		case 20:
		return ( ClothColorCurve.IsValidIndex( 3 ) ) ? ClothColorCurve[3] : 0;
		case 21:
		return ( ClothBaseSpecular.IsValidIndex( 1 ) ) ? ClothBaseSpecular[1] : 0;
		case 22:
		return ( ClothBaseRoughness.IsValidIndex( 1 ) ) ? ClothBaseRoughness[1] : 0;
		case 23:
		return ( ClothBaseMetallic.IsValidIndex( 1 ) ) ? ClothBaseMetallic[1] : 0;
		case 24:
		return ( ClothBaseSpecular.IsValidIndex( 2 ) ) ? ClothBaseSpecular[2] : 0;
		case 25:
		return ( ClothBaseRoughness.IsValidIndex( 2 ) ) ? ClothBaseRoughness[2] : 0;
		case 26:
		return ( ClothBaseMetallic.IsValidIndex( 2 ) ) ? ClothBaseMetallic[2] : 0;
		case 27:
		return ( ClothBaseSpecular.IsValidIndex( 3 ) ) ? ClothBaseSpecular[3] : 0;
		case 28:
		return ( ClothBaseRoughness.IsValidIndex( 3 ) ) ? ClothBaseRoughness[3] : 0;
		case 29:
		return ( ClothBaseMetallic.IsValidIndex( 3 ) ) ? ClothBaseMetallic[3] : 0;
		default:
		return 0;
	}
}

TArray<float> FClothCustomData::CombineClothValues( int32 EndIndexSkin )
{
	TArray<float> ValueArray;

	for( uint32 DataIndex = EndIndexSkin+1; DataIndex <= EndingIndexCloth; DataIndex++ )
	{
		ValueArray.Add( GetFloatCustomDataValue( DataIndex ) );
	}

	return ValueArray;
}
