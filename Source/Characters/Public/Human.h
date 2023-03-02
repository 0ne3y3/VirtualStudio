// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LivingBeing.h"
#include "ModularMeshData.h"
#include "Human.generated.h"

class UModularSkeletalMeshComponent;

/**
* AHuman
*
* Base class for human character.
*/
UCLASS()
class CHARACTERS_API AHuman : public ALivingBeing
{
	GENERATED_BODY()

	/** Default constructor. */
	AHuman();

	public:

	/* Construction script when object spawn or created */
	virtual void OnConstruction( const FTransform& Transform ) override;

	/* Create a new skeletal mesh component, if the body part already exist, return its component. Else, return the new component created. */
	UModularSkeletalMeshComponent* CreateNewBodyPart( FName BodyPartName );

	/* Setup the main body skeletal mesh and its materials, called OnConstruction script. */
	UFUNCTION( BlueprintCallable )
	void SetupMainBody( USkeletalMesh* Mesh, UMainBodyMeshData* MainBodyMeshData, TArray<TSoftObjectPtr<UMaterialInstance>>& MeshMaterials );

	/* Setup the eyes and their material, called OnConstruction script. */
	UFUNCTION( BlueprintCallable )
	void SetupEyes( UStaticMesh* Mesh, FAccessoryMesh& BodyMeshData, USkeletalMeshComponent* ParentMeshComponent );

	/* Setup a body part skeletal mesh component, called OnConstruction script. */
	UFUNCTION(BlueprintCallable)
	void SetupModularBodyPart( USkeletalMesh* Mesh, FBodyMesh& BodyMeshData );

	/* Set the HumanBodyData */
	UFUNCTION( BlueprintSetter )
	void SetHumanBodyData( UHumanBodyData* InHumanBodyData ) { HumanBodyData = InHumanBodyData; };

	/* Get the HumanBodyData */
	UFUNCTION( BlueprintGetter )
	UHumanBodyData* GetHumanBodyData() const { return HumanBodyData; };

	/* Get the MainBody */
	UFUNCTION( BlueprintGetter )
	USkeletalMeshComponent* GetMainBody() const { return MainBody; };

	protected:

	/* Main body of the character, it drive all modular body parts */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = ( AllowPrivateAccess = "true" ), BlueprintGetter = GetMainBody )
	TObjectPtr<USkeletalMeshComponent> MainBody;

	/* Name of the main body mesh component */
	static FName MainBodyName;

	/* Eyes of the character */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UInstancedStaticMeshComponent> Eyes;

	/* Name of the eyes component */
	static FName EyesName;

	/* All modulars body parts component of the character, contained inside a component */
	TArray<TObjectPtr<UModularSkeletalMeshComponent>> BodyParts;
	
	/* Struct containing all the human body information */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Character", meta = ( ExposeOnSpawn = true), BlueprintGetter = GetHumanBodyData, BlueprintSetter = SetHumanBodyData )
	TObjectPtr<UHumanBodyData> HumanBodyData;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Character", AdvancedDisplay, Meta = ( DisplayName = "Main body post-process animation"))
	TObjectPtr<UMainBodyPostProcessAnimationData> PPAMainBody;

	bool bHumanHaveBody = false;
	bool bHumanHaveHead = false;
	bool bHumanHaveEyes = false;
};