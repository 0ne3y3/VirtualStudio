// Fill out your copyright notice in the Description page of Project Settings.


#include "Human.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "ModularSkeletalMeshComponent.h"
#include "Engine/AssetManager.h"
#include "CharacterSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC( LogCharacter, Log, All );

AHuman::AHuman( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	SetActorHiddenInGame( true );

	bRunConstructionScriptOnDrag = false;
	bAllowTickBeforeBeginPlay = false;
	PrimaryActorTick.bCanEverTick = true;

	MainBody = CreateDefaultSubobject<UModularSkeletalMeshComponent>( TEXT( "MainBodyComponent" ) );
	MainBody->bPropagateCurvesToFollowers = true;
	RootComponent = MainBody;
	/* Apply all default parameter, see later what to set as project goes */

	TearsSM = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "TearsComponent" ) );
	TearsSM->SetComponentTickEnabled( false );

	Eyes = CreateDefaultSubobject<UInstancedStaticMeshComponent>( TEXT( "EyesComponent" ) );
	Eyes->SetComponentTickEnabled( false );
	Eyes->SetVisibility( false );
}

bool AHuman::ShouldTickIfViewportsOnly() const
{
	if( GetWorld() != nullptr && GetWorld()->WorldType == EWorldType::Editor )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AHuman::BeginPlay()
{
	Super::BeginPlay();

	ConstructHumanCharacter();
}

void AHuman::OnConstruction( const FTransform& Transform )
{
	Super::OnConstruction( Transform );

	ConstructHumanCharacter();
}

void AHuman::ConstructHumanCharacter()
{
	UE_LOG( LogCharacter, Log, TEXT( "###### START CHARACTER CONSTRUCTION OF %s ######" ), *GetName() );
	UE_LOG( LogCharacter, Log, TEXT( "- Preset name: %s" ), *HumanBodyData.PresetName.ToString() );

	LoadMainBodyAsset( HumanBodyData.MainBody );
}

void AHuman::LoadMainBodyAsset( FModularMainBodyData& MainBodyData )
{
	if( MainBodyData.MeshData )
	{
		UE_LOG( LogCharacter, Log, TEXT( "###### LOAD MAIN BODY #####" ) );
		UAssetManager& AssetManager = UAssetManager::Get();

		TArray<FSoftObjectPath> AssetToLoad;

		AssetToLoad.Add(MainBodyData.MeshData->Mesh.ToSoftObjectPath());
		AssetToLoad.Add(MainBodyData.MeshData->SkinBodyTexture.ToSoftObjectPath());
		AssetToLoad.Add(MainBodyData.MeshData->MorphBodyTexture.ToSoftObjectPath());
		
		for(UMaterialData* DefaultMaterials : MainBodyData.MeshData->DefaultMaterials )
		{
			AssetToLoad.Add( DefaultMaterials->PrimaryMaterial.ToSoftObjectPath() );
		}

		if( MainBodyData.bUseCustomMaterial)
		{
			for( UMaterialData* CustomtMaterials : MainBodyData.CustomMaterials )
			{
				AssetToLoad.Add( CustomtMaterials->PrimaryMaterial.ToSoftObjectPath() );
			}
		}

		AssetManager.LoadAssetList( AssetToLoad, FStreamableDelegate::CreateUObject(this, &AHuman::SetupMainBody), 1);
	}
}

void AHuman::SetupMainBody()
{
	FModularMainBodyData& MainBodyData = HumanBodyData.MainBody;

	if( MainBodyData.MeshData )
	{
		UE_LOG( LogCharacter, Log, TEXT( "###### SETUP MAIN BODY #####" ) );
		if( MainBodyData.MeshData->Mesh.IsValid() )
		{
			UE_LOG( LogCharacter, Log, TEXT( "- Main body mesh loaded and set." ) );
			MainBody->SetModularSkeletalMesh( MainBodyData.MeshData->Mesh.Get() );

			UpdateMainBodyMaterials();
			UpdateSkeletalMeshCommonMaterialParameters( MainBody, MainBodyData.MeshData );
			
			#if WITH_EDITORONLY_DATA
			if( AnimationBP_Editor )
			{
				MainBody->SetAnimInstanceClass( AnimationBP_Editor );
			}
			else
			{
				MainBody->SetAnimInstanceClass( MainBodyData.MeshData->BodyAnimationBlueprint );
			}
			#else
			MainBody->SetAnimInstanceClass( MainBodyData.MeshData->BodyAnimationBlueprint );
			#endif
			
			MainBodyPostProcessAnimationData = MainBodyData.MeshData->PPAnimationData;

			MainBody->SetTextureParameterOnDynamicMaterials( TEXT( "SkinBodyTexture" ), HumanBodyData.MainBody.MeshData->SkinBodyTexture.Get() );
			MainBody->SetTextureParameterOnDynamicMaterials( TEXT( "MorphBodyTexture" ), HumanBodyData.MainBody.MeshData->MorphBodyTexture.Get() );

			#if WITH_EDITOR
			MainBody->SetUpdateAnimationInEditor( true );
			#endif

			FModularSkeletalMeshData& NextSkeletalMeshData = HumanBodyData.GetSkeletalMeshData( EBodyPartType::Head );
			StartLoadingSkeletalMeshData( NextSkeletalMeshData );
		}
	}
}

void AHuman::UpdateMainBodyMaterials()
{
	UE_LOG( LogCharacter, Log, TEXT( "- Setup material." ) );

	int32 MeshMaterialNum = MainBody->GetNumMaterials();

	bool bUseCustomMaterial = HumanBodyData.MainBody.bUseCustomMaterial;
	UE_LOG( LogCharacter, Log, TEXT( "  * Use %s material" ), ( bUseCustomMaterial ? TEXT("custom") : TEXT("default") ) );

	TArray<UMaterialData*>& CustomMaterials = HumanBodyData.MainBody.CustomMaterials;
	TArray<UMaterialData*>& DefaultMaterials = HumanBodyData.MainBody.MeshData->DefaultMaterials;

	for( uint8 MaterialIndex = 0; MaterialIndex < MeshMaterialNum; MaterialIndex++ )
	{
		UMaterialInterface* BaseMaterial = nullptr;
		if( bUseCustomMaterial && CustomMaterials.IsValidIndex(MaterialIndex) )
		{
			UE_LOG( LogCharacter, Log, TEXT( " * Slot %d : Custom" ), MaterialIndex );
			BaseMaterial = CustomMaterials[MaterialIndex]->PrimaryMaterial.Get();
		}

		if( !BaseMaterial && DefaultMaterials.IsValidIndex( MaterialIndex ) )
		{
			UE_LOG( LogCharacter, Log, TEXT( " * Slot %d : Default" ), MaterialIndex );
			BaseMaterial = DefaultMaterials[MaterialIndex]->PrimaryMaterial.Get();
		}
		
		if( !BaseMaterial )
		{
			UE_LOG( LogCharacter, Error, TEXT( " * Didn't found custom or default material for slot %d" ), MaterialIndex );
			BaseMaterial = MainBody->GetMaterial( MaterialIndex );
		}

		if( BaseMaterial )
		{
			UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create( BaseMaterial, this );
			MainBody->SetDynamicMaterialInstance( MaterialIndex, DynMaterial );
		}
	}
}

void AHuman::LoadSkeletalMeshData( FModularSkeletalMeshData& SkeletalMeshData, TArray<FSoftObjectPath>& AdditionalAssets )
{
	if( SkeletalMeshData.MeshData )
	{
		UE_LOG( LogCharacter, Log, TEXT( "###### LOAD SKELETAL MESH %s #####" ), *UEnum::GetValueAsString( SkeletalMeshData.MeshData->MeshType ) );
		UAssetManager& AssetManager = UAssetManager::Get();
		TArray<FSoftObjectPath> AssetToLoad;

		// Add all asset path to load
		AssetToLoad.Add( SkeletalMeshData.MeshData->Mesh.ToSoftObjectPath() );

		for( UMaterialData* DefaultMaterials : SkeletalMeshData.MeshData->DefaultMaterials )
		{
			AssetToLoad.Add( DefaultMaterials->PrimaryMaterial.ToSoftObjectPath() );
			AssetToLoad.Add( DefaultMaterials->SkinMaterial.ToSoftObjectPath() );
		}

		if( SkeletalMeshData.bUseCustomMaterial )
		{
			for( UMaterialData* CustomtMaterials : SkeletalMeshData.CustomMaterials )
			{
				AssetToLoad.Add( CustomtMaterials->PrimaryMaterial.ToSoftObjectPath() );
				AssetToLoad.Add( CustomtMaterials->SkinMaterial.ToSoftObjectPath() );
			}
		}

		for( FSoftObjectPath& SoftPath : AdditionalAssets )
		{
			AssetToLoad.Add(SoftPath);
		}

		AssetManager.LoadAssetList( AssetToLoad, FStreamableDelegate::CreateUObject( this, &AHuman::FinishedLoadingSkeletalMeshData, SkeletalMeshData.MeshData->MeshType ), 1);
	}
}

void AHuman::FinishedLoadingSkeletalMeshData( EBodyPartType BodyPartType )
{
	FModularSkeletalMeshData& SkeletalMeshData = HumanBodyData.GetSkeletalMeshData(BodyPartType);

	UE_LOG( LogCharacter, Log, TEXT( "###### SETUP SKELETAL MESH %s #####" ), *UEnum::GetValueAsString( SkeletalMeshData.MeshData->MeshType ) );

	UModularSkeletalMeshComponent* BodyComponent = SetupOneSkeletalMeshComponent( SkeletalMeshData );
	if( BodyComponent )
	{
		switch( BodyPartType )
		{
			case EBodyPartType::Head:
			SetupHead( BodyComponent, SkeletalMeshData );
			break;
			case EBodyPartType::Hair:
			SetupHair( BodyComponent, SkeletalMeshData );
			break;
			case EBodyPartType::Torso:
			case EBodyPartType::Arms:
			case EBodyPartType::Hands:
			case EBodyPartType::Legs:
			case EBodyPartType::Feet:
			SetupCloth( BodyComponent, SkeletalMeshData );
			default:
			break;
		}
	}
	else
	{
		HumanBodyData.RemoveOneSkeletalMeshPart( BodyPartType );
		LoadNextSkeletalMeshData( HumanBodyData.GetNextBodyPartToLoad( BodyPartType ) );
	}
}

void AHuman::StartLoadingSkeletalMeshData( FModularSkeletalMeshData& SkeletalMeshData )
{
	if( SkeletalMeshData.MeshData )
	{
			TArray<FSoftObjectPath> AdditionalAssets;

			if( UHeadMeshData* HeadMeshData = Cast<UHeadMeshData>( SkeletalMeshData.MeshData ) )
			{
				AdditionalAssets.Add( HeadMeshData->FaceTexture.ToSoftObjectPath() );
				AdditionalAssets.Add( HeadMeshData->TearsMesh.ToSoftObjectPath() );
				AdditionalAssets.Add( HeadMeshData->TearsMaterial.ToSoftObjectPath() );
			}
			else if (UClothMeshData* ClothMeshData = Cast<UClothMeshData>( SkeletalMeshData.MeshData ) )
			{
				AdditionalAssets.Add( ClothMeshData->ClothDetailTexture1.ToSoftObjectPath() );
				AdditionalAssets.Add( ClothMeshData->ClothDetailTexture2.ToSoftObjectPath() );
				AdditionalAssets.Add( ClothMeshData->ClothDetailTexture3.ToSoftObjectPath() );
				AdditionalAssets.Add( ClothMeshData->ClothDetailTexture4.ToSoftObjectPath() );
			}

			LoadSkeletalMeshData( SkeletalMeshData, AdditionalAssets );
	}
}

void AHuman::LoadNextSkeletalMeshData( EBodyPartType NextBodyPart )
{
	if( AreMeshesSetup() )
	{
		UE_LOG( LogCharacter, Log, TEXT( "###### ALL MESHES SETUP #####" ) );
		// MainBody masking

		UE_LOG( LogCharacter, Log, TEXT( "###### END CHARACTER CONSTRUCTION OF %s ######" ), *GetName() );
		// Start global body reveal, for now just unhide the character
		SetActorHiddenInGame( false );
		Eyes->SetVisibility( true );
	}
	else
	{
		UE_LOG( LogCharacter, Log, TEXT( " - Try loading next mesh %s" ), *UEnum::GetValueAsString( NextBodyPart ) );
		uint8 BodyMax = (uint8)EBodyPartType::BodyMAX;

		UE_LOG( LogCharacter, Log, TEXT( "  * Bodymax = %d" ), BodyMax );

		for( uint8 Index = 0; Index < BodyMax; Index++ )
		{
			FModularSkeletalMeshData& NextSkeletalMeshData = HumanBodyData.GetSkeletalMeshData( NextBodyPart );
			if( NextSkeletalMeshData.MeshData )
			{
				UE_LOG( LogCharacter, Log, TEXT( "  * %s found" ), *UEnum::GetValueAsString( NextBodyPart ) );
				return StartLoadingSkeletalMeshData( NextSkeletalMeshData );
			}
			else
			{
				NextBodyPart = HumanBodyData.GetNextBodyPartToLoad( NextBodyPart );
				UE_LOG( LogCharacter, Log, TEXT( "  * Try next mesh %s" ), *UEnum::GetValueAsString( NextBodyPart ) );
			}
		}
	}
}

bool AHuman::SetupHead( UModularSkeletalMeshComponent* HeadComponent, FModularSkeletalMeshData& HeadData )
{
	UE_LOG( LogCharacter, Log, TEXT( "  * Body part is head" ) );
	UHeadMeshData* HeadMeshData = Cast<UHeadMeshData>( HeadData.MeshData );

	if( !HeadMeshData )
	{
		HumanBodyData.RemoveOneSkeletalMeshPart( EBodyPartType::Head );
		SkeletalComponents.Remove( HeadComponent );
		HeadComponent->DestroyComponent();
		LoadNextSkeletalMeshData( EBodyPartType::Hair );
		return false;
	}

	ActiveHeadMeshData = HeadMeshData;

	HeadComponent->SetTextureParameterOnDynamicMaterials(TEXT("FaceTexture"), ActiveHeadMeshData->FaceTexture.Get() );

	TearsSM->AttachToComponent( HeadComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, FName( "head" ) );
	TearsSM->SetRelativeRotation( FRotator( -90, 0, 0 ) ); // somehow it's not the same rotation as the face
	( HumanStateData.FaceEffectIndex == 1 ) ? TearsSM->SetVisibility( true ) : TearsSM->SetVisibility( false );
	UpdateTearsMesh();

	LoadEyesAsset(HumanBodyData.EyeData );

	HeadComponent->SetIsFullySetup( true );

	#if WITH_EDITOR
	if( HeadComponent->HasValidAnimationInstance() )
	{
		HeadComponent->SetUpdateAnimationInEditor( true );
	}
	#endif

	LoadNextSkeletalMeshData( EBodyPartType::Hair );
	return true;
}

bool AHuman::SetupHair( UModularSkeletalMeshComponent* HairComponent, FModularSkeletalMeshData& HairData )
{
	UE_LOG( LogCharacter, Log, TEXT( "  * Body part is hair" ) );
	UHairMeshData* HairMeshData = Cast<UHairMeshData>( HairData.MeshData );

	if( !HairMeshData )
	{
		HumanBodyData.RemoveOneSkeletalMeshPart( EBodyPartType::Hair );
		SkeletalComponents.Remove( HairComponent );
		HairComponent->DestroyComponent();
		LoadNextSkeletalMeshData( EBodyPartType::Torso );
		return false;
	}

	UpdateHairMaterialParameters( HairComponent, HairMeshData );

	HairComponent->SetIsFullySetup( true );

	#if WITH_EDITOR
	if( HairComponent->HasValidAnimationInstance() )
	{
		HairComponent->SetUpdateAnimationInEditor( true );
	}
	#endif

	LoadNextSkeletalMeshData( EBodyPartType::Torso );
	return true;
}

bool AHuman::SetupCloth( UModularSkeletalMeshComponent* ClothComponent, FModularSkeletalMeshData& ClothData )
{
	UE_LOG( LogCharacter, Log, TEXT( "  * Body part is cloth" ) );

	UClothMeshData* ClothMeshData = Cast<UClothMeshData>( ClothData.MeshData );
	if( !ClothMeshData )
	{
		HumanBodyData.RemoveOneSkeletalMeshPart( ClothData.MeshData->MeshType );
		SkeletalComponents.Remove( ClothComponent );
		ClothComponent->DestroyComponent();
		LoadNextSkeletalMeshData( HumanBodyData.GetNextBodyPartToLoad( ClothData.MeshData->MeshType ) );
		return false;
	}

	bool ForcedSkin = false;
	bool ForcedCloth = false;
	bool ParentLoaded = false;
	FModularSkeletalMeshData& ParentBodyData = CheckSkinAndClothFromParentBody( ClothMeshData, ForcedSkin, ForcedCloth, ParentLoaded );

	ClothData.CurrentClothData = ClothMeshData->DefaultClothData;

	if( ForcedCloth )
	{
		ClothData.CopyPropertiesFromParent( ParentBodyData, ParentLoaded );
	}

	ClothData.bUseSkin = ( ForcedSkin ) ? true : ClothMeshData->bDefaultUseSkin;
	UE_LOG( LogCharacter, Log, TEXT( "  * bUseSkin: %s" ), ( ClothData.bUseSkin ? TEXT("TRUE") : TEXT("FALSE") ) );

	UpdateSkeletalMeshCommonMaterialParameters( ClothComponent, ClothMeshData );
	UpdateClothMaterialParameters( ClothComponent, ClothData, Cast<UClothMeshData>( ParentBodyData.MeshData ), ForcedCloth );

	ClothComponent->SetIsFullySetup( true );

	#if WITH_EDITOR
	if( ClothComponent->HasValidAnimationInstance() )
	{
		ClothComponent->SetUpdateAnimationInEditor( true );
	}
	#endif

	LoadNextSkeletalMeshData( HumanBodyData.GetNextBodyPartToLoad( ClothData.MeshData->MeshType ) );
	return true;
}

UModularSkeletalMeshComponent* AHuman::SetupOneSkeletalMeshComponent( FModularSkeletalMeshData& SkeletalMeshData )
{
	if( SkeletalMeshData.MeshData )
	{
		if( SkeletalMeshData.MeshData->Mesh.IsValid() )
		{
			UE_LOG( LogCharacter, Log, TEXT( "- Skeletal mesh %s loaded" ), *UEnum::GetValueAsString( SkeletalMeshData.MeshData->MeshType ) );
			if( UModularSkeletalMeshComponent* BodyComponent = CreateSkeletalMeshComponent( UEnum::GetValueAsName( SkeletalMeshData.MeshData->MeshType ) ) )
			{
				UE_LOG( LogCharacter, Log, TEXT( "  * Component created succesfully" ) );
				BodyComponent->SetModularSkeletalMesh( SkeletalMeshData.MeshData->Mesh.Get() );

				UpdateSkeletalMeshMaterials( BodyComponent, SkeletalMeshData );

				UpdateLeaderComponent( BodyComponent );

				return BodyComponent;
			}
		}
	}

	return nullptr;
}

UModularSkeletalMeshComponent* AHuman::CreateSkeletalMeshComponent( FName BodyPartName )
{
	// check if the body part already exist, return it if it's already created
	for( UModularSkeletalMeshComponent* SkeletalComponent : SkeletalComponents )
	{
		if( BodyPartName == SkeletalComponent->GetFName() )
		{
			return SkeletalComponent;
		}
	}

	UModularSkeletalMeshComponent* BodyComponentCreated = NewObject<UModularSkeletalMeshComponent>( this, UModularSkeletalMeshComponent::StaticClass(), BodyPartName );

	if( BodyComponentCreated )
	{
		InitializeSkeletalMeshComponent( BodyComponentCreated );
		SkeletalComponents.Add( BodyComponentCreated );
		return BodyComponentCreated;
	}

	UE_LOG( LogCharacter, Error, TEXT( " * Error when creating UModularSkeletalMeshComponent %s" ), *BodyPartName.ToString() );
	return nullptr;
}

void AHuman::InitializeSkeletalMeshComponent( UModularSkeletalMeshComponent* SkeletalMeshComponent )
{
	if( SkeletalMeshComponent->GetFName() != UEnum::GetValueAsName( EBodyPartType::Hair ) ) SkeletalMeshComponent->bUseAttachParentBound = true;
	SkeletalMeshComponent->CreationMethod = EComponentCreationMethod::Instance;
	SkeletalMeshComponent->AttachToComponent( MainBody, FAttachmentTransformRules::SnapToTargetIncludingScale );
	SkeletalMeshComponent->RegisterComponent();
}

void AHuman::UpdateSkeletalMeshMaterials( UModularSkeletalMeshComponent* SkeletalMeshComponent, FModularSkeletalMeshData& SkeletalMeshData )
{
	UE_LOG( LogCharacter, Log, TEXT( "- Setup material for %s" ), *UEnum::GetValueAsString( SkeletalMeshData.MeshData->MeshType ) );

	int32 MeshMaterialNum = SkeletalMeshComponent->GetNumMaterials();
	
	bool bUseCustomMaterial = SkeletalMeshData.bUseCustomMaterial;
	UE_LOG( LogCharacter, Log, TEXT( "  * Use %s material" ), ( bUseCustomMaterial ? TEXT( "custom" ) : TEXT( "default" ) ) );

	TArray<UMaterialData*>& CustomMaterials = SkeletalMeshData.CustomMaterials;
	TArray<UMaterialData*>& DefaultMaterials = SkeletalMeshData.MeshData->DefaultMaterials;
	bool bUseSkin = IsSkinOnCloth( SkeletalMeshData );

	UE_LOG( LogCharacter, Log, TEXT( "  * Use skin material: %s" ), ( bUseSkin ? TEXT( "TRUE" ) : TEXT( "FALSE" ) ) );

	int32 SkinIndex = SkeletalMeshComponent->GetMaterialIndex(TEXT("Skin"));

	for( uint8 MaterialIndex = 0; MaterialIndex < MeshMaterialNum; MaterialIndex++ )
	{
		if( MaterialIndex == SkinIndex )
		{
			UE_LOG( LogCharacter, Log, TEXT( " * Slot %d : Skin, skipping creating dynMat" ), MaterialIndex );

			int32 MainBodySkinIndex = MainBody->GetMaterialIndex( TEXT( "Skin" ) );
			if( MainBodySkinIndex != -1 )
			{
				UMaterialInstanceDynamic* MainBodyDynMat = Cast<UMaterialInstanceDynamic>( MainBody->GetMaterial(MainBodySkinIndex) );
				SkeletalMeshComponent->SetDynamicMaterialInstance( MaterialIndex, MainBodyDynMat );
				continue;
			}
		}

		UMaterialInterface* BaseMaterial = nullptr;

		if( bUseCustomMaterial && CustomMaterials.IsValidIndex( MaterialIndex ) )
		{
			if( bUseSkin )
			{
				if( CustomMaterials[MaterialIndex]->SkinMaterial.IsValid() )
				{
					UE_LOG( LogCharacter, Log, TEXT( " * Slot %d : Custom skin" ), MaterialIndex );
					BaseMaterial = CustomMaterials[MaterialIndex]->SkinMaterial.Get();
				}
			}
			else if( CustomMaterials[MaterialIndex]->PrimaryMaterial.IsValid() ){
				UE_LOG( LogCharacter, Log, TEXT( " * Slot %d : Custom cloth" ), MaterialIndex );
				BaseMaterial = CustomMaterials[MaterialIndex]->PrimaryMaterial.Get();
			}
		}

		if( !BaseMaterial && DefaultMaterials.IsValidIndex( MaterialIndex ) )
		{
			if( bUseSkin )
			{
				if( DefaultMaterials[MaterialIndex]->SkinMaterial.IsValid() )
				{
					UE_LOG( LogCharacter, Log, TEXT( " * Slot %d : default skin" ), MaterialIndex );
					BaseMaterial = DefaultMaterials[MaterialIndex]->SkinMaterial.Get();
				}
			}
			else if( DefaultMaterials[MaterialIndex]->PrimaryMaterial.IsValid() )
			{
				UE_LOG( LogCharacter, Log, TEXT( " * Slot %d : default cloth" ), MaterialIndex );
				BaseMaterial = DefaultMaterials[MaterialIndex]->PrimaryMaterial.Get();
			}
		}

		if( !BaseMaterial )
		{
			UE_LOG( LogCharacter, Error, TEXT( " * Didn't found custom or default material for slot %d" ), MaterialIndex );
			BaseMaterial = SkeletalMeshComponent->GetMaterial( MaterialIndex );
		}

		if( BaseMaterial )
		{
			UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create( BaseMaterial, this );
			SkeletalMeshComponent->SetDynamicMaterialInstance( MaterialIndex, DynMaterial );
		}
	}
}

void AHuman::UpdateLeaderComponent( UModularSkeletalMeshComponent* SkeletalMeshComponent )
{
	if( !SkeletalMeshComponent->HasValidAnimationInstance() )
	{
		UE_LOG( LogCharacter, Log, TEXT( "  * Setting MainBody as leader pose" ) );
		SkeletalMeshComponent->SetLeaderPoseComponent( MainBody );
	}
	else
	{
		UE_LOG( LogCharacter, Log, TEXT( "  * Has an animation BP, so no leader" ) );
		SkeletalMeshComponent->SetLeaderPoseComponent( nullptr );
	}
}

void AHuman::UpdateSkeletalMeshCommonMaterialParameters( UModularSkeletalMeshComponent* SkeletalMeshComponent, USkeletalMeshData* SkeletalMeshData )
{
	UE_LOG( LogCharacter, Log, TEXT( " * Setup common material parameters." ) );
	TMap<FName, float> SkinFaceFloatProperties = HumanBodyData.SkinAndFaceData.GetSkinFloatTMap();
	TMap<FName, FVector4> SkinFaceFVector4Properties = HumanBodyData.SkinAndFaceData.GetSkinVectorTMap();
	TMap<FName, float> RainProperties = SkeletalMeshData->GetRainFloatTMap();

	for( const TPair<FName, float>& Pair : SkinFaceFloatProperties )
	{
		SkeletalMeshComponent->SetScalarParameterValueOnMaterials( Pair.Key, Pair.Value );
	}

	for( const TPair<FName, FVector4>& Pair : SkinFaceFVector4Properties )
	{
		SkeletalMeshComponent->SetVectorParameterValueOnMaterials( Pair.Key, Pair.Value );
	}

	for( const TPair<FName, float>& Pair : RainProperties )
	{
		SkeletalMeshComponent->SetScalarParameterValueOnMaterials( Pair.Key, Pair.Value );
	}
}

void AHuman::UpdateHairMaterialParameters( UModularSkeletalMeshComponent* HairComponent, UHairMeshData* HairMeshData )
{
	TMap<FName, float> HairFloatProperties = HumanBodyData.HairData.GetHairTMap();
	TMap<FName, float> HairMeshFloatProperties = HairMeshData->GetHairMeshFloatTMap();
	TMap<FName, FVector4> HairMeshVectorProperties = HairMeshData->GetHairMeshVector4TMap();

	for( const TPair<FName, float>& Pair : HairFloatProperties )
	{
		HairComponent->SetScalarParameterValueOnMaterials( Pair.Key, Pair.Value );
	}

	for( const TPair<FName, float>& Pair : HairMeshFloatProperties )
	{
		HairComponent->SetScalarParameterValueOnMaterials( Pair.Key, Pair.Value );
	}

	for( const TPair<FName, FVector4>& Pair : HairMeshVectorProperties )
	{
		HairComponent->SetVectorParameterValueOnMaterials( Pair.Key, Pair.Value );
	}
}

void AHuman::UpdateClothMaterialParameters( UModularSkeletalMeshComponent* ClothComponent, FModularSkeletalMeshData& ClothData, UClothMeshData* ParentMeshData, bool ForcedCloth )
{
	UE_LOG( LogCharacter, Log, TEXT( " * Setup cloth material parameters." ) );
	TMap<FName, float> ClothFloatProperties = ClothData.CurrentClothData.GetClothFloatTMap();
	TMap<FName, FVector4> ClothFVectorProperties = ClothData.CurrentClothData.GetClothVector4TMap();

	for( const TPair<FName, float>& Pair : ClothFloatProperties )
	{
		ClothComponent->SetScalarParameterValueOnMaterials( Pair.Key, Pair.Value );
	}

	for( const TPair<FName, FVector4>& Pair : ClothFVectorProperties )
	{
		ClothComponent->SetVectorParameterValueOnMaterials( Pair.Key, Pair.Value );
	}

	if( UClothMeshData* ClothMeshData = Cast<UClothMeshData>( ClothData.MeshData ) )
	{		
		if( ForcedCloth && ParentMeshData )
		{
			UE_LOG( LogCharacter, Log, TEXT( " * Forced cloth." ) );
			EBodyPartType MeshType = ClothMeshData->MeshType;
			FChildBodyPart* ChildMeshData = ParentMeshData->ChildBodyParts.FindByPredicate( [MeshType]( const FChildBodyPart& ChildBody ) { return ChildBody.MeshType == MeshType; } );
			if( ChildMeshData )
			{
				UE_LOG( LogCharacter, Log, TEXT( " * Child found." ) );
				switch( ChildMeshData->ClothPropertiesIndex )
				{
					case 1:
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture1" ), ParentMeshData->ClothDetailTexture1.Get() );
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture2" ), ClothMeshData->ClothDetailTexture2.Get() );
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture3" ), ClothMeshData->ClothDetailTexture3.Get() );
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture4" ), ClothMeshData->ClothDetailTexture4.Get() );
					break;
					case 2:
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture2" ), ParentMeshData->ClothDetailTexture2.Get() );
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture1" ), ClothMeshData->ClothDetailTexture1.Get() );
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture3" ), ClothMeshData->ClothDetailTexture3.Get() );
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture4" ), ClothMeshData->ClothDetailTexture4.Get() );
					break;
					case 3:
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture3" ), ParentMeshData->ClothDetailTexture3.Get() );
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture1" ), ClothMeshData->ClothDetailTexture1.Get() );
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture2" ), ClothMeshData->ClothDetailTexture2.Get() );
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture4" ), ClothMeshData->ClothDetailTexture4.Get() );
					break;
					case 4:
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture4" ), ParentMeshData->ClothDetailTexture4.Get() );
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture1" ), ClothMeshData->ClothDetailTexture1.Get() );
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture2" ), ClothMeshData->ClothDetailTexture2.Get() );
					ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture3" ), ClothMeshData->ClothDetailTexture3.Get() );
					break;
					default:
					break;
				}
			}
		}
		else
		{
			UE_LOG( LogCharacter, Log, TEXT( " * Not forced." ) );
			ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture1" ), ClothMeshData->ClothDetailTexture1.Get() );
			ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture2" ), ClothMeshData->ClothDetailTexture2.Get() );
			ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture3" ), ClothMeshData->ClothDetailTexture3.Get() );
			ClothComponent->SetTextureParameterOnDynamicMaterials( TEXT( "ClothDetailTexture4" ), ClothMeshData->ClothDetailTexture4.Get() );
		}
	}
}

bool AHuman::AreMeshesSetup()
{
	if( HumanBodyData.SkeletalMeshes.Num() != SkeletalComponents.Num() ) return false;

	for( UModularSkeletalMeshComponent* SkelMeshComponent : SkeletalComponents )
	{
		if( !SkelMeshComponent->IsAllSetup() ) return false;
	}

	return true;
}

bool AHuman::IsSkinOnCloth( FModularSkeletalMeshData& ClothData )
{
	UClothMeshData* ClothMeshData = Cast<UClothMeshData>( ClothData.MeshData );
	if( !ClothMeshData ) return false;

	bool ForcedSkin = false;
	bool ForcedCloth = false;
	bool ParentLoaded = false;
	CheckSkinAndClothFromParentBody( ClothMeshData, ForcedSkin, ForcedCloth, ParentLoaded );

	UE_LOG( LogCharacter, Log, TEXT( " - Check skin. CanHaveSkin: %s, ForcedSkin: %s, ForcedCloth %s, ParentLoaded: %s" ), ( ClothMeshData->DefaultClothData.bCanHaveSkin ? TEXT("TRUE") : TEXT("FALSE")),
	( ForcedSkin  ? TEXT( "TRUE" ) : TEXT( "FALSE" )),
	( ForcedCloth ? TEXT( "TRUE" ) : TEXT( "FALSE" ) ),
	( ParentLoaded  ? TEXT( "TRUE" ) : TEXT( "FALSE" )) );

	return ClothMeshData->DefaultClothData.bCanHaveSkin && ( (ClothData.bUseSkin && !ForcedCloth) || ForcedSkin );
}

FModularSkeletalMeshData& AHuman::CheckSkinAndClothFromParentBody( UClothMeshData* ClothData, bool& ForcedSkin, bool& ForcedCloth, bool& ParentLoaded )
{
	UCharacterSubsystem* CharacterSubsystem = GEngine->GetEngineSubsystem<UCharacterSubsystem>();

	if( !ClothData ) return CharacterSubsystem->GetDefaultModularSkeletalMeshData();
	
	for( EBodyPartType ParentType : ClothData->ParentHierarchy )
	{
		UE_LOG( LogCharacter, Log, TEXT( " - Check parent %s" ), *UEnum::GetValueAsString( ParentType ) );
		for( FModularSkeletalMeshData& SkeletalData : HumanBodyData.SkeletalMeshes )
		{
			if( SkeletalData.MeshData )
			{
				if( SkeletalData.MeshData->MeshType == ParentType )
				{
					if( UClothMeshData* ParentMeshData = Cast<UClothMeshData>( SkeletalData.MeshData ) )
					{
						UE_LOG( LogCharacter, Log, TEXT( "  * Found cloth parent" ) );
						if( FChildBodyPart* ChildBodyData = ParentMeshData->ChildBodyParts.FindByPredicate( [ClothData]( FChildBodyPart& ChildBody ) { return ChildBody.MeshType == ClothData->MeshType; } ) )
						{
							UE_LOG( LogCharacter, Log, TEXT( "  * Parent have child body part" ) );
							if( ChildBodyData->bSkinMaterialUsed )
							{
								if( UModularSkeletalMeshComponent* SkeletalMeshComponent = GetBodyComponent( ParentType ) )
								{
									if( SkeletalMeshComponent->IsAllSetup() )
									{
										UE_LOG( LogCharacter, Log, TEXT("  * Parent skin %s "), ( SkeletalData.bUseSkin ? TEXT("TRUE") : TEXT("FALSE") ) );
										ForcedSkin = SkeletalData.bUseSkin;
										ParentLoaded = true;
									}
									else
									{
										ForcedSkin = ParentMeshData->bDefaultUseSkin;
										ParentLoaded = false;
									}
								}
								else
								{
									ForcedSkin = ParentMeshData->bDefaultUseSkin;
									ParentLoaded = false;
								}
							}
							else if( ChildBodyData->bForceUsingCloth )
							{
								ForcedCloth = true;
							}
							return SkeletalData;
						}
					}
					break;
				}
			}
		}
	}

	return CharacterSubsystem->GetDefaultModularSkeletalMeshData();
}

/* ################
	## TEARS  ##
   ################*/

void AHuman::UpdateTearsMesh()
{
	if( !ActiveHeadMeshData ) return;

	TearsSM->SetStaticMesh( ActiveHeadMeshData->TearsMesh.Get() );
	TearsSM->SetMaterial(0, ActiveHeadMeshData->TearsMaterial.Get());
	
	// niagara mesh if any
}

/* ################
	## EYES  ##
   ################*/

void AHuman::LoadEyesAsset(FEyeData& EyeData )
{
	if( ActiveHeadMeshData )
	{
		UE_LOG( LogCharacter, Log, TEXT( "###### LOAD EYES #####" ) );
		UAssetManager& AssetManager = UAssetManager::Get();
		TArray<FSoftObjectPath> AssetToLoad;

		// Add all asset path to load
		AssetToLoad.Add( ActiveHeadMeshData->EyeMesh.ToSoftObjectPath() );

		for( auto& DefaultMaterials : ActiveHeadMeshData->DefaultEyesMaterials )
		{
			AssetToLoad.Add( DefaultMaterials.ToSoftObjectPath() );
		}

		if( EyeData.CustomEyesMaterials.Num() > 0 )
		{
			for( auto& CustomMaterials : EyeData.CustomEyesMaterials )
			{
				AssetToLoad.Add( CustomMaterials.ToSoftObjectPath() );
			}
		}

		// Start async load
		AssetManager.LoadAssetList( AssetToLoad, FStreamableDelegate::CreateUObject( this, &AHuman::SetupEyes ), 2 );
	}
}

void AHuman::SetupEyes()
{
	UE_LOG( LogCharacter, Log, TEXT( "###### SETUP EYES #####" ) );

	if( ActiveHeadMeshData )
	{
		if( ActiveHeadMeshData->EyeMesh.IsValid() )
		{
			UE_LOG( LogCharacter, Log, TEXT( " * Eye mesh loaded and set" ));
			UModularSkeletalMeshComponent* HeadComponent = GetHeadComponent();
			Eyes->AttachToComponent( ( HeadComponent ) ? HeadComponent : MainBody, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT( "head" ) );
			Eyes->SetStaticMesh( ActiveHeadMeshData->EyeMesh.Get() );

			uint32 NumberOfEyes = ActiveHeadMeshData->NumberOfEyes;
			UE_LOG( LogCharacter, Log, TEXT( " * Number of eyes: %d" ), NumberOfEyes );
			if( NumberOfEyes < 1 )
			{
				Eyes->ClearInstances();
			}
			else
			{
				Eyes->NumCustomDataFloats = FEyeData::MaxCustomData;

				UpdateNumberOfEyeInstance( NumberOfEyes, HeadComponent );

				UpdateEyesMaterial();

				UpdateEyeDefaultPosition();
				UpdateEyePositionOffset();

				UpdateEyesCustomData( HumanBodyData.EyeData, NumberOfEyes );
			}
		}
	}
}

void AHuman::UpdateNumberOfEyeInstance( int32 NumberOfEyes, USkeletalMeshComponent* HeadComponent)
{
	UE_LOG( LogCharacter, Log, TEXT( "- Update instances" ));

	if( !HeadComponent || !ActiveHeadMeshData ) return Eyes->ClearInstances();

	int32 EyeLimit = FMath::Max( NumberOfEyes, Eyes->GetInstanceCount() );

	for( uint8 EyeIndex = 0; EyeIndex < EyeLimit; EyeIndex++ )
	{
		if( EyeIndex < NumberOfEyes )
		{
			if( !ActiveHeadMeshData->EyesTransform.IsValidIndex( EyeIndex ) )
			{
				UE_LOG( LogCharacter, Log, TEXT( "- Eye %d don't have transforms in HeadMeshData->EyesTransform, not adding instance." ), EyeIndex );
				continue;
			}

			if( Eyes->IsValidInstance( EyeIndex ) )
			{
				Eyes->UpdateInstanceTransform( EyeIndex, ActiveHeadMeshData->EyesTransform[EyeIndex], false);
			}
			else
			{
				Eyes->AddInstance( ActiveHeadMeshData->EyesTransform[EyeIndex], false );
			}
		}
		else
		{
			Eyes->RemoveInstance( EyeIndex );
		}
	}

	if( NumberOfEyes != HumanStateData.EyesBleeding.Num() )
	{
		UE_LOG( LogCharacter, Log, TEXT( " * Reset eyes animation array" ) );
		// Don't use empty() because this realloc the size of the array (can be expensive). Since we have 2 eyes 99% of the time. 
		HumanStateData.EyesBleeding.Reset();
		HumanStateData.PupilScale.Reset();
		HumanStateData.EyesEmissive.Reset();
		HumanStateData.HighlightStrength.Reset();
		HumanStateData.LastFrameEyeBlink.Reset();
		HumanStateData.EyesBleeding.Init( 0, NumberOfEyes );
		HumanStateData.PupilScale.Init( 1, NumberOfEyes );
		HumanStateData.EyesEmissive.Init( 0, NumberOfEyes );
		HumanStateData.HighlightStrength.Init( 0, NumberOfEyes );
		HumanStateData.LastFrameEyeBlink.Init( 0, NumberOfEyes );
	}
}

void AHuman::UpdateEyesMaterial()
{
	UHeadMeshData* HeadMeshData = Cast<UHeadMeshData>( HumanBodyData.GetSkeletalMeshData( EBodyPartType::Head ).MeshData );
	if( !Eyes->GetStaticMesh() || !HeadMeshData ) return;

	UE_LOG( LogCharacter, Log, TEXT( "- Setup material." ) );

	int32 MeshMaterialNum = Eyes->GetNumMaterials();

	bool bUseCustomMaterial = HumanBodyData.EyeData.CustomEyesMaterials.Num() > 0;
	UE_LOG( LogCharacter, Log, TEXT( "  * Use %s material" ), ( bUseCustomMaterial ? TEXT( "custom" ) : TEXT( "default" ) ) );

	TArray<TSoftObjectPtr<UMaterialInterface>>& CustomMaterials = HumanBodyData.EyeData.CustomEyesMaterials;
	TArray<TSoftObjectPtr<UMaterialInterface>>& DefaultMaterials = HeadMeshData->DefaultEyesMaterials;

	for( uint8 MaterialIndex = 0; MaterialIndex < MeshMaterialNum; MaterialIndex++ )
	{
		UMaterialInterface* BaseMaterial = nullptr;
		if( bUseCustomMaterial && CustomMaterials.IsValidIndex( MaterialIndex ) )
		{
			UE_LOG( LogCharacter, Log, TEXT( " * Slot %d : Custom" ), MaterialIndex );
			BaseMaterial = CustomMaterials[MaterialIndex].Get();
		}

		if( !BaseMaterial && DefaultMaterials.IsValidIndex( MaterialIndex ) )
		{
			UE_LOG( LogCharacter, Log, TEXT( " * Slot %d : Default" ), MaterialIndex );
			BaseMaterial = DefaultMaterials[MaterialIndex].Get();
		}

		if( !BaseMaterial )
		{
			UE_LOG( LogCharacter, Error, TEXT( " * Didn't found custom or default material for slot %d" ), MaterialIndex );
			BaseMaterial = MainBody->GetMaterial( MaterialIndex );
		}

		if( BaseMaterial )
		{
			Eyes->SetMaterial( MaterialIndex, BaseMaterial );
		}
	}
}

void AHuman::UpdateEyeDefaultPosition()
{
	if( ActiveHeadMeshData )
	{
		for( uint8 EyeIndex = 0; EyeIndex < HumanBodyData.EyeData.IrisOffset.Num(); EyeIndex++ )
		{
			if( HumanBodyData.EyeData.IrisOffset.IsValidIndex( EyeIndex ) ) HumanBodyData.EyeData.IrisOffset[EyeIndex] = ActiveHeadMeshData->IrisDefaultPosition;
		}
	}
}

void AHuman::UpdateEyePositionOffset()
{
	FVector EyesLocation = Eyes->GetComponentLocation() - MainBody->GetBoneLocation( TEXT( "root" ) );

	UE_LOG( LogCharacter, Log, TEXT("* EyesLocation: %s"), *EyesLocation.ToString());

	for( uint8 EyeIndex = 0; EyeIndex < Eyes->GetInstanceCount(); EyeIndex++ )
	{
		Eyes->SetCustomDataValue( EyeIndex, 32, EyesLocation.X );
		Eyes->SetCustomDataValue( EyeIndex, 33, EyesLocation.Y );
		Eyes->SetCustomDataValue( EyeIndex, 34, EyesLocation.Z );
	}
}

void AHuman::UpdateEyesCustomData( FEyeData& EyeData, int32 NumberOfEyes )
{
	UE_LOG( LogCharacter, Log, TEXT( "- Setup Custom data." ) );
	for( uint8 EyeIndex = 0; EyeIndex < Eyes->GetInstanceCount(); EyeIndex++ )
	{
		for( uint8 CustomDataIndex = 0; CustomDataIndex < 17; CustomDataIndex++ )
		{
			Eyes->SetCustomDataValue( EyeIndex, CustomDataIndex, EyeData.GetCustomDataValue( EyeIndex, CustomDataIndex ) );
		}

		Eyes->SetCustomDataValue( EyeIndex, 19, ( HumanStateData.FaceEffectIndex == 1 ) ? 1.f : 0.f);
		if( HumanStateData.EyesEmissive.IsValidIndex( EyeIndex ) ) Eyes->SetCustomDataValue( EyeIndex, 20, HumanStateData.EyesEmissive[EyeIndex] );
		if( HumanStateData.HighlightStrength.IsValidIndex( EyeIndex ) ) Eyes->SetCustomDataValue( EyeIndex, 21, HumanStateData.HighlightStrength[EyeIndex] );
		if( HumanStateData.PupilScale.IsValidIndex( EyeIndex ) ) Eyes->SetCustomDataValue( EyeIndex, 22, HumanStateData.PupilScale[EyeIndex] );
		
		Eyes->SetCustomDataValue( EyeIndex, 30, HumanStateData.GlobalSphereMaskHardness );

		// Impair numbers are right eye, so we need to reverse the UV.x for the iris (because the eye scale is reversed in x too)
		float ReverseEyeUV = ( EyeIndex % 2 > 0 ) ? 0.f : 1.f;
		
		// if it's the last instance, we set the render state dirty to update all the custom data
		if( EyeIndex == Eyes->GetInstanceCount() - 1 )
		{
			Eyes->SetCustomDataValue( EyeIndex, FEyeData::LeftRightCustomDataIndex, ReverseEyeUV, true );
		}
		else
		{
			Eyes->SetCustomDataValue( EyeIndex, FEyeData::LeftRightCustomDataIndex, ReverseEyeUV );
		}
	}
}

/* ################
	## ON TICK  ##
   ################*/

// Called every frame
void AHuman::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	UpdateEyesAnimation( DeltaTime );

	#if WITH_EDITOR
	DebugAnimationDatasRuntime();
	DebugMorphTargetDatasRuntime();
	#endif
}

