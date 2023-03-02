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


void UHumanBodyData::CopyDataFromInterface( FHumanBodyInterface& HumanBodyInterface )
{
	PresetName = HumanBodyInterface.PresetName;
	BodyMorphology = HumanBodyInterface.BodyMorphology;
	BodyMeshes = HumanBodyInterface.BodyMeshes;
	BodyAccessories = HumanBodyInterface.BodyAccessories;
	SkinAndFaceCustomData = HumanBodyInterface.SkinAndFaceCustomData;
	HairCustomData = HumanBodyInterface.HairCustomData;
	EyeCustomData = HumanBodyInterface.EyeCustomData;
	ClothCustomData = HumanBodyInterface.ClothCustomData;

	bIsPreset = false;
}

void UHumanBodyData::CopyDataFromAnotherDataAsset( UHumanBodyData* InHumanBodyData )
{
	if( !InHumanBodyData ) return;

	PresetName = InHumanBodyData->PresetName;
	BodyMorphology = InHumanBodyData->BodyMorphology;
	BodyMeshes = InHumanBodyData->BodyMeshes;
	BodyAccessories = InHumanBodyData->BodyAccessories;
	SkinAndFaceCustomData = InHumanBodyData->SkinAndFaceCustomData;
	HairCustomData = InHumanBodyData->HairCustomData;
	EyeCustomData = InHumanBodyData->EyeCustomData;
	ClothCustomData = InHumanBodyData->ClothCustomData;

	bIsPreset = false;
}

void UHumanBodyData::CopyDataToInterface( FHumanBodyInterface& HumanBodyInterface )
{
	HumanBodyInterface.PresetName = PresetName;
	HumanBodyInterface.BodyMorphology = BodyMorphology;
	HumanBodyInterface.BodyMeshes = BodyMeshes;
	HumanBodyInterface.BodyAccessories = BodyAccessories;
	HumanBodyInterface.SkinAndFaceCustomData = SkinAndFaceCustomData;
	HumanBodyInterface.HairCustomData = HairCustomData;
	HumanBodyInterface.EyeCustomData = EyeCustomData;
	HumanBodyInterface.ClothCustomData = ClothCustomData;
}

FBodyMesh* UHumanBodyData::GetMainBody()
{
	return BodyMeshes.FindByPredicate( []( const FBodyMesh& InBodyMesh ){ return InBodyMesh.MeshData->MeshType == EBodyPartType::MainBody; });
}