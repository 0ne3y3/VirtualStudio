// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "FAnimNode_ProcessSimplePose.h"
#include "AnimGraphNode_ProcessSimplePose.generated.h"

/** Process ARkit curves to face pose */
UCLASS()
class CHARACTERSEDITOR_API UAnimGraphNode_ProcessSimplePose : public UAnimGraphNode_Base
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_ProcessSimplePose Node;

	public:
	UAnimGraphNode_ProcessSimplePose();

	// UEdGraphNode interface
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetMenuCategory() const override;
	// End of UEdGraphNode interface
};