void AHuman::UpdateEyesAnimation( float DeltaTime )
{
	if( Eyes->GetInstanceCount() < 1 ) return;

	UAnimInstance* MainBodyAnimInstance = MainBody->GetAnimInstance();
	if( !MainBodyAnimInstance ) return;

	/* Iris position(in - out, up - down) : Lerp between base position (default iris offset) and max position, set in head data
	*/
	FVector2D IrisPositionLeft( HumanBodyData.EyeData.GetCustomDataValue( 0, 6 ), HumanBodyData.EyeData.GetCustomDataValue( 0, 7 ) );
	FVector2D IrisOffsetXLeft = FVector2D::Zero();
	FVector2D IrisOffsetYLeft = FVector2D::Zero();
	float EyeLookInLeft = MainBodyAnimInstance->GetCurveValue( "EyeLookInLeft" );
	float EyeLookOutLeft = MainBodyAnimInstance->GetCurveValue( "EyeLookOutLeft" );
	float EyeLookUpLeft = MainBodyAnimInstance->GetCurveValue( "EyeLookUpLeft" );
	float EyeLookDownLeft = MainBodyAnimInstance->GetCurveValue( "EyeLookDownLeft" );
	if( EyeLookInLeft > 0.f )
	{
		IrisOffsetXLeft = FMath::Lerp( IrisPositionLeft, ActiveHeadMeshData->IrisMaxPositionIn, EyeLookInLeft ) - IrisPositionLeft;
	}
	else if( EyeLookOutLeft > 0.f )
	{
		IrisOffsetXLeft = FMath::Lerp( IrisPositionLeft, ActiveHeadMeshData->IrisMaxPositionOut, EyeLookOutLeft ) - IrisPositionLeft;
	}
	if( EyeLookUpLeft > 0.f )
	{
		IrisOffsetYLeft = FMath::Lerp( IrisPositionLeft, ActiveHeadMeshData->IrisMaxPositionUp, EyeLookUpLeft ) - IrisPositionLeft;
	}
	else if( EyeLookDownLeft > 0.f )
	{
		IrisOffsetYLeft = FMath::Lerp( IrisPositionLeft, ActiveHeadMeshData->IrisMaxPositionDown, EyeLookDownLeft ) - IrisPositionLeft;
	}
	IrisPositionLeft += IrisOffsetXLeft + IrisOffsetYLeft;

	FVector2D IrisPositionRight( HumanBodyData.EyeData.GetCustomDataValue( 1, 6 ), HumanBodyData.EyeData.GetCustomDataValue( 1, 7 ) );
	FVector2D IrisOffsetXRight = FVector2D::Zero();
	FVector2D IrisOffsetYRight = FVector2D::Zero();
	float EyeLookInRight = MainBodyAnimInstance->GetCurveValue( "EyeLookInRight" );
	float EyeLookOutRight = MainBodyAnimInstance->GetCurveValue( "EyeLookOutRight" );
	float EyeLookUpRight = MainBodyAnimInstance->GetCurveValue( "EyeLookUpRight" );
	float EyeLookDownRight = MainBodyAnimInstance->GetCurveValue( "EyeLookDownRight" );
	if( EyeLookInRight > 0.f )
	{
		IrisOffsetXRight = FMath::Lerp( IrisPositionRight, ActiveHeadMeshData->IrisMaxPositionIn, EyeLookInRight ) - IrisPositionRight;
	}
	else if( EyeLookOutRight > 0.f )
	{
		IrisOffsetXRight = FMath::Lerp( IrisPositionRight, ActiveHeadMeshData->IrisMaxPositionOut, EyeLookOutRight ) - IrisPositionRight;
	}
	if( EyeLookUpRight > 0.f )
	{
		IrisOffsetYRight = FMath::Lerp( IrisPositionRight, ActiveHeadMeshData->IrisMaxPositionUp, EyeLookUpRight ) - IrisPositionRight;
	}
	else if( EyeLookDownRight > 0.f )
	{
		IrisOffsetYRight = FMath::Lerp( IrisPositionRight, ActiveHeadMeshData->IrisMaxPositionDown, EyeLookDownRight ) - IrisPositionRight;
	}
	IrisPositionRight += IrisOffsetXRight + IrisOffsetYRight;

	/* Iris scale : if eye is wide, scale down the iris for a scared/suprise effect. Move down the eyelash shadow (Sclera dark corner) when eye is closing
	*/
	float EyeWideLeft = MainBodyAnimInstance->GetCurveValue( "EyeWideLeft" );
	float ScalingClampedLeft = FMath::Max( ( 1 - EyeWideLeft ), HumanBodyData.EyeData.EyesMinimumScale );
	FVector2D IrisScaleLeft( HumanBodyData.EyeData.GetCustomDataValue( 0, 4 ) * ScalingClampedLeft, HumanBodyData.EyeData.GetCustomDataValue( 0, 5 ) * ScalingClampedLeft );

	float EyeWideRight = MainBodyAnimInstance->GetCurveValue( "EyeWideRight" );
	float ScalingClampedRight = FMath::Max( ( 1 - EyeWideRight ), HumanBodyData.EyeData.EyesMinimumScale );
	FVector2D IrisScaleRight( HumanBodyData.EyeData.GetCustomDataValue( 1, 4 ) * ScalingClampedRight, HumanBodyData.EyeData.GetCustomDataValue( 1, 5 ) * ScalingClampedRight );

	/* Highlight strength : when eye blink, animate the eye highlight for some live2d'esque effect. Calculate some velocity and accumulate strength
	*/
	float EyeBlinkLeft = MainBodyAnimInstance->GetCurveValue( "EyeBlinkLeft" );
	float EyeBlinkRight = MainBodyAnimInstance->GetCurveValue( "EyeBlinkRight" );

	if( HumanStateData.LastFrameEyeBlink.Num() == 2 && HumanStateData.HighlightStrength.Num() == 2 )
		{
			float EyelashLeftVelocity = ( HumanStateData.LastFrameEyeBlink[0] - EyeBlinkLeft );
			float EyelashRightVelocity = ( HumanStateData.LastFrameEyeBlink[1] - EyeBlinkRight );

			if( EyelashLeftVelocity > 0.1f )
			{
				HumanStateData.HighlightStrength[0] = FMath::Min( HumanStateData.HighlightStrength[0] + EyelashLeftVelocity, 1 );
			}
			else
			{
				HumanStateData.HighlightStrength[0] = FMath::Max( HumanStateData.HighlightStrength[0] - DeltaTime * 0.60, 0 );
			}
			HumanStateData.LastFrameEyeBlink[0] = EyeBlinkLeft;

			if( EyelashRightVelocity > 0.1f )
			{
				HumanStateData.HighlightStrength[1] = FMath::Min( HumanStateData.HighlightStrength[1] + EyelashRightVelocity, 1 );
			}
			else
			{
				HumanStateData.HighlightStrength[1] = FMath::Max( HumanStateData.HighlightStrength[1] - DeltaTime * 0.60, 0 );
			}

			HumanStateData.LastFrameEyeBlink[1] = EyeBlinkRight;
		}

	bool IsCrying = false;
	if( HumanStateData.FaceEffectIndex == 0.1f ) // Crying effect
	{
		IsCrying = true;
		TearsSM->SetVisibility( true );
	}
	else
	{
		TearsSM->SetVisibility( false );
	}

	/* Dark sclera position : the shadow below the eyelash on the sclera
	*/
	float EyelashPositionLeft = HumanBodyData.EyeData.GetCustomDataValue( 0, 17 ) + EyeWideLeft * HumanBodyData.EyeData.EyelashShadowWeightOpen - EyeBlinkLeft * HumanBodyData.EyeData.EyelashShadowWeightClose;
	float EyelashPositionRight = HumanBodyData.EyeData.GetCustomDataValue( 1, 17 ) + EyeWideRight * HumanBodyData.EyeData.EyelashShadowWeightOpen - EyeBlinkRight * HumanBodyData.EyeData.EyelashShadowWeightClose;

	// Set custom data for all instances
	uint8 NumberOfEyes = Eyes->GetInstanceCount();
	for( uint8 EyeIndex = 0; EyeIndex < NumberOfEyes; EyeIndex++ )
	{
		Eyes->SetCustomDataValue( EyeIndex, 19, (IsCrying) ? HumanBodyData.EyeData.MaxCryingEffect : 0.f );

		if( EyeIndex % 2 > 0 )
		{
			// right eye
			Eyes->SetCustomDataValue( EyeIndex, 4, IrisScaleRight.X );
			Eyes->SetCustomDataValue( EyeIndex, 5, IrisScaleRight.Y );
			Eyes->SetCustomDataValue( EyeIndex, 6, IrisPositionRight.X );
			Eyes->SetCustomDataValue( EyeIndex, 7, IrisPositionRight.Y );
			if( HumanStateData.HighlightStrength.IsValidIndex( 1 ) ) Eyes->SetCustomDataValue( EyeIndex, 21, HumanStateData.HighlightStrength[1] );

			if( EyeIndex == NumberOfEyes - 1 )
			{
				Eyes->SetCustomDataValue( EyeIndex, 17, EyelashPositionRight, true ); // last instance so set render dirty
			}
			else
			{
				Eyes->SetCustomDataValue( EyeIndex, 17, EyelashPositionRight );
			}
		}
		else
		{
			// left eye
			Eyes->SetCustomDataValue( EyeIndex, 4, IrisScaleLeft.X );
			Eyes->SetCustomDataValue( EyeIndex, 5, IrisScaleLeft.Y );
			Eyes->SetCustomDataValue( EyeIndex, 6, IrisPositionLeft.X );
			Eyes->SetCustomDataValue( EyeIndex, 7, IrisPositionLeft.Y );
			if( HumanStateData.HighlightStrength.IsValidIndex( 0 ) )Eyes->SetCustomDataValue( EyeIndex, 21, HumanStateData.HighlightStrength[0] );

			if( EyeIndex == NumberOfEyes - 1 )
			{
				Eyes->SetCustomDataValue( EyeIndex, 17, EyelashPositionLeft, true ); // last instance so set render dirty
			}
			else
			{
				Eyes->SetCustomDataValue( EyeIndex, 17, EyelashPositionLeft );
			}
		}
	}
}

