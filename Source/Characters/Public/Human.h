// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LivingBeing.h"
#include "Human.generated.h"

class UCustomizableObject;
class UCustomizableObjectInstance;
class UCustomizableSkeletalComponent;
class UMutableBodyPreset;
class UMutableHeadPreset;
class UMutableCharacterPreset;

/**
* AHuman
*
* Base class for human character.
*/
UCLASS()
class CHARACTERS_API AHuman : public ALivingBeing
{
	GENERATED_BODY()

	public:

	/** Default UObject constructor. */
	AHuman( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

	/* Allow ticking in editor without simulating */
	bool ShouldTickIfViewportsOnly() const override;

	// Called when actor is spawned and start ticking
	virtual void BeginPlay() override;

	/* Construction script when object spawn or created */
	virtual void OnConstruction( const FTransform& Transform ) override;

	// Called every frame
	virtual void Tick( float DeltaTime ) override;

	void LoadAndChangeCharacterObjectInstance(FName RowName);
	void LoadAndChangeCharacterPreset(FName RowName);
	void ChangeBodyPreset(UMutableBodyPreset* BodyPreset, bool bUpdateMesh = false);
	void ChangeHeadPreset(UMutableHeadPreset* HeadPreset, bool bUpdateMesh = false);
	void CreateCustomMutableCharacterProperties(UMutableBodyPreset* BodyPreset, UMutableHeadPreset* HeadPreset);

	void UpdateMutableSkinMaterial(FString RowName);
	void UpdateMutableSkinColor(FString RowName);
	void UpdateMutableHeadMesh(FString RowName);
	void UpdateMutableIrisTexture(FString RowName);
	void UpdateMutableHighlightTexture(FString RowName);
	void UpdateMutableCETexture(FString RowName);
	void UpdateMutableBodyMesh(FString RowName);

	void InitBody(bool bUpdateMesh = false);
	void InitHead(bool bUpdateMesh = false);

	protected:

	void ConstructCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ShowInnerProperties))
	TObjectPtr<USkeletalMeshComponent> BodySkeletalMeshComponent;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UCustomizableSkeletalComponent> MutableBodyComponent;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UCustomizableObject> MutableBodyObject;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Transient)
	TObjectPtr<UCustomizableObjectInstance> MutableBodyInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "TRUE"))
	FName MutableBodyRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "TRUE"))
	TObjectPtr<UMutableCharacterPreset> MutableCharacterProperties;
};