// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularMeshData.h"

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
		case 9:
			return SkinAtlasCurve;
		case 10:
			return BaseSpecular;
		case 11:
			return BaseRoughness;
		case 12:
			return BaseMetallic;
		case 13:
			return BaseSubsurfaceOpacity;
		case 14:
			return SubsurfaceColor.R;
		case 15:
			return SubsurfaceColor.G;
		case 16:
			return SubsurfaceColor.B;
		default:
		return 0;
	}
}

float FSkinFaceCustomData::GetBodyCustomDataValue( int32 CustomDataIndex )
{
	switch( CustomDataIndex )
	{
		case 17:
		return HandNailsColor.R;
		case 18:
		return HandNailsColor.G;
		case 19:
		return HandNailsColor.B;
		case 20:
		return FeetNailsColor.R;
		case 21:
		return FeetNailsColor.G;
		case 22:
		return FeetNailsColor.B;
		default:
		return 0;
	}
}

float FSkinFaceCustomData::GetFaceCustomDataValue( int32 CustomDataIndex )
{
	switch( CustomDataIndex )
	{
		case 17:
		return EyelashColorCurve;
		case 18:
		return EyebrowColorCurve;
		case 19:
		return InsideMouthColor.R;
		case 20:
		return InsideMouthColor.G;
		case 21:
		return InsideMouthColor.B;
		case 22:
		return CheekColor.R;
		case 23:
		return CheekColor.G;
		case 24:
		return CheekColor.B;
		case 25:
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