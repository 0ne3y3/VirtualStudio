// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanAnimationData.h"

TMap<FName, float> FHumanStateData::GetHumanStateFloatTMap()
{
	TMap<FName, float> FloatMap;

	FloatMap.Add( TTuple<FName, float>( TEXT( "FaceEffectIndex" ), (float)FaceEffectIndex ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "IsUnderRoof" ), ( bIsUnderRoof ) ? 1.f : 0.f ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "WetnessGlobalOpacity" ), WetnessGlobalOpacity ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "WetnessZHeight" ), WetnessZHeight ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "WetnessZOpacity" ), WetnessZOpacity ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "MaskingMaskHeight" ), GlobalMaskHeight ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "MaskingSphereMaskHardness" ), GlobalSphereMaskHardness ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "MaskingSphereMaskRadius" ), GlobalSphereMaskRadius ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "MaskingReverseSphereMask" ), ( bReverseGlobalSphereMask ) ? 1.f : 0.f ) );
	FloatMap.Add( TTuple<FName, float>( TEXT( "MaskingReverseHeightMask" ), ( bReverseGlobalSphereMask ) ? 1.f : 0.f ) );

	return FloatMap;
}

TMap<FName, FVector4> FHumanStateData::GetHumanStateVectorTMap()
{
	TMap<FName, FVector4> FVector4Map;

	FVector4Map.Add( TTuple<FName, FVector4>( TEXT( "MaskingSphereMaskPosition" ), GlobalSphereMaskPosition ) );

	return FVector4Map;
}