/* ################
	## SETTER ##
   ################*/

void AHuman::SetHumanBodyData( const FHumanBodyData& InHumanBodyData )
{
	HumanBodyData = InHumanBodyData;
}

/* ################
	## GETTER ##
   ################*/

FHumanBodyData& AHuman::GetHumanBodyData()
{
	return HumanBodyData;
}

UModularSkeletalMeshComponent* AHuman::GetHeadComponent() const
{
	for( int32 ComponentIndex = 0; ComponentIndex < SkeletalComponents.Num(); ComponentIndex++ )
	{
		if( SkeletalComponents[ComponentIndex]->GetFName() == UEnum::GetValueAsName( EBodyPartType::Head ) ) return SkeletalComponents[ComponentIndex];
	}

	return nullptr;
}

UModularSkeletalMeshComponent* AHuman::GetMainBodyComponent() const
{
	return MainBody;
}

UModularSkeletalMeshComponent* AHuman::GetBodyComponent( EBodyPartType BodyPartType ) const
{
	return *(SkeletalComponents.FindByPredicate([BodyPartType]( UModularSkeletalMeshComponent* ModularSkelComp ){ return ModularSkelComp->GetFName() == UEnum::GetValueAsName( BodyPartType ); }));
}

/* ################
	## EFFECTS ##
   ################*/

