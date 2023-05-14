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
		return InsideMouthColor.R;
		case 21:
		return InsideMouthColor.G;
		case 22:
		return InsideMouthColor.B;
		case 23:
		return CheekColor.R;
		case 24:
		return CheekColor.G;
		case 25:
		return CheekColor.B;
		case 26:
		return LipsColor.R;
		case 27:
		return LipsColor.G;
		case 28:
		return LipsColor.B;
		case 29:
		return MascaraColorCurve;
		default:
		return 0;
	}
}

TArray<float> FSkinFaceCustomData::CombineBodyData()
{
	TArray<float> ValueArray;
	uint32 EndIndexSkin = StartingIndexSkin + 7;

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
	uint32 EndIndexSkin = StartingIndexSkin + 7;

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