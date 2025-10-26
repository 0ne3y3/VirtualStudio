// Fill out your copyright notice in the Description page of Project Settings.


#include "FAnimNode_UpdateFace.h"
#include "ModularMeshData.h"
#include "AnimationRuntime.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimStats.h"
#include "Animation/AnimCurveUtils.h"
#include "CharacterSubsystem.h"

FAnimNode_UpdateFace::FAnimNode_UpdateFace()
: LODThreshold(INDEX_NONE)
{
}

void FAnimNode_UpdateFace::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Initialize_AnyThread)
	Super::Initialize_AnyThread(Context);
	SourcePose.Initialize(Context);

	LastFrameEyeBlinkLeft = 0.f;
	LastFrameEyeBlinkRight = 0.f;
	HighLightAnimationLeft = 0.f;
	HighLightAnimationRight = 0.f;
	
	InitializeCurves();
}

void FAnimNode_UpdateFace::InitializeCurves()
{
	if(Curves.Num() == 0)
	{
		Curves.Reserve(12);
		Curves.Add("EyeLookInLeft", 0);
		Curves.Add("EyeLookOutLeft", 0);
		Curves.Add("EyeLookUpLeft", 0);
		Curves.Add("EyeLookDownLeft", 0);
		Curves.Add("EyeLookInRight", 0);
		Curves.Add("EyeLookOutRight", 0);
		Curves.Add("EyeLookUpRight", 0);
		Curves.Add("EyeLookDownRight", 0);
		Curves.Add("EyeWideLeft", 0);
		Curves.Add("EyeWideRight", 0);
		Curves.Add("EyeCloseLeft", 0);
		Curves.Add("EyeCloseRight", 0);
	}
}

void FAnimNode_UpdateFace::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(CacheBones_AnyThread)
	Super::CacheBones_AnyThread(Context);
	SourcePose.CacheBones(Context);
}

void FAnimNode_UpdateFace::GetCurvesValue(FPoseContext& Output, TMap<FName, float>& OutArray)
{
	// We build an array to filter the curves we want to get value
	UE::Anim::TNamedValueArray<FDefaultAllocator, UE::Anim::FNamedIndexElement> CurveFilter;
	int32 I = 0;
	for(auto It = OutArray.CreateConstIterator(); It; ++It)
	{
		CurveFilter.Add(It.Key(), I);
		I++;
	}

	// Bulk get all the curves and export then in the CachedCurves Tmap.
	UE::Anim::FCurveUtils::BulkGet(Output.Curve, CurveFilter,
	[&OutputArray = OutArray](const UE::Anim::FNamedIndexElement& InBulkElement, float InValue)
	{
		if(OutputArray.Contains(InBulkElement.Name))
		{
			OutputArray[InBulkElement.Name] = InValue;
		}
	});
}