void AHuman::SetGlobalWetness( float GlobalWetnessOpacity )
{
	HumanStateData.WetnessGlobalOpacity = GlobalWetnessOpacity;
}

void AHuman::SetIsUnderRoof( bool IsUnderRoof )
{
	HumanStateData.bIsUnderRoof = IsUnderRoof;
}

void AHuman::SetHeightWetness( float HeightWetness, float HeightWetnessOpacity )
{
	HumanStateData.WetnessZHeight = HeightWetness;
	HumanStateData.WetnessZOpacity = HeightWetnessOpacity;

	MainBody->SetScalarParameterValueOnMaterials( TEXT("WetnessZHeight"), HeightWetness );

	for(UModularSkeletalMeshComponent* SkelComp : SkeletalComponents )
	{
		SkelComp->SetScalarParameterValueOnMaterials( TEXT("WetnessZHeight"), HeightWetness );
	}
}

void AHuman::SetHeightMaskGlobal( float HeightMask )
{
	if( HumanStateData.GlobalMaskHeight == HeightMask ) return;

	HumanStateData.GlobalMaskHeight = HeightMask;

	FName ParameterName = TEXT( "MaskingHeightMask" );

	MainBody->SetScalarParameterValueOnMaterials( ParameterName, HeightMask );

	for( UModularSkeletalMeshComponent* SkeletalMeshComponent : SkeletalComponents )
	{
		SkeletalMeshComponent->SetScalarParameterValueOnMaterials( ParameterName, HeightMask );
	}

	int32 NumberOfEyes = Eyes->GetInstanceCount();

	if( NumberOfEyes <= 0 ) return;

	for( int32 EyeIndex = 0; EyeIndex < NumberOfEyes; EyeIndex++ )
	{
		Eyes->SetCustomDataValue( EyeIndex, 25, HeightMask );
	}
}

