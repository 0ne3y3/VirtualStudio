// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimGraphNode_ProcessFacePose.h"

#define LOCTEXT_NAMESPACE "WanUpdateFace"

UAnimGraphNode_ProcessFacePose::UAnimGraphNode_ProcessFacePose()
{
}

FText UAnimGraphNode_ProcessFacePose::GetMenuCategory() const
{
	return LOCTEXT("UAnimGraphNode_ProcessFacePose_Category", "WanCharacter");
}

FText UAnimGraphNode_ProcessFacePose::GetTooltipText() const
{
	return LOCTEXT("UAnimGraphNode_ProcessFacePose_Tooltip", "Find the nearest face pose based on ARkit curves, then blend to it at n speed");
}

FText UAnimGraphNode_ProcessFacePose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("UAnimGraphNode_ProcessFacePose_Title", "Process ARKit face pose");
}