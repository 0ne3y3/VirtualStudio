// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "FAnimNode_ProcessFacePose.h"
#include "AnimGraphNode_ProcessFacePose.generated.h"

/** Process ARkit curves to face pose */
UCLASS()
class CHARACTERSEDITOR_API UAnimGraphNode_ProcessFacePose : public UAnimGraphNode_Base
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_ProcessFacePose Node;

	public:
	UAnimGraphNode_ProcessFacePose();

	// UEdGraphNode interface
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetMenuCategory() const override;
	// End of UEdGraphNode interface
};