void AHuman::SetSphereMaskGlobal( FVector4 SphereMaskPosition, float SphereMaskRadius )
{
	if( HumanStateData.GlobalSphereMaskPosition == SphereMaskPosition && HumanStateData.GlobalSphereMaskRadius == SphereMaskRadius ) return;

	HumanStateData.GlobalSphereMaskPosition = SphereMaskPosition;
	HumanStateData.GlobalSphereMaskRadius = SphereMaskRadius;
	FName ParameterNamePosition = TEXT( "MaskingSphereMaskPosition" );
	FName ParameterNameRadius = TEXT( "MaskingSphereMaskRadius" );

	MainBody->SetVectorParameterValueOnMaterials( ParameterNamePosition, SphereMaskPosition );
	MainBody->SetScalarParameterValueOnMaterials( ParameterNameRadius, SphereMaskRadius );

	for( UModularSkeletalMeshComponent* SkeletalMeshComponent : SkeletalComponents )
	{
		SkeletalMeshComponent->SetVectorParameterValueOnMaterials( ParameterNamePosition, SphereMaskPosition );
		SkeletalMeshComponent->SetScalarParameterValueOnMaterials( ParameterNameRadius, SphereMaskRadius );
	}


	int32 NumberOfEyes = Eyes->GetInstanceCount();

	if( NumberOfEyes <= 0 ) return;

	for( int32 EyeIndex = 0; EyeIndex < NumberOfEyes; EyeIndex++ )
	{
		Eyes->SetCustomDataValue( EyeIndex, 26, SphereMaskPosition.X );
		Eyes->SetCustomDataValue( EyeIndex, 27, SphereMaskPosition.Y );
		Eyes->SetCustomDataValue( EyeIndex, 28, SphereMaskPosition.Z );
		Eyes->SetCustomDataValue( EyeIndex, 29, SphereMaskRadius );
	}
}

