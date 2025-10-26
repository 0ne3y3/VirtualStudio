// Fill out your copyright notice in the Description page of Project Settings.


#include "FAnimNode_ProcessFacePose.h"
#include "HumanAnimationData.h"
#include "AnimationRuntime.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimStats.h"
#include "Animation/PoseAsset.h"

FAnimNode_ProcessFacePose::FAnimNode_ProcessFacePose()
: LODThreshold(INDEX_NONE)
{
}

void FAnimNode_ProcessFacePose::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Initialize_AnyThread)
	Super::Initialize_AnyThread(Context);
	SourcePose.Initialize(Context);

	GoalPoseName = FName("FC_Idle");
	SecondPoseName = FName("FC_Idle");
}

void FAnimNode_ProcessFacePose::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(CacheBones_AnyThread)
	Super::CacheBones_AnyThread(Context);
	SourcePose.CacheBones(Context);
}

void FAnimNode_ProcessFacePose::Evaluate_AnyThread(FPoseContext& Output)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Evaluate_AnyThread)
	QUICK_SCOPE_CYCLE_COUNTER(UpdateFace);

	if (!IsLODEnabled(Output.AnimInstanceProxy) || !FaceProperties)
	{
		SourcePose.Evaluate(Output);
		return;
	}

	FPoseContext SourceData(Output);
	SourcePose.Evaluate(SourceData);

	Output = SourceData;

	PreprocessARKitCurves(Output);
	SearchGoalPose(Output);
	ApplyFacePose(Output);
}

void FAnimNode_ProcessFacePose::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Update_AnyThread)

	// Run update on input pose nodes
	SourcePose.Update(Context);

	// Evaluate any BP logic plugged into this node
	GetEvaluateGraphExposedInputs().Execute(Context);

	if (FaceProperties)
	{
		if (FaceProperties->GoalPosePreset)
		{
			GoalPoseScore.Empty(FaceProperties->GoalPosePreset->GoalPoses.Num());
			if (FaceProperties->GoalPosePreset->PoseAsset)
			{
				TArray<FName> CurveNames = FaceProperties->GoalPosePreset->PoseAsset->GetCurveFNames();
				TMap<FName,float> TempMap = CurrentPoseValues;

				CurrentPoseValues.Empty(CurveNames.Num());
				for (FName CurveName : CurveNames)
				{
					if (TempMap.Contains(CurveName))
					{
						CurrentPoseValues.Add(CurveName, TempMap[CurveName]);
					}
					else
					{
						CurrentPoseValues.Add(CurveName, 0);
					}
				}
			}
		}
	}
}

void FAnimNode_ProcessFacePose::GatherDebugData(FNodeDebugData& DebugData)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(GatherDebugData)
	FString DebugLine = DebugData.GetNodeName(this);
	DebugData.AddDebugItem(DebugLine);

	SourcePose.GatherDebugData(DebugData);
}


void FAnimNode_ProcessFacePose::PreprocessARKitCurves(FPoseContext& Output)
{
	if (UARkitPreProcessorPreset* ARKitPreprocessorPreset = FaceProperties->ARKitPreprocessorPreset)
	{
		for (FARKitPreProcessorData& PreprocessorData : ARKitPreprocessorPreset->Preprocessors)
		{
			UCurveFloat* Curve = PreprocessorData.Curve;

			for (FName& CurvesToEvaluate : PreprocessorData.CurvesToEvaluate)
			{
				Output.Curve.Set(CurvesToEvaluate, Curve->GetFloatValue( Output.Curve.Get(CurvesToEvaluate) ));
			}
		}
	}
}

