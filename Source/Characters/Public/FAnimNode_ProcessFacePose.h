// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNodeBase.h"
#include "FAnimNode_ProcessFacePose.generated.h"

class UARKitPosePreset;

/**
* FAnimNode_ProcessFacePose
*
* Animation node class for ARKit face pose.
*/
USTRUCT(BlueprintInternalUseOnly)
struct CHARACTERS_API FAnimNode_ProcessFacePose : public FAnimNode_Base
{
	GENERATED_BODY()

	FAnimNode_ProcessFacePose();

	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite, Category = Links)
	FPoseLink SourcePose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UpdateFace, meta = (PinShownByDefault))
	TObjectPtr<UARKitPosePreset> FaceProperties;

	/*
	 * Max LOD that this node is allowed to run
	 * For example if you have LODThreshold to be 2, it will run until LOD 2 (based on 0 index)
	 * when the component LOD becomes 3, it will stop update/evaluate
	 * currently transition would be issue and that has to be re-visited
	 */
	UPROPERTY(EditAnywhere, Category = Performance, meta = (PinHiddenByDefault, DisplayName = "LOD Threshold"))
	int32 LODThreshold;


	// FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;

	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	virtual int32 GetLODThreshold() const override { return LODThreshold; }
	// End of FAnimNode_Base interface

	private:
	TMap<FName, float> GoalPoseScore;
	TMap<FName, float> CurrentPoseValues;
	FName GoalPoseName;
	FName SecondPoseName;
	void PreprocessARKitCurves(FPoseContext& Output);
	void SearchGoalPose(FPoseContext& Output);
	void ApplyFacePose(FPoseContext& Output);
};