void AHuman::SetSphereMaskHardness( float SphereMaskHardness )
{
	if( HumanStateData.GlobalSphereMaskHardness == SphereMaskHardness ) return;

	HumanStateData.GlobalSphereMaskHardness = SphereMaskHardness;
	FName ParameterNamePosition = TEXT( "MaskingSphereMaskHardness" );

	MainBody->SetScalarParameterValueOnMaterials( ParameterNamePosition, SphereMaskHardness );

	for( UModularSkeletalMeshComponent* SkeletalMeshComponent : SkeletalComponents )
	{
		SkeletalMeshComponent->SetScalarParameterValueOnMaterials( ParameterNamePosition, SphereMaskHardness );
	}

	int32 NumberOfEyes = Eyes->GetInstanceCount();

	if( NumberOfEyes <= 0 ) return;

	for( int32 EyeIndex = 0; EyeIndex < NumberOfEyes; EyeIndex++ )
	{
		Eyes->SetCustomDataValue( EyeIndex , 30, SphereMaskHardness );
	}
}

void AHuman::SetReverseSphereMaskGlobal( bool IsMaskReverse )
{
	if( HumanStateData.bReverseGlobalSphereMask == IsMaskReverse ) return;

	HumanStateData.bReverseGlobalSphereMask = IsMaskReverse;

	float ReverseMaskFloat = ( IsMaskReverse ) ? 1.f : 0.f;
	FName ParameterName = TEXT( "MaskingReverseSphereMask" );

	MainBody->SetScalarParameterValueOnMaterials( ParameterName, ReverseMaskFloat );

	for( UModularSkeletalMeshComponent* SkeletalMeshComponent : SkeletalComponents )
	{
		SkeletalMeshComponent->SetScalarParameterValueOnMaterials( ParameterName, ReverseMaskFloat );
	}

	int32 NumberOfEyes = Eyes->GetInstanceCount();

	if( NumberOfEyes <= 0 ) return;

	for( int32 EyeIndex = 0; EyeIndex < NumberOfEyes; EyeIndex++ )
	{
		Eyes->SetCustomDataValue( EyeIndex, 31, IsMaskReverse );
	}
}