void FAnimNode_ProcessFacePose::SearchGoalPose(FPoseContext& Output)
{
	if (UGoalPosePreset* GoalPosePreset = FaceProperties->GoalPosePreset)
	{
		// First compare ARkit curves with Goal pose curves
		for (FGoalPoseData& GoalPose : GoalPosePreset->GoalPoses)
		{
			float Score = 0.f;

			for (auto It = GoalPose.CurvesToEvaluate.CreateConstIterator(); It; ++It)
			{
				float CurveGoalValue = It.Value();
				float CurveCurrentValue = Output.Curve.Get(It.Key());

				Score += 1.f - FMath::Abs(CurveGoalValue - CurveCurrentValue);
			}

			Score /= (float)GoalPose.CurvesToEvaluate.Num();

			if (GoalPoseScore.Contains(GoalPose.GoalCurveName))
			{
				GoalPoseScore[GoalPose.GoalCurveName] = Score;
			}
			else 
			{
				GoalPoseScore.Add(GoalPose.GoalCurveName, Score);
			}

		}

		// Now compare goal pose score and pick up the best score
		for (auto It = GoalPoseScore.CreateConstIterator(); It; ++It)
		{
			if (GoalPoseName.IsNone())
			{
				GoalPoseName = It.Key();
			}
			if (GoalPoseName.IsNone())
			{
				SecondPoseName = It.Key();
			}

			if (It.Value() > GoalPoseScore[GoalPoseName])
			{
				GoalPoseName = It.Key();
			}
			else if (It.Value() > GoalPoseScore[SecondPoseName])
			{
				SecondPoseName = It.Key();
			}
		}
	}
}

void FAnimNode_ProcessFacePose::ApplyFacePose(FPoseContext& Output)
{
	if (UGoalPosePreset* GoalPosePreset = FaceProperties->GoalPosePreset)
	{
		if (GoalPosePreset->PoseAsset)
		{
			TArray<FName> CurveNames = GoalPosePreset->PoseAsset->GetCurveFNames();
			int32 PoseIndex = GoalPosePreset->PoseAsset->GetPoseIndexByName(GoalPoseName);
			int32 PoseIndexSecondary = GoalPosePreset->PoseAsset->GetPoseIndexByName(SecondPoseName);
			FGoalPoseData* GoalPoseData = GoalPosePreset->GoalPoses.FindByPredicate([this](const FGoalPoseData& In){return In.GoalCurveName == this->GoalPoseName;});

			if (PoseIndex != INDEX_NONE && GoalPoseData)
			{
				//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FString::Printf(TEXT("Delta: %f, Speed: %f"), Output.AnimInstanceProxy->GetDeltaSeconds(), GoalPoseData->TransitionSpeed));

				//int32 debugI = 1;
				float ScoreAlpha = FMath::Clamp(( GoalPoseScore[GoalPoseName] - GoalPoseScore[SecondPoseName] ) * (float)GoalPoseScore.Num(), 0.f, 1.f);
				for (FName& CurveName : CurveNames)
				{
					int32 CurveIndex = GoalPosePreset->PoseAsset->GetCurveIndexByName(CurveName);
					float GoalValue = 0.f;
					float SecondValue = 0.f;
					GoalPosePreset->PoseAsset->GetCurveValue(PoseIndex, CurveIndex, GoalValue);
					GoalPosePreset->PoseAsset->GetCurveValue(PoseIndexSecondary, CurveIndex, SecondValue);

					float CurrentValue = CurrentPoseValues[CurveName];
					
					float GoalFinal = FMath::Lerp(SecondValue, GoalValue, ScoreAlpha);
					float FinalValue = FMath::FInterpTo(CurrentValue, GoalFinal, Output.AnimInstanceProxy->GetDeltaSeconds(), GoalPoseData->TransitionSpeed);
					//GEngine->AddOnScreenDebugMessage(debugI, 1.f, FColor::Yellow, FString::Printf(TEXT("%s : current: %f - goal: %f - final: %f"), *CurveName.ToString(), CurrentValue, GoalValue, FinalValue));

					Output.Curve.Set(CurveName, FinalValue);
					CurrentPoseValues[CurveName] = FinalValue;
				}
			}
		}
	}
}
