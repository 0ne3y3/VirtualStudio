// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimGraphNode_ProcessSimplePose.h"

#define LOCTEXT_NAMESPACE "WanUpdateFace"

UAnimGraphNode_ProcessSimplePose::UAnimGraphNode_ProcessSimplePose()
{
}

FText UAnimGraphNode_ProcessSimplePose::GetMenuCategory() const
{
	return LOCTEXT("AnimGraphNode_ProcessSimplePose_Category", "WanCharacter");
}

FText UAnimGraphNode_ProcessSimplePose::GetTooltipText() const
{
	return LOCTEXT("AnimGraphNode_ProcessSimplePose_Tooltip", "Blend ARkit curves to simple face pose");
}

FText UAnimGraphNode_ProcessSimplePose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("AnimGraphNode_ProcessSimplePose_Title", "Simple process ARKit to pose");
}