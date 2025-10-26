// Fill out your copyright notice in the Description page of Project Settings.


#include "FAnimNode_ProcessSimplePose.h"
#include "HumanAnimationData.h"
#include "AnimationRuntime.h"
#include "Animation/AnimCurveUtils.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimStats.h"
#include "Animation/PoseAsset.h"

FAnimNode_ProcessSimplePose::FAnimNode_ProcessSimplePose()
: LODThreshold(INDEX_NONE)
{
}

void FAnimNode_ProcessSimplePose::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Initialize_AnyThread)
	Super::Initialize_AnyThread(Context);
	SourcePose.Initialize(Context);
}

void FAnimNode_ProcessSimplePose::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(CacheBones_AnyThread)
	Super::CacheBones_AnyThread(Context);
	SourcePose.CacheBones(Context);
}

void FAnimNode_ProcessSimplePose::Evaluate_AnyThread(FPoseContext& Output)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Evaluate_AnyThread)
	QUICK_SCOPE_CYCLE_COUNTER(UpdateFace);

	if(!IsLODEnabled(Output.AnimInstanceProxy) || !FaceProperties)
	{
		SourcePose.Evaluate(Output);
		return;
	}

	FPoseContext SourceData(Output);
	SourcePose.Evaluate(SourceData);

	Output = SourceData;

	GetCurvesValue(Output, CachedCurves);
	PreprocessARKitCurves(Output, CachedCurves);
	ApplyFacePose(Output, CachedCurves);
}

void FAnimNode_ProcessSimplePose::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Update_AnyThread)

	// Run update on input pose nodes
	SourcePose.Update(Context);

	// Evaluate any BP logic plugged into this node
	GetEvaluateGraphExposedInputs().Execute(Context);

	CachedCurves.Empty();
	if(FaceProperties) GetCurvesName(CachedCurves);
}

void FAnimNode_ProcessSimplePose::GatherDebugData(FNodeDebugData& DebugData)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(GatherDebugData)
	FString DebugLine = DebugData.GetNodeName(this);
	DebugData.AddDebugItem(DebugLine);

	SourcePose.GatherDebugData(DebugData);
}

void FAnimNode_ProcessSimplePose::GetCurvesName(TMap<FName, float>& OutArray)
{
	if(UARkitPreProcessorPreset* ARKitPreprocessorPreset = FaceProperties->ARKitPreprocessorPreset)
	{
		for(FARKitPreProcessorData& PreprocessorData : ARKitPreprocessorPreset->Preprocessors)
		{
			for(FName& CurveName : PreprocessorData.CurvesToEvaluate)
			{
				if(!OutArray.Contains(CurveName))
				{
					OutArray.Add(CurveName, 0.f);
				}
			}
		}
	}

	if(USimpleArkitToFaceCurvesPreset* SimplePosePreset = FaceProperties->SimplePosePreset)
	{
		for(FSimpleARKitToFaceCurvesData& SimpleFacePoseData : SimplePosePreset->SimpleFacePoses)
		{
			for(auto It = SimpleFacePoseData.CurvesToEvaluate.CreateConstIterator(); It; ++It)
			{
				if(!OutArray.Contains(It.Key()))
				{
					OutArray.Add(It.Key(), 0.f);
				}
			}
		}
	}
}

void FAnimNode_ProcessSimplePose::GetCurvesValue(FPoseContext& Output, TMap<FName, float>& OutArray)
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
	[&OutputArray=OutArray](const UE::Anim::FNamedIndexElement& InBulkElement, float InValue)
	{
		if(OutputArray.Contains(InBulkElement.Name))
		{
			OutputArray[InBulkElement.Name] = InValue;
		}
	});
}


