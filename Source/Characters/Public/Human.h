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

	public:

	/** Default UObject constructor. */
	AHuman( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

	// Called every frame
	virtual void Tick( float DeltaTime ) override;

	/* Construction script when object spawn or created */
	virtual void OnConstruction( const FTransform& Transform ) override;

	/* Setup the main body skeletal mesh and its materials/customdata, called OnConstruction script. */
	UFUNCTION( BlueprintCallable )
	bool SetupMainBody( USkeletalMeshData* MeshData );

	/* Update the main body materials. */
	UFUNCTION( BlueprintCallable )
	void UpdateMainBodyMaterials();

	/* Update the main body CustomData. */
	UFUNCTION( BlueprintCallable )
	void UpdateMainBodyCustomData( FSkinFaceCustomData& SkinFaceData);

	/* Setup the eyes and their material/customdata, called OnConstruction script. */
	UFUNCTION( BlueprintCallable )
	bool SetupEyes( int32 NumberOfEyes, USkeletalMeshComponent* HeadComponent );

	/* Update the number of instance for the ISM. Change ,already existing, eye position if needed. */
	UFUNCTION( BlueprintCallable )
	void UpdateNumberOfEyeInstance( int32 NumberOfEyes, USkeletalMeshComponent* HeadComponent);

	/* Update the eye materials. */
	UFUNCTION( BlueprintCallable )
	void UpdateEyesMaterial( int32 MaterialIndex );

	/* Update the eye CustomData. */
	UFUNCTION( BlueprintCallable )
	void UpdateEyesCustomData( FEyeCustomData& EyeData, int32 NumberOfEyes );

	/* Update the eye default position. */
	UFUNCTION( BlueprintCallable )
	void UpdateEyeDefaultPosition();

	/* Update the eye position/scale/effect. */
	UFUNCTION( BlueprintCallable )
	void UpdateEyes();

	/* Setup all skeletal mesh components, called OnConstruction script. */
	UFUNCTION(BlueprintCallable)
	void SetupSkeletalMeshComponents( TArray<FModularSkeletalMeshData>& SkeletalMeshes );

	/* Setup one skeletal mesh components. */
	UFUNCTION( BlueprintCallable )
	void SetupOneSkeletalMeshComponent( FModularSkeletalMeshData& SkeletalMeshData );

	/* Create a new skeletal mesh component, if component name already exist, return the component. Else, return the new component created. */
	UFUNCTION( BlueprintCallable )
	USkeletalMeshComponent* CreateSkeletalMeshComponent( FName BodyPartName );

	/* Initalize skeletal mesh component with default behavior. */
	void InitializeSkeletalMeshComponent( USkeletalMeshComponent* SkeletalMeshComponent);

	/* Initalize skeletal mesh component with default behavior. */
	UFUNCTION( BlueprintCallable )
	void RefreshLeaderComponent( USkeletalMeshComponent* SkeletalMeshComponent );

	/* Update a skeletal mesh component materials. */
	UFUNCTION( BlueprintCallable )
	void UpdateSkeletalMeshMaterials( USkeletalMeshComponent* SkeletalMeshComponent, TArray<TSoftObjectPtr<UMaterialInstance>>& Materials );

	/* Update a skeletal mesh component custom data. */
	UFUNCTION( BlueprintCallable )
	void UpdateSkeletalMeshCustomData( USkeletalMeshComponent* SkeletalMeshComponent, TArray<float>& CustomDatas );

	/* Update a skeletal mesh component custom data. */
	UFUNCTION( BlueprintCallable )
	void UpdateHeadAnimInstance();

	#if WITH_EDITORONLY_DATA
	/* Update the animation to play for MainBody. Used only by the character creator tool in editor */
	void UpdateMainBodyAnimation( UAnimSequence* AnimationToPlay, bool ShowControlRig, float InAnimPlayRate);

	protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAnimSequence> MainBodyAnimationToPlay;

	UPROPERTY( BlueprintReadOnly )
	bool bShowControlRig;

	UPROPERTY( BlueprintReadOnly )
	float AnimPlayRate;
	#endif // WITH_EDITORONLY_DATA

	public:

	/* ################
		## GETTER ##
	   ################*/

	/* Get head skeletal mesh component. */
	UFUNCTION( BlueprintCallable )
	USkeletalMeshComponent* GetHeadComponent() const;

	UFUNCTION( BlueprintCallable, BlueprintGetter )
	USkeletalMeshComponent* GetMainBodyComponent() const;

	/* Get the face data from child class. */
	FORCEINLINE virtual UARKitPresetData* GetHeadAnimationData() const { return nullptr; };

	/* ################
		## SETTER ##
	   ################*/
	UFUNCTION( BlueprintCallable, BlueprintSetter )
	void SetHumanBodyData( const FHumanBodyData& InHumanBodyData );

	/* Set the face data for child class. */
	FORCEINLINE virtual void SetHeadAnimationData( UARKitPresetData* InFaceARKitData ) {};

	protected:

	/* Main body of the character, it drive all modular body parts */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = ( AllowPrivateAccess = "true" ), BlueprintGetter = GetMainBodyComponent )
	TObjectPtr<USkeletalMeshComponent> MainBody;

	/* Name of the main body mesh component */
	static FName MainBodyName;

	/* Eyes of the character */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UInstancedStaticMeshComponent> Eyes;

	/* Name of the eyes component */
	static FName EyesName;

	/* All skeletal mesh component of the human */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character" )
	TArray<TObjectPtr<USkeletalMeshComponent>> SkeletalComponents;
	
	/* Struct containing all the human body information */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Character|Data", meta = ( ExposeOnSpawn = true), BlueprintSetter = SetHumanBodyData )
	FHumanBodyData HumanBodyData;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character|Data" )
	TObjectPtr<UHeadMeshData> HeadMeshData;
	
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character|Data" )
	TObjectPtr<UMainBodyPostProcessAnimationData> MainBodyPP;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character|Data" )
	FDynamicMaterialData DynamicMaterialData;
};


/*

*/