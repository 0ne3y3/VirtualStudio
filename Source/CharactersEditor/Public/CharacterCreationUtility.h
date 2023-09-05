// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "ModularMeshData.h"
#include "CharacterCreationUtility.generated.h"

class UDetailsView;
class UAnimInstance;
class AHuman;
class UButton;

/**
 * UCharacterCreationUtility
 * 
 * Utility to create a human body mesh
 */
UCLASS( BlueprintType )
class CHARACTERSEDITOR_API UCharacterCreationUtility : public UEditorUtilityWidget
{
	GENERATED_BODY()

	UFUNCTION( BlueprintCallable )
	void DestroyCharacter();

	UFUNCTION( BlueprintCallable )
	void InitializeEditorWidget();

	UFUNCTION()
	void LoadButtonClicked();

	UFUNCTION()
	void SaveButtonClicked();

	UFUNCTION()
	void BlinkButtonClicked();

	bool IsAnimSequenceCompatible();

	virtual void NativeConstruct() override;

	UFUNCTION( BlueprintCallable )
	void UpdateCharacter();

	UFUNCTION( BlueprintCallable )
	void UpdateEyesArray();

	UFUNCTION( BlueprintCallable )
	void OnPropertyChangedMainPanel( FName PropertyName );

	UFUNCTION( BlueprintCallable )
	void OnPropertyChangedAnimation( FName PropertyName );

	UFUNCTION( BlueprintCallable )
	void OnPropertyChangedEditorPreview( FName PropertyName );

	void UpdatePreviewData();

	protected :
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UDetailsView> CharacterDetailsView;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, meta = ( BindWidget ) )
	TObjectPtr<UDetailsView> AnimationDetailsView;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, meta = ( BindWidget ) )
	TObjectPtr<UDetailsView> EditorDetailsView;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, meta = ( BindWidget ) )
	TObjectPtr<UButton> SaveButton;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, meta = ( BindWidget ) )
	TObjectPtr<UButton> LoadButton;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, meta = ( BindWidget ) )
	TObjectPtr<UButton> BlinkButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FHumanBodyData CharacterProperties;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Character", meta = ( DisplayName = "Class to use" ) )
	TSubclassOf<AHuman> CharacterClass;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = ( DisplayName = "MainBody animation preview" ) )
	TObjectPtr<UAnimSequence> AnimationPreview;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = ( DisplayName = "MainBody animation speed" ) )
	float AnimPlayRate = 1.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = ( DisplayName = "ABP used for the MainBody" ) )
	TSubclassOf<UAnimInstance> AnimationBP;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = ( DisplayName = "ARKit device name" ) )
	FName ARKitName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Preview|Effect", meta = ( DisplayName = "Face effect", ClampMin = 0, UIMin = 0 ) )
	float Effect = 0.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Preview|Effect", meta = ( DisplayName = "Eyes bleeding", ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	TArray<float> EyesBleeding;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Preview|Effect", meta = ( DisplayName = "Eyes emissive", ClampMin = 0, ClampMax = 10, UIMin = 0, UIMax = 10 ) )
	TArray<float> EyesEmissive;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Preview|Effect", meta = ( DisplayName = "Pupil Scale", ClampMin = 0, ClampMax = 2, UIMin = 0, UIMax = 2 ) )
	TArray<float> PupilScale;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Preview|Wetness", meta = ( DisplayName = "Global wetness", ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float MainWetness = 0.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Preview|Wetness", meta = ( DisplayName = "Height wetness (in UE unit)", ClampMin = 0, UIMin = 0 ) )
	float HeightWetness = 0.0f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Preview|Wetness", meta = ( DisplayName = "Height wetness opacity", ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1 ) )
	float HeightWetnessOpacity = 0.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Preview|Wetness", meta = ( DisplayName = "Is under roof" ) )
	bool UnderRoof = false;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Preview|Mask", meta = ( DisplayName = "Height mask (in UE unit)" ) )
	float HeightMask = 0.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Preview|Mask", meta = ( DisplayName = "Sphere mask position" ) )
	FVector SphereMaskPosition;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Preview|Mask", meta = ( DisplayName = "Sphere mask radius" ) )
	float SphereMaskRadius = 0.f;

	TObjectPtr<AHuman> CharacterSpawned = nullptr;
};