void AHuman::SetReverseHeightMaskGlobal( bool IsMaskReverse )
{
	if( HumanStateData.bReverseGlobalHeightMask == IsMaskReverse ) return;

	HumanStateData.bReverseGlobalHeightMask = IsMaskReverse;

	float ReverseMaskFloat = ( IsMaskReverse ) ? 1.f : 0.f;
	FName ParameterName = TEXT( "MaskingReverseHeightMask" );

	MainBody->SetScalarParameterValueOnMaterials( ParameterName, ReverseMaskFloat );

	for( UModularSkeletalMeshComponent* SkeletalMeshComponent : SkeletalComponents )
	{
		SkeletalMeshComponent->SetScalarParameterValueOnMaterials( ParameterName, ReverseMaskFloat );
	}

	int32 NumberOfEyes = Eyes->GetInstanceCount();

	if( NumberOfEyes <= 0 ) return;

	for( int32 EyeIndex = 0; EyeIndex < NumberOfEyes; EyeIndex++ )
	{
		Eyes->SetCustomDataValue( EyeIndex, 31, IsMaskReverse );
	}
}

void AHuman::SetFaceEffect( float EffectIndex )
{
	HumanStateData.FaceEffectIndex = EffectIndex;
}

void AHuman::SetEyesEmissive( float EmissiveCoef, TArray<int32>& EyesIndex )
{
	UModularSkeletalMeshComponent* HeadComponent = GetHeadComponent();
	if( HeadComponent && ActiveHeadMeshData )
	{
		uint8 NumberOfEyes = Eyes->GetInstanceCount();

		if( NumberOfEyes <= 0 ) return;

		for(float EyeIndex : EyesIndex )
		{
			if( HumanStateData.EyesEmissive.IsValidIndex( EyeIndex ) )
			{
				if( HumanStateData.EyesEmissive[EyeIndex] == EmissiveCoef ) continue;

				HumanStateData.EyesEmissive[EyeIndex] = EmissiveCoef;

				if( Eyes->IsValidInstance( EyeIndex ) )
				{
					#if WITH_EDITOR
					Eyes->SetCustomDataValue( EyeIndex, 20, EmissiveCoef, true );
					#else
					Eyes->SetCustomDataValue( EyeIndex, 20, EmissiveCoef );
					#endif
				}
			}
		}
	}
}

void AHuman::SetEyesBleeding( float BleedingCoef, TArray<int32>& EyesIndex )
{
	uint8 NumberOfEyes = Eyes->GetInstanceCount();

	if( NumberOfEyes <= 0 ) return;

	for( int32 EyeIndex : EyesIndex )
	{
		if( HumanStateData.EyesBleeding.IsValidIndex( EyeIndex ) && HumanBodyData.EyeData.ScleraColorBleeding.IsValidIndex( EyeIndex ) )
		{
			if( HumanStateData.EyesBleeding[EyeIndex] == BleedingCoef || HumanBodyData.EyeData.ScleraColorBleeding[EyeIndex] == HumanBodyData.EyeData.ScleraColor[EyeIndex] ) continue;

			HumanStateData.EyesBleeding[EyeIndex] = BleedingCoef;

			FLinearColor ColorFinal = FMath::Lerp( HumanBodyData.EyeData.ScleraColor[EyeIndex], HumanBodyData.EyeData.ScleraColorBleeding[EyeIndex], BleedingCoef );

			if( Eyes->IsValidInstance( EyeIndex ) )
			{
				#if WITH_EDITOR
				Eyes->SetCustomDataValue( EyeIndex, 8, ColorFinal.R );
				Eyes->SetCustomDataValue( EyeIndex, 9, ColorFinal.G );
				Eyes->SetCustomDataValue( EyeIndex, 10, ColorFinal.B );
				Eyes->SetCustomDataValue( EyeIndex, 11, BleedingCoef, true );
				#else
				Eyes->SetCustomDataValue( EyeIndex, 8, ColorFinal.R );
				Eyes->SetCustomDataValue( EyeIndex, 9, ColorFinal.G );
				Eyes->SetCustomDataValue( EyeIndex, 10, ColorFinal.B );
				Eyes->SetCustomDataValue( EyeIndex, 11, BleedingCoef );
				#endif
			}
		}
	}
}

