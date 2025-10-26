// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HumanAnimationData.generated.h"

// ############################## CONTROL RIG ANIMATION (EDITOR) ##############################

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

	FCRFaceData();

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

// ############################## DYNAMIC MATERIAL EFFECT  ##############################

/**
* FHumanStateData
*
* Structure containing common dynamic data for a human state, like is crying \ eyes bleeding \ is wet \ is under a roof for rain and such.
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FHumanStateData
{
	GENERATED_USTRUCT_BODY()

	FHumanStateData()
	{
		GlobalSphereMaskPosition = FVector4::Zero();
	};

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face", meta = (ClampMin = 0, UIMin = 0))
	float FaceEffectIndex = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes", meta = (ClampMin = 0, ClampMax = 2, UIMin = 0, UIMax = 2))
	TArray<float> EyesEmissive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes", meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1))
	TArray<float> EyesBleeding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes", meta = (ClampMin = 0, ClampMax = 2, UIMin = 0, UIMax = 2))
	TArray<float> PupilScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes", meta = (ClampMin = 0, ClampMax = 2, UIMin = 0, UIMax = 2))
	TArray<float> HighlightStrength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Eyes")
	TArray<float> LastFrameEyeBlink;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rain")
	bool bIsUnderRoof = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wetness", meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1))
	float WetnessGlobalOpacity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wetness", meta = (ClampMin = 0, UIMin = 0))
	float WetnessZHeight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body", meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1))
	float WetnessZOpacity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask", meta = (ClampMin = 0, UIMin = 0))
	float GlobalMaskHeight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask", meta = (ClampMin = 0, UIMin = 0))
	float GlobalSphereMaskHardness = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask")
	FVector4 GlobalSphereMaskPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask", meta = (ClampMin = 0, UIMin = 0))
	float GlobalSphereMaskRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask")
	bool bReverseGlobalSphereMask = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask")
	bool bReverseGlobalHeightMask = false;

	TMap<FName, float> GetHumanStateFloatTMap();
	TMap<FName, FVector4> GetHumanStateVectorTMap();
};

// ############################## ANIMATION POST PROCESS ASSETS ##############################

/**
* UPostProcessBodyPreset
*
* Mutable data asset that contains all animation post process data for the body.
*/
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Post process data preset", ShortTooltip = "Asset that contains all animation post process data for the body."))
class CHARACTERS_API UPostProcessBodyPreset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPostProcessBodyPreset();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName PresetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. ControlRig Twist Correction|Arms")
	float UpperarmCorrectiveRootWeight = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. ControlRig Twist Correction|Arms")
	float UpperarmTwist1Weight = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. ControlRig Twist Correction|Arms")
	float UpperarmTwist2Weight = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. ControlRig Twist Correction|Arms")
	float LowerarmCorrectiveRootWeight = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. ControlRig Twist Correction|Arms")
	float LowerarmTwist1Weight = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. ControlRig Twist Correction|Arms")
	float LowerarmTwist2Weight = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. ControlRig Twist Correction|Legs")
	float ThighCorrectiveRootWeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. ControlRig Twist Correction|Legs")
	float ThighTwistWeight = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. ControlRig Twist Correction|Legs")
	float CalfCorrectiveRootWeight = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. ControlRig Twist Correction|Legs")
	float CalfTwistWeight = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Arms")
	TObjectPtr<UPoseAsset> PoseAssetClavicleL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Arms")
	TObjectPtr<UPoseAsset> PoseAssetClavicleR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Arms")
	TObjectPtr<UPoseAsset> PoseAssetUpperarmL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Arms")
	TObjectPtr<UPoseAsset> PoseAssetUpperarmR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Arms")
	TObjectPtr<UPoseAsset> PoseAssetLowerarmL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Arms")
	TObjectPtr<UPoseAsset> PoseAssetLowerarmR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Arms")
	TObjectPtr<UPoseAsset> PoseAssetHandL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Arms")
	TObjectPtr<UPoseAsset> PoseAssetHandR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Legs")
	TObjectPtr<UPoseAsset> PoseAssetThighL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Legs")
	TObjectPtr<UPoseAsset> PoseAssetThighR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Legs")
	TObjectPtr<UPoseAsset> PoseAssetCalfL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Legs")
	TObjectPtr<UPoseAsset> PoseAssetCalfR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Legs")
	TObjectPtr<UPoseAsset> PoseAssetFeetL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. PoseAsset|Legs")
	TObjectPtr<UPoseAsset> PoseAssetFeetR;
};

// ############################## SIMPLE POSE ASSETS ##############################

/**
* Enum for pose asset type
*/
UENUM( BlueprintType )
enum class EPoseCurveType : uint8
{
	Eyes,
	Mouth
};