void FAnimNode_UpdateFace::Evaluate_AnyThread(FPoseContext& Output)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Evaluate_AnyThread)
	QUICK_SCOPE_CYCLE_COUNTER(UpdateFace);

	if (!IsLODEnabled(Output.AnimInstanceProxy) || !Head || !HeadMeshProperties)
	{
		SourcePose.Evaluate(Output);
		return;
	}

	FPoseContext SourceData(Output);
	SourcePose.Evaluate(SourceData);

	Output = SourceData;

	GetCurvesValue(Output, Curves);
	FBlendedCurve FinalArrayCurves;

	/* Iris position(in - out, up - down) : Lerp between base iris position and max iris position.
	*/
	FVector2D IrisPositionLeft = FVector2D(HeadMeshProperties->IrisDefaultPositionX, HeadMeshProperties->IrisDefaultPositionY);
	if (Curves["EyeLookInLeft"] > Curves["EyeLookOutLeft"])
	{
		IrisPositionLeft.X = FMath::Lerp(IrisPositionLeft.X, HeadMeshProperties->IrisMaxPositionInOut.X, Curves["EyeLookInLeft"]);
	}
	else
	{
		IrisPositionLeft.X = FMath::Lerp(IrisPositionLeft.X, HeadMeshProperties->IrisMaxPositionInOut.Y, Curves["EyeLookOutLeft"]);
	}
	if (Curves["EyeLookUpLeft"] > Curves["EyeLookDownLeft"])
	{
		IrisPositionLeft.Y = FMath::Lerp(IrisPositionLeft.Y, HeadMeshProperties->IrisMaxPositionUpDown.X, Curves["EyeLookUpLeft"]);
	}
	else
	{
		IrisPositionLeft.Y = FMath::Lerp(IrisPositionLeft.Y, HeadMeshProperties->IrisMaxPositionUpDown.Y, Curves["EyeLookDownLeft"]);
	}

	FVector2D IrisPositionRight = FVector2D(HeadMeshProperties->IrisDefaultPositionX, HeadMeshProperties->IrisDefaultPositionY);
	if(Curves["EyeLookInRight"] > Curves["EyeLookOutRight"])
	{
		IrisPositionRight.X = FMath::Lerp(IrisPositionRight.X, HeadMeshProperties->IrisMaxPositionInOut.X, Curves["EyeLookInRight"]);
	}
	else
	{
		IrisPositionRight.X = FMath::Lerp(IrisPositionRight.X, HeadMeshProperties->IrisMaxPositionInOut.Y, Curves["EyeLookOutRight"]);
	}
	if(Curves["EyeLookUpRight"] > Curves["EyeLookDownRight"])
	{
		IrisPositionRight.Y = FMath::Lerp(IrisPositionRight.Y, HeadMeshProperties->IrisMaxPositionUpDown.X, Curves["EyeLookUpRight"]);
	}
	else
	{
		IrisPositionRight.Y = FMath::Lerp(IrisPositionRight.Y, HeadMeshProperties->IrisMaxPositionUpDown.Y, Curves["EyeLookDownRight"]);
	}

	FinalArrayCurves.Add("MC_IrisOffsetXLeft", IrisPositionLeft.X);
	FinalArrayCurves.Add("MC_IrisOffsetYLeft", IrisPositionLeft.Y);
	FinalArrayCurves.Add("MC_IrisOffsetXRight", IrisPositionRight.X);
	FinalArrayCurves.Add("MC_IrisOffsetYRight", IrisPositionRight.Y);
	
	/* Iris scale : if eye is wide, scale down the iris for a scared/suprise effect. Move down the eyelash shadow (Sclera dark corner) when eye is closing
	*/
	float MinimumScale = Head->HeadProperties.EyesMinimumScale;

	float ScalingClampedLeft = FMath::Lerp(1.f, MinimumScale, Curves["EyeWideLeft"]);
	FinalArrayCurves.Add("MC_IrisScaleXLeft", HeadMeshProperties->EyesScaleX * ScalingClampedLeft);
	FinalArrayCurves.Add("MC_IrisScaleYLeft", HeadMeshProperties->EyesScaleY * ScalingClampedLeft);

	float ScalingClampedRight = FMath::Lerp(1.f, MinimumScale, Curves["EyeWideRight"]);
	FinalArrayCurves.Add("MC_IrisScaleXRight", HeadMeshProperties->EyesScaleX * ScalingClampedRight);
	FinalArrayCurves.Add("MC_IrisScaleYRight", HeadMeshProperties->EyesScaleY * ScalingClampedRight);

	/* Highlight strength : when eye blink, animate the eye highlight for some live2d'esque effect. Calculate some velocity and accumulate strength
	*/
	float EyelashLeftVelocity = FMath::Abs(LastFrameEyeBlinkLeft - Curves["EyeCloseLeft"]);
	float EyelashRightVelocity = FMath::Abs(LastFrameEyeBlinkRight - Curves["EyeCloseRight"]);

	if (EyelashLeftVelocity > 0.1f)
	{
		HighLightAnimationLeft = FMath::Min((HighLightAnimationLeft + EyelashLeftVelocity), 1.f);
	}
	else
	{
		HighLightAnimationLeft = FMath::Max(HighLightAnimationLeft - Output.AnimInstanceProxy->GetDeltaSeconds() * 0.60f, 0.f);
	}
	FinalArrayCurves.Add("MC_HighlightLeft", HighLightAnimationLeft);
	LastFrameEyeBlinkLeft = Curves["EyeCloseLeft"];

	if (EyelashRightVelocity > 0.1f)
	{
		HighLightAnimationRight = FMath::Min((HighLightAnimationRight + EyelashRightVelocity), 1.f);

	}
	else
	{
		HighLightAnimationRight = FMath::Max(HighLightAnimationRight - Output.AnimInstanceProxy->GetDeltaSeconds() * 0.60f, 0.f);
	}
	FinalArrayCurves.Add("MC_HighlightRight", HighLightAnimationRight);
	LastFrameEyeBlinkRight = Curves["EyeCloseRight"];

	/* Dark sclera position : the shadow below the eyelash on the sclera should move up or down if the eye close or widen.
	*/
	float DarkScleraEyeCloseWeight = HeadMeshProperties->ScleraDarkCornerEyeCloseWeight;
	float DarkScleraEyeWideWeight = HeadMeshProperties->ScleraDarkCornerEyeWideWeight;
	FinalArrayCurves.Add("MC_ScleraCornerDarkOffsetYLeft", HeadMeshProperties->ScleraDefaultDarkCornerOffsetY + Curves["EyeWideLeft"] * DarkScleraEyeWideWeight - Curves["EyeCloseLeft"] * DarkScleraEyeCloseWeight);
	FinalArrayCurves.Add("MC_ScleraCornerDarkOffsetYRight", HeadMeshProperties->ScleraDefaultDarkCornerOffsetY + Curves["EyeWideRight"] * DarkScleraEyeWideWeight - Curves["EyeCloseRight"] * DarkScleraEyeCloseWeight);
	
	UE::Anim::FNamedValueArrayUtils::Union(Output.Curve, FinalArrayCurves,
	[](UE::Anim::FCurveElement& InOutResult, const UE::Anim::FCurveElement& InCurveElement, UE::Anim::ENamedValueUnionFlags InFlags)
	{
		// Only apply curves that we are overriding
		if(EnumHasAnyFlags(InFlags, UE::Anim::ENamedValueUnionFlags::ValidArg1))
		{
			InOutResult.Value = InCurveElement.Value;
		}
	});
}

void FAnimNode_UpdateFace::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Update_AnyThread)

	// Run update on input pose nodes
	SourcePose.Update(Context);

	// Evaluate any BP logic plugged into this node
	GetEvaluateGraphExposedInputs().Execute(Context);

	UCharacterSubsystem* CharacterSubsystem = GEngine->GetEngineSubsystem<UCharacterSubsystem>();
	if(CharacterSubsystem)
	{
		if(CharacterSubsystem->MutableHeadMeshTable && Head)
		{
			static const FString ContextString(TEXT("Finding row in Human Head data table"));
			HeadMeshProperties = CharacterSubsystem->MutableHeadMeshTable->FindRow<FMutableHumanHead>(Head->HeadMeshRowName, ContextString);
		}
	}
}

void FAnimNode_UpdateFace::GatherDebugData(FNodeDebugData& DebugData)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(GatherDebugData)
	FString DebugLine = DebugData.GetNodeName(this);
	DebugData.AddDebugItem(DebugLine);

	SourcePose.GatherDebugData(DebugData);
}