// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "ModularMeshData.h"
#include "CharacterCreationUtility.generated.h"

class USinglePropertyView;
class UDetailsView;
class AHuman;
class UButton;

/**
 * UCharacterCreationUtility
 * 
 * Utility to create a human body mesh
 */
UCLASS()
class VIRTUALSTUDIOEDITOR_API UCharacterCreationUtility : public UEditorUtilityWidget
{
	GENERATED_BODY()

	UFUNCTION( BlueprintCallable )
	void DestroyCharacter();

	UFUNCTION( BlueprintCallable )
	void InitializeEditorWidget();

	UFUNCTION()
	void UpdateButtonClicked();

	UFUNCTION()
	void LoadButtonClicked();

	UFUNCTION()
	void SaveButtonClicked();

	bool IsAnimSequenceCompatible();

	protected :
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UDetailsView> CharacterDetailsView;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, meta = ( BindWidget ) )
	TObjectPtr<USinglePropertyView> AnimationPreviewView;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, meta = ( BindWidget ) )
	TObjectPtr<UButton> SaveButton;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, meta = ( BindWidget ) )
	TObjectPtr<UButton> UpdateButton;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, meta = ( BindWidget ) )
	TObjectPtr<UButton> LoadButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FHumanBodyInterface CharacterProperties;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
	TObjectPtr<UAnimSequence> AnimationPreview;

	TObjectPtr<AHuman> CharacterSpawned = nullptr;

	TObjectPtr<UHumanBodyData> CharacterDataAsset = nullptr;
};
