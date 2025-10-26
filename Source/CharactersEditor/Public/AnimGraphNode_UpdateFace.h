// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "FAnimNode_UpdateFace.h"
#include "AnimGraphNode_UpdateFace.generated.h"

/** Easy way to modify curve values on a pose */
UCLASS()
class CHARACTERSEDITOR_API UAnimGraphNode_UpdateFace : public UAnimGraphNode_Base
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_UpdateFace Node;

	public:
	UAnimGraphNode_UpdateFace();

	// UEdGraphNode interface
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetMenuCategory() const override;
	// End of UEdGraphNode interface
	
	
};