void AHuman::SetPupilScale( float PupilScale, TArray<int32>& EyesIndex )
{
	uint8 NumberOfEyes = Eyes->GetInstanceCount();

	if( NumberOfEyes <= 0 ) return;

	for( float EyeIndex : EyesIndex )
	{
		if( HumanStateData.PupilScale.IsValidIndex( EyeIndex ) )
		{
			if( HumanStateData.PupilScale[EyeIndex] == PupilScale ) continue;
			HumanStateData.PupilScale[EyeIndex] = PupilScale;

			if( Eyes->IsValidInstance( EyeIndex ) ){
				#if WITH_EDITOR
				Eyes->SetCustomDataValue( EyeIndex, 22, PupilScale, true );
				#else
				Eyes->SetCustomDataValue( EyeIndex, 22, PupilScale );
				#endif
			}
		}
	}
}

void AHuman::SetHighlightStrength( float HighlightStrength, TArray<int32>& EyesIndex )
{
	uint8 NumberOfEyes = Eyes->GetInstanceCount();

	if( NumberOfEyes <= 0 ) return;

	for( float EyeIndex : EyesIndex )
	{
		if( HumanStateData.HighlightStrength.IsValidIndex( EyeIndex ) )
		{
			if( HumanStateData.HighlightStrength[EyeIndex] == HighlightStrength ) continue;

			HumanStateData.HighlightStrength[EyeIndex] = HighlightStrength;

			if( Eyes->IsValidInstance( EyeIndex ) )
			{
				#if WITH_EDITOR
				Eyes->SetCustomDataValue( EyeIndex, 21, HighlightStrength, true );
				#else
				Eyes->SetCustomDataValue( EyeIndex, 21, HighlightStrength );
				#endif
			}
		}
	}
}

#if WITH_EDITORONLY_DATA

void AHuman::UpdateMainBodyAnimation_Editor( UAnimSequence* AnimationToPlay, float InAnimPlayRate, FName InIphoneName )
{
	MainBodyAnimation_Editor = AnimationToPlay;
	AnimationPlayRate_Editor = InAnimPlayRate;
	SetArkitName_Editor( InIphoneName );
	SetEditorAnimationBP_Editor( AnimationBP_Editor );
}

void AHuman::SetEditorAnimationBP_Editor( TSubclassOf<UAnimInstance> InAnimationBP )
{
	AnimationBP_Editor = InAnimationBP;

	if( AnimationBP_Editor )
	{
		MainBody->SetAnimInstanceClass( AnimationBP_Editor );
	}
	else
	{
		UMainBodyMeshData* MainBodyData = Cast<UMainBodyMeshData>( HumanBodyData.MainBody.MeshData );
		if( MainBodyData )
		{
			MainBody->SetAnimInstanceClass( MainBodyData->BodyAnimationBlueprint );
		}
	}
}

void AHuman::DebugAnimationDatasRuntime()
{
	if( !DebugBodyArmCurves && !DebugBodyLegsCurves ) return;

	GEngine->AddOnScreenDebugMessage(0, 5, FColor::Cyan, TEXT("--- Starting body curves debug ---"));

	UAnimInstance* AnimInstance = MainBody->GetAnimInstance();
	if( !AnimInstance ) return GEngine->AddOnScreenDebugMessage( 0, 5, FColor::Red, TEXT( "Error: no animation instance..." ) );

	if( DebugBodyArmCurves )
	{
		TArray<FName> ArmsBoneNames;
		FName ArrBone[] = { TEXT( "clavicle_l" ),
		TEXT( "upperarm_l" ),
		TEXT( "lowerarm_l" ),
		TEXT( "hand_l" ),
		TEXT( "clavicle_r" ),
		TEXT( "upperarm_r" ),
		TEXT( "lowerarm_r" ),
		TEXT( "hand_r" )};

		ArmsBoneNames.Append( ArrBone, 8 );

		TArray<FName> ActiveCurveNames;
		AnimInstance->GetActiveCurveNames( EAnimCurveType::AttributeCurve, ActiveCurveNames );

		for( uint8 Index = 0; Index < ArmsBoneNames.Num(); Index++ )
		{
			FName BoneName = ArmsBoneNames[Index];
			int32 NameLength = ArmsBoneNames[Index].GetStringLength();

			GEngine->AddOnScreenDebugMessage( Index + 1, 5, FColor::Cyan, FString::Printf( TEXT( "#### Bone %s ####" ), *BoneName.ToString() ) );
			FString DebugMessageMorph = TEXT( "- " );
			FString DebugMessageCurve = TEXT( "- " );

			for( uint8 IndexCurve = 0; IndexCurve < ActiveCurveNames.Num(); IndexCurve++ )
			{
				FName CurveName = ActiveCurveNames[IndexCurve];

				if( CurveName.ToString().StartsWith( BoneName.ToString() ) )
				{
					if( CurveName.ToString().EndsWith( TEXT( "_morph" ) ) )
					{
						if( AnimInstance->GetCurveValue( CurveName ) != 0.f )
						{
							DebugMessageMorph.Append( FString::Printf( TEXT( "%s : %f  ||  " ), *CurveName.ToString(), AnimInstance->GetCurveValue( CurveName ) ) );
						}
					}
					else
					{
						DebugMessageCurve.Append( FString::Printf( TEXT( "%s : %f  ||  " ), *CurveName.ToString(), AnimInstance->GetCurveValue( CurveName ) ) );
					}
				}
			}
			GEngine->AddOnScreenDebugMessage( ( Index + 1 ) * 100, 5, FColor::Cyan, DebugMessageCurve );
			GEngine->AddOnScreenDebugMessage( ( Index + 1 ) * 100 + 1, 5, FColor::Cyan, DebugMessageMorph );
		}
	}

	if( DebugBodyLegsCurves )
	{
		TArray<FName> LegsBoneNames;
		FName ArrBone[] = { TEXT( "thigh_l" ),
		TEXT( "calf_l" ),
		TEXT( "foot_l" ),
		TEXT( "ball_l" ),
		TEXT( "thigh_r" ),
		TEXT( "calf_r" ),
		TEXT( "foot_r" ),
		TEXT( "ball_r" )};

		LegsBoneNames.Append( ArrBone, 8 );

		TArray<FName> ActiveCurveNames;
		AnimInstance->GetActiveCurveNames( EAnimCurveType::AttributeCurve, ActiveCurveNames );

		for( uint8 Index = 0; Index < LegsBoneNames.Num(); Index++ )
		{
			FName BoneName = LegsBoneNames[Index];
			int32 NameLength = LegsBoneNames[Index].GetStringLength();

			GEngine->AddOnScreenDebugMessage( Index + 1, 5, FColor::Cyan, FString::Printf( TEXT( "#### Bone %s ####" ), *BoneName.ToString() ) );
			FString DebugMessageMorph = TEXT( "- " );
			FString DebugMessageCurve = TEXT( "- " );

			for( uint8 IndexCurve = 0; IndexCurve < ActiveCurveNames.Num(); IndexCurve++ )
			{
				FName CurveName = ActiveCurveNames[IndexCurve];

				if( CurveName.ToString().StartsWith( BoneName.ToString() ) )
				{
					if( CurveName.ToString().EndsWith( TEXT( "_morph" ) ) )
					{
						if( AnimInstance->GetCurveValue( CurveName ) != 0.f )
						{
							DebugMessageMorph.Append( FString::Printf( TEXT( "%s : %f  ||  " ), *CurveName.ToString(), AnimInstance->GetCurveValue( CurveName ) ) );
						}
					}
					else
					{
						DebugMessageCurve.Append( FString::Printf( TEXT( "%s : %f  ||  " ), *CurveName.ToString(), AnimInstance->GetCurveValue( CurveName ) ) );
					}
				}
			}
			GEngine->AddOnScreenDebugMessage( ( Index + 1 ) * 100, 5, FColor::Cyan, DebugMessageCurve );
			GEngine->AddOnScreenDebugMessage( ( Index + 1 ) * 100 + 1, 5, FColor::Cyan, DebugMessageMorph );
		}
	}
}

void AHuman::DebugMorphTargetDatasRuntime()
{
	if(!DebugBodyFaceCurves) return;

	USkeletalMeshComponent* HeadComponent = GetHeadComponent();
	if( HeadComponent )
	{
		GEngine->AddOnScreenDebugMessage( 0, 5, FColor::Cyan, TEXT( "--- Starting morphtarget debug ---" ) );

		UAnimInstance* AnimationInstance = HeadComponent->GetAnimInstance();
		if( !AnimationInstance ) return GEngine->AddOnScreenDebugMessage( 0, 5, FColor::Red, TEXT( "ANIMATION INSTANCE DOESN'T EXIST" ) );

		TArray<FName> CurvesName;
		AnimationInstance->GetActiveCurveNames( EAnimCurveType::MorphTargetCurve, CurvesName );

		TArray<FString> BoneNames;
		FString ArrBone[] = { TEXT( "clavicle" ),
			TEXT( "upperarm" ),
			TEXT( "lowerarm" ),
			TEXT( "hand" ),
			TEXT( "thigh" ),
			TEXT( "calf" ),
			TEXT( "foot" ),
			TEXT( "ball" ),
			TEXT( "Legs" ),
			TEXT( "Body" ),
			TEXT( "Arm" ),
			TEXT( "Boobs" )};

		BoneNames.Append( ArrBone, 12 );

		uint8 Index = 1;
		for( FName CurveName : CurvesName )
		{
			bool IsFaceCurve = true;

			for( FString BoneName : BoneNames )
			{
				if( CurveName.ToString().StartsWith( BoneName ) ) IsFaceCurve = false;
			}

			if( IsFaceCurve && CurveName == TEXT("CatMouth") )
			{
				FString DebugMessageMorph = TEXT( "- " );
				DebugMessageMorph.Append( FString::Printf( TEXT( "%s : %f" ), *CurveName.ToString(), AnimationInstance->GetCurveValue( CurveName ) ) );
				GEngine->AddOnScreenDebugMessage( Index, 5, FColor::Cyan, DebugMessageMorph );
				Index++;
			}
		}
	}
}

#endif // WITH_EDITORONLY_DATA