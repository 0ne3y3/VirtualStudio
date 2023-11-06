// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LivingBeing.h"
#include "ModularMeshData.h"
#include "Human.generated.h"

class UModularSkeletalMeshComponent;
struct FStreamableHandle;

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

	/* Construct the whole character with HumanBodyData */
	void ConstructHumanCharacter();

	// Called every frame
	virtual void Tick( float DeltaTime ) override;

	/* Load the mesh and its materials for the MainBody component. */
	void LoadMainBodyAsset( FModularMainBodyData& MainBodyData );
	/* Setup the MainBody component, this function assume the skeletal mesh used for the main body is loaded */
	UFUNCTION()
	void SetupMainBody();
	void UpdateMainBodyMaterials();

	/* Load the a skeletal mesh and its materials. */
	void LoadSkeletalMeshData( FModularSkeletalMeshData& SkeletalMeshData, TArray<FSoftObjectPath>& AdditionalAssets );
	/* Callback when the data is loaded */
	UFUNCTION()
	void FinishedLoadingSkeletalMeshData( EBodyPartType BodyPartType );
	/* Start loading one skeletal mesh component. */
	void StartLoadingSkeletalMeshData( FModularSkeletalMeshData& SkeletalMeshData );
	void LoadNextSkeletalMeshData( EBodyPartType NextBodyPart );
	/* Setup common properties in one skeletal mesh component and return it. */
	UModularSkeletalMeshComponent* SetupOneSkeletalMeshComponent( FModularSkeletalMeshData& SkeletalMeshData );
	/* Create a new skeletal mesh component, if component name already exist, return the component. Else, return the new component created. */
	UModularSkeletalMeshComponent* CreateSkeletalMeshComponent( FName BodyPartName );
	/* Initalize a skeletal mesh component with default behavior. */
	void InitializeSkeletalMeshComponent( UModularSkeletalMeshComponent* SkeletalMeshComponent );
	/* Update a skeletal mesh component materials. */
	void UpdateSkeletalMeshMaterials( UModularSkeletalMeshComponent* SkeletalMeshComponent, FModularSkeletalMeshData& SkeletalMeshData );
	void UpdateSkeletalMeshCommonMaterialParameters( UModularSkeletalMeshComponent* SkeletalMeshComponent, USkeletalMeshData* SkeletalMeshData );
	void UpdateHairMaterialParameters( UModularSkeletalMeshComponent* HairComponent, UHairMeshData* HairMeshData );
	void UpdateClothMaterialParameters( UModularSkeletalMeshComponent* ClothComponent, FModularSkeletalMeshData& ClothData, UClothMeshData* ParentMeshData = nullptr, bool ForcedCloth = false );
	void UpdateLeaderComponent( UModularSkeletalMeshComponent* SkeletalMeshComponent );
	/* Setup the a specific part of the body with its data, return true if the data asset is correctly setup */
	bool SetupHead( UModularSkeletalMeshComponent* HeadComponent, FModularSkeletalMeshData& HeadData );
	bool SetupHair( UModularSkeletalMeshComponent* HairComponent, FModularSkeletalMeshData& HairData );
	bool SetupCloth( UModularSkeletalMeshComponent* ClothComponent, FModularSkeletalMeshData& ClothData );

	bool IsSkinOnCloth( FModularSkeletalMeshData& ClothData );
	FModularSkeletalMeshData& CheckSkinAndClothFromParentBody( UClothMeshData* ClothData, bool& ForcedSkin, bool& ForcedCloth, bool& ParentLoaded );

	/* Load the eye mesh and its materials for Eyes component. */
	void LoadEyesAsset( FEyeData& EyeData );
	/* Setup the Eyes instanced mesh component, this function assume the static mesh used for the eyes is loaded */
	UFUNCTION()
	void SetupEyes();
	void UpdateEyesMaterial();
	/* Update the number of instance for the ISM. Change the instance position. Reset eyes animation data if needed */
	void UpdateNumberOfEyeInstance( int32 NumberOfEyes, USkeletalMeshComponent* HeadComponent );
	/* Update the eye default position from the ActiveHeadMeshData to FEyeData (used for eyes material customdata). */
	void UpdateEyeDefaultPosition();
	void UpdateEyesCustomData( FEyeData& EyeData, int32 NumberOfEyes );
	/* Update the eye position/scale/etc..., each frame. */
	void UpdateEyesAnimation( float DeltaTime );
	/* Update the position offset of the eyes, for the masking shader. Since the component is snap to head the root of the component is at the head bone position.
	So in shader, 0,0,0 is at the position of the head instead of the MainBody root bone */
	void UpdateEyePositionOffset();

	/* Update tears meshes */
	void UpdateTearsMesh();

	bool AreMeshesSetup();

	public:

	/* ################
		## GETTER ##
	   ################*/

	/* Get head skeletal mesh component. */
	UFUNCTION( BlueprintCallable )
	UModularSkeletalMeshComponent* GetHeadComponent() const;

	UFUNCTION( BlueprintCallable, BlueprintGetter )
	UModularSkeletalMeshComponent* GetMainBodyComponent() const;

	UFUNCTION( BlueprintCallable, BlueprintGetter )
	UModularSkeletalMeshComponent* GetBodyComponent(EBodyPartType BodyPartType) const;

	UFUNCTION( BlueprintCallable, BlueprintGetter )
	FHumanBodyData& GetHumanBodyData();

	/* Get the face ARKit data for child class Livelink human. */
	FORCEINLINE virtual UARKitPresetData* GetHeadAnimationData() const { return nullptr; };

	/* ################
		## SETTER ##
	   ################*/
	UFUNCTION( BlueprintCallable, BlueprintSetter )
	void SetHumanBodyData( const FHumanBodyData& InHumanBodyData );

	/* Set the face ARKit data for child class Livelink human. */
	FORCEINLINE virtual void SetHeadAnimationData( UARKitPresetData* InFaceARKitData ) {};

	/* ################
	## EFFECT ######
	################*/
	void SetGlobalWetness( float GlobalWetnessOpacity );
	void SetHeightWetness( float HeightWetness, float HeightWetnessOpacity );
	void SetIsUnderRoof( bool IsUnderRoof );
	void SetHeightMaskGlobal( float HeightMask );
	void SetSphereMaskGlobal( FVector4 SphereMaskPosition, float SphereMaskRadius );
	void SetSphereMaskHardness( float SphereMaskHardness );
	void SetReverseSphereMaskGlobal( bool IsMaskReverse );
	void SetReverseHeightMaskGlobal( bool IsMaskReverse );
	void SetFaceEffect( float EffectIndex );
	void SetEyesEmissive( float EmissiveCoef, TArray<int32>& EyesIndex );
	void SetEyesBleeding( float BleedingCoef, TArray<int32>& EyesIndex );
	void SetPupilScale( float PupilScale, TArray<int32>& EyesIndex );
	void SetHighlightStrength( float HighlightStrength, TArray<int32>& EyesIndex );

	protected:

	/* Main body of the character, it drive all modular body parts */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components", meta = ( AllowPrivateAccess = "true" ), BlueprintGetter = GetMainBodyComponent )
	TObjectPtr<UModularSkeletalMeshComponent> MainBody;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character|Data" )
	TObjectPtr<UHeadMeshData> ActiveHeadMeshData;

	/* Eyes of the character */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components", meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UInstancedStaticMeshComponent> Eyes;

	/* All skeletal mesh component of the human */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components" )
	TArray<TObjectPtr<UModularSkeletalMeshComponent>> SkeletalComponents;

	/* Tears static mesh used when crying effect is enabled */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components", meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UStaticMeshComponent> TearsSM;
	
	/* Struct containing all the human body information */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Character|Data", meta = ( ExposeOnSpawn = true), BlueprintSetter = SetHumanBodyData )
	FHumanBodyData HumanBodyData;

	/* Contain the human state data */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character|Data" )
	FHumanStateData HumanStateData;

	/* Main body postprocess animation blueprint datas */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Character|Data" )
	TObjectPtr<UMainBodyPostProcessAnimationData> MainBodyPostProcessAnimationData;
	


	/* ################
	## EDITOR ######
	################*/

	#if WITH_EDITORONLY_DATA
		
		public:
		UPROPERTY( BlueprintReadOnly )
		TObjectPtr<UAnimSequence> MainBodyAnimation_Editor;

		UPROPERTY( BlueprintReadOnly )
		TSubclassOf<UAnimInstance> AnimationBP_Editor;

		UPROPERTY( BlueprintReadOnly )
		float AnimationPlayRate_Editor = 1.f;

		UPROPERTY( BlueprintReadOnly )
		bool bTestBlink_Editor = false;

		/* Update the animation to play for MainBody. Used only by the character creator tool in editor */
		void UpdateMainBodyAnimation_Editor( UAnimSequence* AnimationToPlay, float InAnimPlayRate, FName InIphoneName );

		/* Set the name for ARKit name editor */
		FORCEINLINE virtual void SetArkitName_Editor( FName InName ) {};

		/* Set the animation blueprint for MainBody when in editor */
		void SetEditorAnimationBP_Editor( TSubclassOf<UAnimInstance> InAnimationBP );

		UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = ( DisplayName = "Debug arm curves" ) )
		bool DebugBodyArmCurves = false;

		UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = ( DisplayName = "Debug legs curves" ) )
		bool DebugBodyLegsCurves = false;

		UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = ( DisplayName = "Debug face curves" ) )
		bool DebugBodyFaceCurves = false;

		/* ################
		## DEBUG ######
		################*/
		void DebugAnimationDatasRuntime();
		void DebugMorphTargetDatasRuntime();
	#endif // WITH_EDITORONLY_DATA
};