void FAnimNode_ProcessSimplePose::PreprocessARKitCurves(FPoseContext& Output, TMap<FName, float>& InArray)
{
	if(UARkitPreProcessorPreset* ARKitPreprocessorPreset = FaceProperties->ARKitPreprocessorPreset)
	{
		for(auto It = InArray.CreateConstIterator(); It; ++It)
		{
			UCurveFloat* Curve = nullptr;
			for(FARKitPreProcessorData& PreprocessorData : ARKitPreprocessorPreset->Preprocessors)
			{
				if(PreprocessorData.CurvesToEvaluate.FindByKey(It.Key()))
				{
					Curve = PreprocessorData.Curve;
					break;
				}
			}
			
			if(Curve) 
			{
				float Value = Curve->GetFloatValue(It.Value());
				//Curves.Add(It.Key(), Value);
				InArray[It.Key()] = Value;
			}
		}
	}
}

void FAnimNode_ProcessSimplePose::ApplyFacePose(FPoseContext& Output, TMap<FName, float>& InArray)
{
	if(USimpleArkitToFaceCurvesPreset* SimplePosePreset = FaceProperties->SimplePosePreset)
	{
		if(SimplePosePreset->PoseAssetEyes && SimplePosePreset->PoseAssetMouth)
		{		
			// For each pose asset curves, calculate the curve value and save the information
			TArray<FPoseAssetCurve> PoseCurves;
			
			for(FSimpleARKitToFaceCurvesData& SimpleFacePoseData : SimplePosePreset->SimpleFacePoses)
			{
				FPoseAssetCurve PoseCurve;
				for(auto It = SimpleFacePoseData.CurvesToEvaluate.CreateConstIterator(); It; ++It)
				{
					if(InArray.Contains(It.Key())) PoseCurve.PoseCurveValue += InArray[It.Key()] * It.Value();
				}
				PoseCurve.PoseCurveValue /= SimpleFacePoseData.CurvesToEvaluate.Num();
				
				PoseCurve.PoseCurveName = SimpleFacePoseData.TargetName;
				PoseCurve.PoseCurveType = SimpleFacePoseData.PoseCurveType;

				PoseCurves.Add(PoseCurve);
			}

			// Finally build the final morphtarget array (pose asset can contain curve that drive material too)
			TMap<FName, float> FinalValues;
			for(FPoseAssetCurve& PoseCurve : PoseCurves)
			{
				int32 PoseIndex = INDEX_NONE;
				UPoseAsset* PoseAsset;
				if(PoseCurve.PoseCurveType == EPoseCurveType::Eyes)
				{
					PoseAsset = SimplePosePreset->PoseAssetEyes;
					PoseIndex = PoseAsset->GetPoseIndexByName(PoseCurve.PoseCurveName);
				}
				else
				{
					PoseAsset = SimplePosePreset->PoseAssetMouth;
					PoseIndex = PoseAsset->GetPoseIndexByName(PoseCurve.PoseCurveName);
				}

				if(PoseIndex != INDEX_NONE)
				{
					TArray<FName> PoseAssetNames = PoseAsset->GetCurveFNames();
					for(FName& CurveName : PoseAssetNames)
					{
						int32 CurveIndex = PoseAsset->GetCurveIndexByName(CurveName);
						float CurveValue = 0.f;
						PoseAsset->GetCurveValue(PoseIndex, CurveIndex, CurveValue);
						CurveValue *= PoseCurve.PoseCurveValue;
						if(FinalValues.Contains(CurveName))
						{
							FinalValues[CurveName] = FMath::Min(FinalValues[CurveName] + CurveValue, 1.f);
						}
						else
						{
							FinalValues.Add(CurveName, CurveValue);
						}
					}
				}
			}

			// Bulk set all the curves. Can't use FBlendedCurve directly because it doesn't contain a key (FName) check ? So duplicate can exist
			FBlendedCurve FinalArrayCurves;			
			for(auto It = FinalValues.CreateConstIterator(); It; ++It)
			{
				FinalArrayCurves.Add(It.Key(), It.Value());
			}
			
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
	}
}