/**
* FPoseCurve
*
* Structure containing all pose asset information
*/
USTRUCT()
struct CHARACTERS_API FPoseAssetCurve
{
	GENERATED_USTRUCT_BODY()

public:
	EPoseCurveType PoseCurveType = EPoseCurveType::Eyes;
	FName PoseCurveName;
	float PoseCurveValue = 0.f;
};

/**
* FARKitPreProcessorData
*
* Structure to process ARKit (or metahuman) curves. Like JawOpen | MouthSmileLeft | EyeCloseRight | etc...
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FARKitPreProcessorData
{
	GENERATED_USTRUCT_BODY()

public:
	// ARKit curve (or metahuman) you want to evaluate (JawOpen, MouthSmileLeft, EyeCloseRight, etc...)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> CurvesToEvaluate;

	// Curve to evaluate
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCurveFloat> Curve;
};

/**
* UARkitPreProcessorPreset
*
* Mutable data asset that contains all ARKit preprocessor.
*/
UCLASS(BlueprintType, Const, Meta = (DisplayName = "ARKit preprocessor preset", ShortTooltip = "Asset that contains all ARKit preprocessor."))
class CHARACTERS_API UARkitPreProcessorPreset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UARkitPreProcessorPreset();

	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName PresetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty = "CurvesToEvaluate - Type"))
	TArray<FARKitPreProcessorData> Preprocessors;
};

/**
* FGoalPoseData
*
* Structure that contain the data for the goal pose we want to reach. 
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FGoalPoseData
{
	GENERATED_USTRUCT_BODY()

public:

	// Goal pose curve name we want to reach.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName GoalCurveName;

	// How fast we transition to the goal pose.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TransitionSpeed = 1.f;

	// ARKit curve (or metahuman) you want to evaluate (JawOpen, MouthSmileLeft, EyeCloseRight, etc...) and their goal value.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, float> CurvesToEvaluate;

};

/**
* UGoalPosePreset
*
* Mutable data asset that contains all the goal pose.
*/
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Goal pose preset", ShortTooltip = "Asset that contains all the goal pose."))
class CHARACTERS_API UGoalPosePreset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UGoalPosePreset();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName PresetName;

	// The animation sequence asset containing goal curve
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPoseAsset> PoseAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(TitleProperty="GoalCurveName"))
	TArray<FGoalPoseData> GoalPoses;
};

/**
* FSimpleARKitToFaceCurvesData
*
* Structure to process ARKit (or metahuman) curves. Like JawOpen | MouthSmileLeft | EyeCloseRight | etc...
*/
USTRUCT(BlueprintType)
struct CHARACTERS_API FSimpleARKitToFaceCurvesData
{
	GENERATED_USTRUCT_BODY()

public:
	// Target pose curve name (FC_PupilLookUpLeft, FC_PupilLookDownRight, etc...)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TargetName;

	// Which pose asset the TargetName is from.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPoseCurveType PoseCurveType = EPoseCurveType::Eyes;

	// ARKit curve (or metahuman) you want to evaluate (JawOpen, MouthSmileLeft, EyeCloseRight, etc...) and their goal value.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, float> CurvesToEvaluate;
};

/**
* USimpleArkitToFaceCurvesPreset
*
* Mutable data asset that contains all the goal pose.
*/
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Simple ARKit to face pose preset", ShortTooltip = "Asset that contains all the simple face pose."))
class CHARACTERS_API USimpleArkitToFaceCurvesPreset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	USimpleArkitToFaceCurvesPreset();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName PresetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty = "TargetName"))
	TArray<FSimpleARKitToFaceCurvesData> SimpleFacePoses;

	// Pose asset for the eyes curves
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPoseAsset> PoseAssetEyes;

	// Pose asset for the Mouth curves
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPoseAsset> PoseAssetMouth;
};

/**
* UARKitPosePreset
*
* Mutable data asset that contains all data for processing ARKit curves to a pose.
*/
UCLASS(BlueprintType, Const, Meta = (DisplayName = "ARKit pose preset", ShortTooltip = "Asset that contains all data for processing ARKit curves to a pose."))
class CHARACTERS_API UARKitPosePreset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UARKitPosePreset();

	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "1. General")
	FName PresetName;

	// Delay (in seconds) when the jaw follow the mouth
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Data|Jaw", meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1))
	float JawDelay;

	// How much the jaw should follow the mouth
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Data|Jaw", meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1))
	float JawWeight;
	
	// List of preprocessor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Data|Preprocessor")
	TObjectPtr<UARkitPreProcessorPreset> ARKitPreprocessorPreset;

	// List of goal for pose searching
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Data|Processor")
	TObjectPtr<UGoalPosePreset> GoalPosePreset;

	// List of simple ARkit to face pose
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Data|Processor")
	TObjectPtr<USimpleArkitToFaceCurvesPreset> SimplePosePreset;
};
