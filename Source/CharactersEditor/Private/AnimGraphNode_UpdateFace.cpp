// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimGraphNode_UpdateFace.h"

#define LOCTEXT_NAMESPACE "WanUpdateFace"

UAnimGraphNode_UpdateFace::UAnimGraphNode_UpdateFace()
{
}

FText UAnimGraphNode_UpdateFace::GetMenuCategory() const
{
	return LOCTEXT("UAnimGraphNode_UpdateFace_Category", "WanCharacter");
}

FText UAnimGraphNode_UpdateFace::GetTooltipText() const
{
	return LOCTEXT("UAnimGraphNode_UpdateFace_Tooltip", "Update the character's face curves for their material. Like the iris position, mouth shade");
}

FText UAnimGraphNode_UpdateFace::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("UAnimGraphNode_UpdateFace_Title", "Character face update");
}