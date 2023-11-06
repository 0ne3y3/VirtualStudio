// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HumanAnimationData.generated.h"

/**
* Enum for part of the body
*/
UENUM( BlueprintType )
enum class EControlType : uint8
{
	OneShot, // Driving 1 curve in a [0-1] range
	TwoShot, // Driving 2 oposites curves [0-0.5] is first curve, [0.5-1] is the second
	Position // 2D vector
};

/**
* FCRFaceData
*
* Structure containing datas for the control rig used in sequencer for animating face.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FCRFaceData
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Control" )
	FName ControlName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Control" )
	EControlType ControlType = EControlType::OneShot;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Control" )
	TArray<FName> CurveDriven;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Control" )
	FName CurveConstraint;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Control" )
	bool bIsOffset = false;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "View" )
	float ControlMinMax = 0.75f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "View" )
	FTransform InitialPosition;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "View" )
	FLinearColor Color;
};

/**
* UCRFacePresetData
*
* Mutable data asset that contains all CR control data. Create preset for the face control rig data. /!\ Do not change values at runtime.
*/
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "Face control rig data", ShortTooltip = "Data asset used for the face control rig." ) )
class CHARACTERS_API UCRFacePresetData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
	FName PresetName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Data", meta = ( TitleProperty = "ControlName" ) )
	TArray<FCRFaceData> CurvesData;
};

/**
* FARKitCurveData
*
* Structure containing iphone blendshape data.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FARKitCurveData
{
	GENERATED_USTRUCT_BODY()

	public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="General")
	FName CurveName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float ThresholdVibration = 0.05;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Range", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float RangeMin = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Range", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float RangeMax = 1.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Range", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float ClampMin = 0;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Range", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float ClampMax = 1.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Smooth", meta = ( ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100 ) )
	float SmoothIncrease = 20.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Smooth", meta = ( ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100 ) )
	float SmoothDecrease = 20.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "General" )
	TArray<FName> AdditionnalCurveDriven;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "General", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float LastFrameValue = 0;
};

/**
* FHumanStateData
*
* Structure containing common dynamic data for a human state, like is crying \ eyes bleeding \ is wet \ is under a roof for rain and such.
*/
USTRUCT( BlueprintType )
struct CHARACTERS_API FHumanStateData
{
	GENERATED_USTRUCT_BODY()

	FHumanStateData()
	{
		GlobalSphereMaskPosition = FVector4::Zero();
	};

	public:

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Face", meta = ( ClampMin = 0, UIMin = 0 ) )
	float FaceEffectIndex = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes", meta = ( ClampMin = 0, ClampMax = 2, UIMin = 0, UIMax = 2 ) )
	TArray<float> EyesEmissive;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	TArray<float> EyesBleeding;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes", meta = ( ClampMin = 0, ClampMax = 2, UIMin = 0, UIMax = 2 ) )
	TArray<float> PupilScale;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Eyes", meta = ( ClampMin = 0, ClampMax = 2, UIMin = 0, UIMax = 2 ) )
	TArray<float> HighlightStrength;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Eyes" )
	TArray<float> LastFrameEyeBlink;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rain" )
	bool bIsUnderRoof = false;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Wetness", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float WetnessGlobalOpacity = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Wetness", meta = ( ClampMin = 0, UIMin = 0 ) )
	float WetnessZHeight = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Body", meta = ( ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float WetnessZOpacity = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mask", meta = ( ClampMin = 0, UIMin = 0 ) )
	float GlobalMaskHeight = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mask", meta = ( ClampMin = 0, UIMin = 0 ) )
	float GlobalSphereMaskHardness = 0.75f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mask" )
	FVector4 GlobalSphereMaskPosition;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mask", meta = ( ClampMin = 0, UIMin = 0 ) )
	float GlobalSphereMaskRadius = 0.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mask" )
	bool bReverseGlobalSphereMask = false;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mask" )
	bool bReverseGlobalHeightMask = false;

	TMap<FName, float> GetHumanStateFloatTMap();
	TMap<FName, FVector4> GetHumanStateVectorTMap();
};

/**
* UARKitPresetData
*
* Mutable data asset that contains all ARKit curves data. Create preset for runtime control rig. /!\ Do not change values at runtime.
*/
UCLASS( BlueprintType, Const, Meta = ( DisplayName = "ARKit curves data", ShortTooltip = "Data asset used for runtime ARKit control rig." ) )
class CHARACTERS_API UARKitPresetData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "General" )
	FName PresetName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Data" )
	UCurveFloat* HeadRotationWeight;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Data" )
	float TongueGoesUpWeight = 0.5f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Data", meta = ( TitleProperty = "CurveName" ) )
	TArray<FARKitCurveData> CurvesData;
};