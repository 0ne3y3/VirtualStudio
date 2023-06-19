// Fill out your copyright notice in the Description page of Project Settings.


#include "Human.h"
#include "ModularSkeletalMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC( LogCharacter, Log, All );

FName AHuman::MainBodyName(TEXT("MainBodyMesh"));
FName AHuman::EyesName( TEXT( "EyesMesh" ) );

AHuman::AHuman( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	bAllowTickBeforeBeginPlay = false;
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "CharacterScene" ) );
	RootComponent->SetComponentTickEnabled(false);

	MainBody = CreateDefaultSubobject<USkeletalMeshComponent>( AHuman::MainBodyName );
	MainBody->SetupAttachment( RootComponent );
	MainBody->bPropagateCurvesToFollowers = true;
	//MainBody->SetComponentTickEnabled( false );
	/* Apply all default parameter, see later what to set as project goes */

	Eyes = CreateDefaultSubobject<UInstancedStaticMeshComponent>( AHuman::EyesName );
	//Eyes->SetComponentTickEnabled( false );
}

void AHuman::BeginPlay()
{
	Super::BeginPlay();

	ResetFirstInit();
	OnConstruction( FTransform () );
}

void AHuman::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	Super::EndPlay( EndPlayReason );

}

// Called every frame
void AHuman::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	UpdateHeadAnimation();

	UpdateEyesAnimation();

	#if WITH_EDITOR
	DebugAnimationDatasRuntime();
	DebugMorphTargetDatasRuntime();
	#endif
}

void AHuman::OnConstruction( const FTransform& Transform )
{
	Super::OnConstruction( Transform );

	if( !bIsFirstInit ) return;

	UE_LOG( LogCharacter, Log, TEXT( "###### ON CONSTRUCTION START %s ######" ), *GetName() );
	UE_LOG( LogCharacter, Log, TEXT( "- Preset name: %s" ), *HumanBodyData.PresetName.ToString() );

	if( HumanBodyData.MainBody.MeshData != nullptr)
	{
		UE_LOG( LogCharacter, Log, TEXT("###### SETUP MAIN BODY #####") );
		if(!SetupMainBody( HumanBodyData.MainBody.MeshData )) return;

		UE_LOG( LogCharacter, Log, TEXT( "###### SETUP SKELETAL MESH COMPONENTS #####" ) );
		// setup SkeletalMeshModular
		SetupSkeletalMeshComponents( HumanBodyData.SkeletalMeshes );

		UE_LOG( LogCharacter, Log, TEXT( "###### SETUP STATIC MESH COMPONENTS #####" ) );
		// setup Accessory

		// setup eyes if head
		UE_LOG( LogCharacter, Log, TEXT( "###### SETUP EYES #####" ) );
		if( HeadMeshData != nullptr)
		{
			SetHeadAnimationData( HeadMeshData->AnimationData );
			SetupEyes( HeadMeshData->NumberOfEyes, GetHeadComponent() );
		}
		else
		{
			UE_LOG( LogCharacter, Log, TEXT( "- No head data found, discard eyes setup." ) );
			UpdateNumberOfEyeInstance(0, MainBody);
		}
	}

	bIsFirstInit = false;

	#if WITH_EDITOR
	MainBody->SetUpdateAnimationInEditor( true );
	for(USkeletalMeshComponent* ChildComponent : SkeletalComponents )
	{
		ChildComponent->SetUpdateAnimationInEditor( true );
	}
	#endif

	UE_LOG( LogCharacter, Log, TEXT( "###### ON CONSTRUCTION END %s ######" ), *GetName() );
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

bool AHuman::SetupMainBody( USkeletalMeshData* MeshData )
{
	UMainBodyMeshData* MainBodyData = Cast<UMainBodyMeshData>( MeshData );

	if( MainBodyData )
	{
		USkeletalMesh* SkeletalMesh = MainBodyData->Mesh.LoadSynchronous();

		if( SkeletalMesh )
		{
			UE_LOG( LogCharacter, Log, TEXT( "- Skeletal mesh found and loaded." ) );
			MainBody->SetSkeletalMesh( SkeletalMesh );
			UpdateMainBodyMaterials();

			TArray<float> CustomFaceDatas = HumanBodyData.SkinAndFaceCustomData.CombineBodyData();
			UpdateMainBodyCustomData( CustomFaceDatas );

			// TODO: Temporary set all body mask to default, later make function updateMainBodyMask
			MainBody->SetCustomPrimitiveDataFloat( 22, 0.f );
			MainBody->SetCustomPrimitiveDataFloat( 23, 1.01f );
			MainBody->SetCustomPrimitiveDataFloat( 24, 0.f );
			MainBody->SetCustomPrimitiveDataFloat( 25, 1.01f );
			MainBody->SetCustomPrimitiveDataFloat( 26, 1.01f );
			MainBody->SetCustomPrimitiveDataFloat( 27, 1.01f );
			MainBody->SetCustomPrimitiveDataFloat( 28, 1.01f );

			#if WITH_EDITOR
			MainBody->SetDefaultCustomPrimitiveDataFloat( 22, 0.f );
			MainBody->SetDefaultCustomPrimitiveDataFloat( 23, 1.01f );
			MainBody->SetDefaultCustomPrimitiveDataFloat( 24, 0.f );
			MainBody->SetDefaultCustomPrimitiveDataFloat( 25, 1.01f );
			MainBody->SetDefaultCustomPrimitiveDataFloat( 26, 1.01f );
			MainBody->SetDefaultCustomPrimitiveDataFloat( 27, 1.01f );
			MainBody->SetDefaultCustomPrimitiveDataFloat( 28, 1.01f );
			#endif
						
			#if WITH_EDITOR
			if( EditorAnimationBP )
			{
				MainBody->SetAnimInstanceClass( EditorAnimationBP );
			}
			else
			{
				MainBody->SetAnimInstanceClass( MainBodyData->BodyAnimationBlueprint );
			}
			#else
			MainBody->SetAnimInstanceClass( MainBodyData->BodyAnimationBlueprint );
			#endif
			
			MainBodyPP = MainBodyData->PPAnimationData;

			return true;
		}
	}

	return false;
}

void AHuman::UpdateMainBodyMaterials()
{
	if( !HumanBodyData.MainBody.MeshData && !MainBody->GetSkeletalMeshAsset() ) return;

	int32 MeshMaterialNum = MainBody->GetSkeletalMeshAsset()->GetNumMaterials();

	bool bUseCustomMaterial = HumanBodyData.MainBody.bUseCustomMaterial;
	UE_LOG( LogCharacter, Log, TEXT( "  * Use %s material" ), ( bUseCustomMaterial ? TEXT("custom") : TEXT("default") ) );

	TArray<TSoftObjectPtr<UMaterialInstance>>& CustomMaterials = HumanBodyData.MainBody.CustomMaterials;
	TArray<TSoftObjectPtr<UMaterialInstance>>& DefaultMaterials = HumanBodyData.MainBody.MeshData->DefaultMaterials;

	for( uint8 MaterialIndex = 0; MaterialIndex < MeshMaterialNum; MaterialIndex++ )
	{
		UMaterialInstance* Material;
		if( bUseCustomMaterial && CustomMaterials.IsValidIndex(MaterialIndex) )
		{
			Material = CustomMaterials[MaterialIndex].LoadSynchronous();
			if( Material )
			{
				MainBody->SetMaterial( MaterialIndex, Material );
				continue;
			}
		}

		if( DefaultMaterials.IsValidIndex( MaterialIndex ) )
		{
			Material = DefaultMaterials[MaterialIndex].LoadSynchronous();
			if( Material ) MainBody->SetMaterial( MaterialIndex, Material );
		}
	}
}

void AHuman::UpdateMainBodyCustomData( TArray<float>& CustomDatas )
{
	for( int32 DataIndex = 0; DataIndex < CustomDatas.Num(); DataIndex++ )
	{
		MainBody->SetCustomPrimitiveDataFloat( DataIndex + FSkinFaceCustomData::StartingIndexSkin, CustomDatas[DataIndex] );
	}

	// Only set default CustomPrimitiveData in editor because for runtime it's useless
	#if WITH_EDITOR
	for( int32 DataIndex = 0; DataIndex < CustomDatas.Num(); DataIndex++ )
	{
		MainBody->SetDefaultCustomPrimitiveDataFloat( DataIndex + FSkinFaceCustomData::StartingIndexSkin, CustomDatas[DataIndex] );
	}
	#endif

	UpdateAllMainBodyMorphology();
}

void AHuman::UpdateAllMainBodyMorphology()
{
	UpdateSkeletalMeshBodyMorphology (MainBody, EBodyMorphCustomData::Boobs, EBodyPartType::MainBody);
	UpdateSkeletalMeshBodyMorphology( MainBody, EBodyMorphCustomData::BodyMuscle, EBodyPartType::MainBody );
	UpdateSkeletalMeshBodyMorphology( MainBody, EBodyMorphCustomData::LegsMuscle, EBodyPartType::MainBody );
}

bool AHuman::SetupEyes( int32 NumberOfEyes, USkeletalMeshComponent* HeadComponent )
{
	UE_LOG( LogCharacter, Log, TEXT( "- Head preset: %s" ), *HeadMeshData->PresetName.ToString() );
	UE_LOG( LogCharacter, Log, TEXT( " * Head component %s" ), ( HeadComponent == nullptr ? TEXT("is nullptr") : TEXT("isn't nullptr") ));
	if( !HeadMeshData ) return false;

	UE_LOG( LogCharacter, Log, TEXT( " * Number of eyes: %d" ), NumberOfEyes );
	if( NumberOfEyes < 1 ) return true;

	UStaticMesh* EyeMesh = HeadMeshData->EyeMesh.LoadSynchronous();
	if( EyeMesh )
	{
		UE_LOG( LogCharacter, Log, TEXT( "- Eye mesh found and loaded." ) );
		Eyes->AttachToComponent( (HeadComponent) ? HeadComponent : MainBody, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT( "head" ) );
		Eyes->SetStaticMesh( EyeMesh );
		
		UpdateEyesMaterial(HumanBodyData.EyeCustomData.MaterialIndex);

		Eyes->NumCustomDataFloats = FEyeCustomData::MaxCustomData;

		UpdateNumberOfEyeInstance( NumberOfEyes, HeadComponent );

		UpdateEyeDefaultPosition();

		UpdateEyesCustomData( HumanBodyData.EyeCustomData, NumberOfEyes );
		
		return true;
	}

	return false;
}

void AHuman::UpdateNumberOfEyeInstance( int32 NumberOfEyes, USkeletalMeshComponent* HeadComponent)
{
	if( !HeadComponent || !HeadMeshData) return;

	int32 EyeLimit = FMath::Max( NumberOfEyes, Eyes->GetInstanceCount() );

	for( uint8 EyeIndex = 0; EyeIndex < EyeLimit; EyeIndex++ )
	{
		if( EyeIndex < NumberOfEyes )
		{
			if( !HeadMeshData->EyesTransform.IsValidIndex( EyeIndex ) ) continue;

			if( Eyes->IsValidInstance( EyeIndex ) )
			{
				Eyes->UpdateInstanceTransform( EyeIndex, HeadMeshData->EyesTransform[EyeIndex], false);
			}
			else
			{
				Eyes->AddInstance( HeadMeshData->EyesTransform[EyeIndex], false );
			}
		}
		else
		{
			Eyes->RemoveInstance( EyeIndex );
		}
	}

	HumanStateData.EyesBleeding.Empty();
	HumanStateData.PupilScale.Empty();
	HumanStateData.EyesEmissive.Empty();
	for(int32 Index = 0; Index < NumberOfEyes; Index++ )
	{
		HumanStateData.EyesBleeding.Add( 0 );
		HumanStateData.PupilScale.Add( 1 );
		HumanStateData.EyesEmissive.Add( 0 );
	}
}

void AHuman::UpdateEyesMaterial(int32 MaterialIndex)
{
	if( HeadMeshData )
	{
		UMaterialInstance* Material = nullptr;
		if( HeadMeshData->EyeMaterials.IsValidIndex( MaterialIndex ))
		{
			Material = HeadMeshData->EyeMaterials[MaterialIndex].LoadSynchronous();
		}
		else
		{
			if( HeadMeshData->EyeMaterials.IsValidIndex(0) )
			{
				Material = HeadMeshData->EyeMaterials[0].LoadSynchronous();
			}
		}

		if( Material )
		{
			Eyes->SetMaterial(0, Material );
		}
	}
}

void AHuman::UpdateEyesCustomData( FEyeCustomData& EyeData, int32 NumberOfEyes )
{
	for( uint8 EyeIndex = 0; EyeIndex < NumberOfEyes; EyeIndex++ )
	{
		for( uint8 CustomDataIndex = 0; CustomDataIndex < Eyes->NumCustomDataFloats; CustomDataIndex++ )
		{
			Eyes->SetCustomDataValue( EyeIndex, CustomDataIndex, EyeData.GetCustomDataValue( EyeIndex, CustomDataIndex ) );
		}

		// Impair numbers are right eye, so we need to reverse the UV.x for the iris (because the socket transform is reversed too)
		if( EyeIndex % 2 > 0 )
		{
			// right eye
			Eyes->SetCustomDataValue( EyeIndex, FEyeCustomData::LeftRightCustomDataIndex, 1.0f );
		}
		else
		{
			// left eye
			Eyes->SetCustomDataValue( EyeIndex, FEyeCustomData::LeftRightCustomDataIndex, 0.0f );
		}

		// if it's the last instance, we set the render state dirty to update all the custom data
		if( EyeIndex == NumberOfEyes - 1 )
		{
			Eyes->SetCustomDataValue( EyeIndex, FEyeCustomData::MaxCustomData - 1, 1.0f, true );
		}
		else
		{
			Eyes->SetCustomDataValue( EyeIndex, FEyeCustomData::MaxCustomData - 1, 1.0f );
		}
	}
}

void AHuman::UpdateEyeDefaultPosition()
{
	if( HeadMeshData )
	{
		for( uint8 EyeIndex = 0; EyeIndex < HumanBodyData.EyeCustomData.IrisOffset.Num(); EyeIndex++ )
		{
			if( HumanBodyData.EyeCustomData.IrisOffset.IsValidIndex( EyeIndex ) ) HumanBodyData.EyeCustomData.IrisOffset[EyeIndex] = HeadMeshData->IrisDefaultPosition;
		}
	}
}

void AHuman::UpdateEyesAnimation()
{
	USkeletalMeshComponent* HeadComponent = GetHeadComponent();
	
	if( HeadComponent && HeadMeshData )
	{
		if( HeadMeshData->NumberOfEyes <= 0 ) return;

		UAnimInstance* HeadAnimInstance = HeadComponent->GetAnimInstance();
		if(!HeadAnimInstance ) return;

		/* Iris position(in - out, up - down) : Lerp between base position (default iris offset) and max position, set in head data
		*/
		FVector2D IrisPositionLeft( HumanBodyData.EyeCustomData.GetCustomDataValue( 0, 6 ), HumanBodyData.EyeCustomData.GetCustomDataValue( 0, 7 ) );
		FVector2D IrisOffsetXLeft = FVector2D::Zero();
		FVector2D IrisOffsetYLeft = FVector2D::Zero();
		float EyeLookInLeft = HeadAnimInstance->GetCurveValue( "EyeLookInLeft" );
		float EyeLookOutLeft = HeadAnimInstance->GetCurveValue( "EyeLookOutLeft" );
		float EyeLookUpLeft = HeadAnimInstance->GetCurveValue( "EyeLookUpLeft" );
		float EyeLookDownLeft = HeadAnimInstance->GetCurveValue( "EyeLookDownLeft" );
		if( EyeLookInLeft > 0.f )
		{
			IrisOffsetXLeft = FMath::Lerp( IrisPositionLeft, HeadMeshData->IrisMaxPositionIn, EyeLookInLeft ) - IrisPositionLeft;
		}
		else if( EyeLookOutLeft > 0.f )
		{
			IrisOffsetXLeft = FMath::Lerp( IrisPositionLeft, HeadMeshData->IrisMaxPositionOut, EyeLookOutLeft ) - IrisPositionLeft;
		}
		if( EyeLookUpLeft > 0.f )
		{
			IrisOffsetYLeft = FMath::Lerp( IrisPositionLeft, HeadMeshData->IrisMaxPositionUp, EyeLookUpLeft ) - IrisPositionLeft;
		}
		else if( EyeLookDownLeft > 0.f )
		{
			IrisOffsetYLeft = FMath::Lerp( IrisPositionLeft, HeadMeshData->IrisMaxPositionDown, EyeLookDownLeft ) - IrisPositionLeft;
		}

		IrisPositionLeft += IrisOffsetXLeft + IrisOffsetYLeft;

		FVector2D IrisPositionRight( HumanBodyData.EyeCustomData.GetCustomDataValue( 1, 6 ), HumanBodyData.EyeCustomData.GetCustomDataValue( 1, 7 ) );
		FVector2D IrisOffsetXRight = FVector2D::Zero();
		FVector2D IrisOffsetYRight = FVector2D::Zero();
		float EyeLookInRight = HeadAnimInstance->GetCurveValue( "EyeLookInRight" );
		float EyeLookOutRight = HeadAnimInstance->GetCurveValue( "EyeLookOutRight" );
		float EyeLookUpRight = HeadAnimInstance->GetCurveValue( "EyeLookUpRight" );
		float EyeLookDownRight = HeadAnimInstance->GetCurveValue( "EyeLookDownRight" );
		if( EyeLookInRight > 0.f )
		{
			IrisOffsetXRight = FMath::Lerp( IrisPositionRight, HeadMeshData->IrisMaxPositionIn, EyeLookInRight ) - IrisPositionRight;
		}
		else if( EyeLookOutRight > 0.f )
		{
			IrisOffsetXRight = FMath::Lerp( IrisPositionRight, HeadMeshData->IrisMaxPositionOut, EyeLookOutRight ) - IrisPositionRight;
		}
		if( EyeLookUpRight > 0.f )
		{
			IrisOffsetYRight = FMath::Lerp( IrisPositionRight, HeadMeshData->IrisMaxPositionUp, EyeLookUpRight ) - IrisPositionRight;
		}
		else if( EyeLookDownRight > 0.f )
		{
			IrisOffsetYRight = FMath::Lerp( IrisPositionRight, HeadMeshData->IrisMaxPositionDown, EyeLookDownRight ) - IrisPositionRight;
		}

		IrisPositionRight += IrisOffsetXRight + IrisOffsetYRight;

		/* Iris scale : if eye is wide, scale down the iris for a scared/suprise effect. Move down the eyelash shadow (Sclera dark corner) when eye is closing
		*/
		float EyeWideLeft = HeadAnimInstance->GetCurveValue( "EyeWideLeft" );
		float ScalingClampedLeft = FMath::Max(( 1 - EyeWideLeft ), HeadMeshData->EyesMinimumScale );
		FVector2D IrisScaleLeft( HumanBodyData.EyeCustomData.GetCustomDataValue( 0, 4 ) * ScalingClampedLeft, HumanBodyData.EyeCustomData.GetCustomDataValue( 0, 5 ) * ScalingClampedLeft );

		float EyeWideRight = HeadAnimInstance->GetCurveValue( "EyeWideRight" );
		float ScalingClampedRight = FMath::Max( ( 1 - EyeWideRight ), HeadMeshData->EyesMinimumScale );
		FVector2D IrisScaleRight( HumanBodyData.EyeCustomData.GetCustomDataValue( 1, 4 ) * ScalingClampedRight, HumanBodyData.EyeCustomData.GetCustomDataValue( 1, 5 ) * ScalingClampedRight );

		float EyelashPositionLeft = HumanBodyData.EyeCustomData.GetCustomDataValue( 0, 17 ) + EyeWideLeft * HumanBodyData.EyeCustomData.EyelashShadowWeightOpen - HeadAnimInstance->GetCurveValue("EyeBlinkLeft") * HumanBodyData.EyeCustomData.EyelashShadowWeightClose;
		float EyelashPositionRight = HumanBodyData.EyeCustomData.GetCustomDataValue( 1, 17 ) + EyeWideRight * HumanBodyData.EyeCustomData.EyelashShadowWeightOpen - HeadAnimInstance->GetCurveValue("EyeBlinkRight") * HumanBodyData.EyeCustomData.EyelashShadowWeightClose;
		
		uint8 NumberOfEyes = Eyes->GetInstanceCount();

		// Set custom data for all instances
		for( uint8 EyeIndex = 0; EyeIndex < NumberOfEyes; EyeIndex++ )
		{
			if( EyeIndex % 2 > 0 )
			{
				// right eye
				Eyes->SetCustomDataValue( EyeIndex, 4, IrisScaleRight.X );
				Eyes->SetCustomDataValue( EyeIndex, 5, IrisScaleRight.Y );
				Eyes->SetCustomDataValue( EyeIndex, 6, IrisPositionRight.X );
				Eyes->SetCustomDataValue( EyeIndex, 7, IrisPositionRight.Y );

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
}

void AHuman::SetupSkeletalMeshComponents( TArray<FModularSkeletalMeshData>& SkeletalMeshes )
{
	for( FModularSkeletalMeshData& SkeletalMeshData : SkeletalMeshes )
	{
		SetupOneSkeletalMeshComponent( SkeletalMeshData );
	}
}

void AHuman::SetupOneSkeletalMeshComponent( FModularSkeletalMeshData& SkeletalMeshData )
{
	if( SkeletalMeshData.MeshData )
	{
		if( USkeletalMesh* SkeletalMesh = SkeletalMeshData.MeshData->Mesh.LoadSynchronous() )
		{
			UE_LOG( LogCharacter, Log, TEXT( "- Skeletal mesh %s found and loaded" ), *UEnum::GetValueAsString( SkeletalMeshData.MeshData->MeshType ) );
			if( USkeletalMeshComponent* BodyComponent = CreateSkeletalMeshComponent( UEnum::GetValueAsName( SkeletalMeshData.MeshData->MeshType ) ) )
			{
				UE_LOG( LogCharacter, Log, TEXT( "  * Component created succesfully" ) );
				BodyComponent->SetSkeletalMesh( SkeletalMesh );

				if( SkeletalMeshData.bUseCustomMaterial )
				{
					UE_LOG( LogCharacter, Log, TEXT( "  * Use custom materials" ) );
					UpdateSkeletalMeshMaterials( BodyComponent, SkeletalMeshData.CustomMaterials );
				}
				else
				{
					UE_LOG( LogCharacter, Log, TEXT( "  * Use default materials" ) );
					UpdateSkeletalMeshMaterials( BodyComponent, SkeletalMeshData.MeshData->DefaultMaterials );
				}

				if( SkeletalMeshData.MeshData->MeshType == EBodyPartType::Head )
				{
					HeadMeshData = Cast<UHeadMeshData>( SkeletalMeshData.MeshData );
					if( !HeadMeshData ) return BodyComponent->DestroyComponent();
					UE_LOG( LogCharacter, Log, TEXT( "  * Body part is head" ) );

					TArray<float> CustomFaceDatas = HumanBodyData.SkinAndFaceCustomData.CombineFaceData();
					UpdateSkeletalMeshCustomData( BodyComponent, CustomFaceDatas, SkeletalMeshData.MeshData->MeshType );

					// TODO: Temporary set body mask to default
					BodyComponent->SetCustomPrimitiveDataFloat( 35, 1.01f);

					#if WITH_EDITOR
					BodyComponent->SetDefaultCustomPrimitiveDataFloat( 35, 1.01f );
					#endif
					

					UpdateHeadAnimInstance();
				}
				else if( SkeletalMeshData.MeshData->MeshType == EBodyPartType::Hair )
				{
					UE_LOG( LogCharacter, Log, TEXT( "  * Body part is hair" ) );
				}
				else
				{
					UE_LOG( LogCharacter, Log, TEXT( "  * Body part is cloth" ) );
					TArray<float> SkinData = HumanBodyData.SkinAndFaceCustomData.CombineSkinData();
					// combine cloth data and send merge both array
					UpdateSkeletalMeshCustomData( BodyComponent, SkinData, SkeletalMeshData.MeshData->MeshType);

					// TODO: Temporary set all body mask to default, later make function updateMainBodyMask
					BodyComponent->SetCustomPrimitiveDataFloat( 22, 0.f );
					BodyComponent->SetCustomPrimitiveDataFloat( 23, 1.01f );
					BodyComponent->SetCustomPrimitiveDataFloat( 24, 0.f );
					BodyComponent->SetCustomPrimitiveDataFloat( 25, 1.01f );
					BodyComponent->SetCustomPrimitiveDataFloat( 26, 1.01f );
					BodyComponent->SetCustomPrimitiveDataFloat( 27, 1.01f );
					BodyComponent->SetCustomPrimitiveDataFloat( 28, 1.01f );

					// Only set default CustomPrimitiveData in editor because for runtime it's useless
					#if WITH_EDITOR
					BodyComponent->SetDefaultCustomPrimitiveDataFloat( 22, 0.f );
					BodyComponent->SetDefaultCustomPrimitiveDataFloat( 23, 1.01f );
					BodyComponent->SetDefaultCustomPrimitiveDataFloat( 24, 0.f );
					BodyComponent->SetDefaultCustomPrimitiveDataFloat( 25, 1.01f );
					BodyComponent->SetDefaultCustomPrimitiveDataFloat( 26, 1.01f );
					BodyComponent->SetDefaultCustomPrimitiveDataFloat( 27, 1.01f );
					BodyComponent->SetDefaultCustomPrimitiveDataFloat( 28, 1.01f );
					#endif
				}

				RefreshLeaderComponent( BodyComponent );
			}
		}
	}
}

USkeletalMeshComponent* AHuman::CreateSkeletalMeshComponent( FName BodyPartName )
{
	// check if the body part already exist, return it if it's already created
	for( USkeletalMeshComponent* SkeletalComponent : SkeletalComponents )
	{
		if( BodyPartName == SkeletalComponent->GetFName() )
		{
			return SkeletalComponent;
		}
	}

	USkeletalMeshComponent* BodyComponentCreated = NewObject<USkeletalMeshComponent>( this, USkeletalMeshComponent::StaticClass(), BodyPartName);

	if( BodyComponentCreated )
	{
		InitializeSkeletalMeshComponent( BodyComponentCreated );
		SkeletalComponents.Add( BodyComponentCreated );
		return BodyComponentCreated;
	}

	UE_LOG( LogCharacter, Error, TEXT( "Error when creating USkeletalMeshComponent" ) );
	return nullptr;
}

void AHuman::InitializeSkeletalMeshComponent( USkeletalMeshComponent* SkeletalMeshComponent )
{
	if( SkeletalMeshComponent->GetFName() == UEnum::GetValueAsName( EBodyPartType::Hair ) ) SkeletalMeshComponent->bUseAttachParentBound = true;
	
	RemoveOwnedComponent( SkeletalMeshComponent );
	SkeletalMeshComponent->CreationMethod = EComponentCreationMethod::Instance;
	AddOwnedComponent( SkeletalMeshComponent );

	SkeletalMeshComponent->AttachToComponent( MainBody, FAttachmentTransformRules::SnapToTargetIncludingScale );

	SkeletalMeshComponent->RegisterComponent();
}

void AHuman::RefreshLeaderComponent( USkeletalMeshComponent* SkeletalMeshComponent )
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

void AHuman::UpdateSkeletalMeshMaterials( USkeletalMeshComponent* SkeletalMeshComponent, TArray<TSoftObjectPtr<UMaterialInstance>>& Materials )
{
	int32 MeshMaterialNum = SkeletalMeshComponent->GetSkeletalMeshAsset()->GetNumMaterials();
	for( uint8 MaterialIndex = 0; MaterialIndex < Materials.Num(); MaterialIndex++ )
	{
		if( MaterialIndex >= MeshMaterialNum ) return;

		if( Materials.IsValidIndex( MaterialIndex ) )
		{
			UMaterialInstance* Material = Materials[MaterialIndex].LoadSynchronous();
			if( Material ) SkeletalMeshComponent->SetMaterial( MaterialIndex, Material );
		}
	}
}

void AHuman::UpdateSkeletalMeshCustomData( USkeletalMeshComponent* SkeletalMeshComponent, TArray<float>& CustomDatas, EBodyPartType BodyPart )
{
	for( int32 DataIndex = 0; DataIndex < CustomDatas.Num(); DataIndex++ )
	{
		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( DataIndex + FSkinFaceCustomData::StartingIndexSkin, CustomDatas[DataIndex] );
	}

	#if WITH_EDITOR
	for( int32 DataIndex = 0; DataIndex < CustomDatas.Num(); DataIndex++ )
	{
		SkeletalMeshComponent->SetDefaultCustomPrimitiveDataFloat( DataIndex + FSkinFaceCustomData::StartingIndexSkin, CustomDatas[DataIndex] );
	}
	#endif

	UpdateSkeletalMeshBodyMorphology( SkeletalMeshComponent, EBodyMorphCustomData::BodyMuscle, BodyPart );
	UpdateSkeletalMeshBodyMorphology( SkeletalMeshComponent, EBodyMorphCustomData::LegsMuscle, BodyPart );
	UpdateSkeletalMeshBodyMorphology( SkeletalMeshComponent, EBodyMorphCustomData::Boobs, BodyPart );
}

void AHuman::UpdateSkeletalMeshBodyMorphology( USkeletalMeshComponent* SkeletalMeshComponent, EBodyMorphCustomData CustomDataType, EBodyPartType BodyPart )
{
	switch( CustomDataType )
	{
		case EBodyMorphCustomData::BodyMuscle:
		if( BodyPart == EBodyPartType::Head ) return;

		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( (BodyPart == EBodyPartType::MainBody ) ? 30 : 34, HumanBodyData.BodyMorphology.BodyMuscle );

		#if WITH_EDITOR
		SkeletalMeshComponent->SetDefaultCustomPrimitiveDataFloat( ( BodyPart == EBodyPartType::MainBody ) ? 30 : 34, HumanBodyData.BodyMorphology.BodyMuscle );
		#endif
		break;
		case EBodyMorphCustomData::LegsMuscle:
		if( BodyPart == EBodyPartType::Head ) return;

		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( ( BodyPart == EBodyPartType::MainBody ) ? 31 : 35, HumanBodyData.BodyMorphology.LegsMuscle );

		#if WITH_EDITOR
		SkeletalMeshComponent->SetDefaultCustomPrimitiveDataFloat( ( BodyPart == EBodyPartType::MainBody ) ? 31 : 35, HumanBodyData.BodyMorphology.LegsMuscle );
		#endif
		break;
		case EBodyMorphCustomData::Boobs:
		if( BodyPart == EBodyPartType::MainBody )
		{
			SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 29, HumanBodyData.BodyMorphology.Boobs );
			#if WITH_EDITOR
			SkeletalMeshComponent->SetDefaultCustomPrimitiveDataFloat( 29, HumanBodyData.BodyMorphology.Boobs );
			#endif
		}
		else if ( BodyPart == EBodyPartType::Head )
		{
			SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 34, HumanBodyData.BodyMorphology.Boobs );
			#if WITH_EDITOR
			SkeletalMeshComponent->SetDefaultCustomPrimitiveDataFloat( 34, HumanBodyData.BodyMorphology.Boobs );
			#endif
		}
		else
		{
			SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 33, HumanBodyData.BodyMorphology.Boobs );
			#if WITH_EDITOR
			SkeletalMeshComponent->SetDefaultCustomPrimitiveDataFloat( 33, HumanBodyData.BodyMorphology.Boobs );
			#endif
		}
		break;
		default:
		return;
		break;
	}
}

void AHuman::UpdateHeadAnimInstance()
{
	if( HeadMeshData )
	{
		if( USkeletalMeshComponent* HeadComponent = GetHeadComponent() )
		{

			HeadComponent->SetAnimInstanceClass( HeadMeshData->HeadAnimationBlueprint );
		}
	}
}

USkeletalMeshComponent* AHuman::GetHeadComponent() const
{
	for( int32 ComponentIndex = 0; ComponentIndex < SkeletalComponents.Num(); ComponentIndex++ )
	{
		if( SkeletalComponents[ComponentIndex]->GetFName() == UEnum::GetValueAsName(EBodyPartType::Head) ) return SkeletalComponents[ComponentIndex];
	}

	return nullptr;
}

void AHuman::SetHumanBodyData( const FHumanBodyData& InHumanBodyData )
{
	HumanBodyData = InHumanBodyData;
}

FHumanBodyData& AHuman::GetHumanBodyData()
{
	return HumanBodyData;
}

USkeletalMeshComponent* AHuman::GetMainBodyComponent() const
{
	return MainBody;
}

void AHuman::ResetFirstInit()
{
	bIsFirstInit = true;
}

void AHuman::SetGlobalWetness( float GlobalWetnessOpacity )
{
	HumanStateData.GlobalWetness = GlobalWetnessOpacity;

	MainBody->SetCustomPrimitiveDataFloat( 0, GlobalWetnessOpacity );

	for( USkeletalMeshComponent* SkeletalMeshComponent : SkeletalComponents )
	{
		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 0, GlobalWetnessOpacity );
	}
}

void AHuman::SetIsUnderRoof( bool IsUnderRoof )
{
	if( HumanStateData.bIsUnderRoof == IsUnderRoof ) return;

	HumanStateData.bIsUnderRoof = IsUnderRoof;

	float UnderRoofFloat = (IsUnderRoof) ? 1.f : 0.f;

	MainBody->SetCustomPrimitiveDataFloat( 3, UnderRoofFloat );
	#if WITH_EDITOR
	MainBody->SetDefaultCustomPrimitiveDataFloat( 3, UnderRoofFloat );
	#endif

	for( USkeletalMeshComponent* SkeletalMeshComponent : SkeletalComponents )
	{
		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 3, UnderRoofFloat );
		#if WITH_EDITOR
		SkeletalMeshComponent->SetDefaultCustomPrimitiveDataFloat( 3, UnderRoofFloat );
		#endif
	}
}

void AHuman::SetHeightWetness( float HeightWetness, float HeightWetnessOpacity )
{
	if( HumanStateData.WetnessZHeight == HeightWetness && HumanStateData.WetnessZOpacity == HeightWetnessOpacity ) return;

	HumanStateData.WetnessZHeight = HeightWetness;
	HumanStateData.WetnessZOpacity = HeightWetnessOpacity;

	MainBody->SetCustomPrimitiveDataFloat( 1, HeightWetness );
	MainBody->SetCustomPrimitiveDataFloat( 2, HeightWetnessOpacity );
	#if WITH_EDITOR
	MainBody->SetDefaultCustomPrimitiveDataFloat( 1, HeightWetness );
	MainBody->SetDefaultCustomPrimitiveDataFloat( 2, HeightWetnessOpacity );
	#endif

	for( USkeletalMeshComponent* SkeletalMeshComponent : SkeletalComponents )
	{
		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 1, HeightWetness );
		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 2, HeightWetnessOpacity );
		#if WITH_EDITOR
		SkeletalMeshComponent->SetDefaultCustomPrimitiveDataFloat( 1, HeightWetness );
		SkeletalMeshComponent->SetDefaultCustomPrimitiveDataFloat( 2, HeightWetnessOpacity );
		#endif
	}
}

void AHuman::SetHeightMask( float HeightMask )
{
	if( HumanStateData.MaskHeight == HeightMask ) return;

	HumanStateData.MaskHeight = HeightMask;

	MainBody->SetCustomPrimitiveDataFloat( 4, HeightMask );
	#if WITH_EDITOR
	MainBody->SetDefaultCustomPrimitiveDataFloat( 4, HeightMask );
	#endif

	for( USkeletalMeshComponent* SkeletalMeshComponent : SkeletalComponents )
	{
		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 4, HeightMask );
		#if WITH_EDITOR
		SkeletalMeshComponent->SetDefaultCustomPrimitiveDataFloat( 4, HeightMask );
		#endif
	}
}

void AHuman::SetSphereMask( FVector SphereMaskPosition, float SphereMaskRadius )
{
	if( HumanStateData.SphereMaskPosition == SphereMaskPosition && HumanStateData.SphereMaskRadius == SphereMaskRadius ) return;

	HumanStateData.SphereMaskPosition = SphereMaskPosition;
	HumanStateData.SphereMaskRadius = SphereMaskRadius;

	MainBody->SetCustomPrimitiveDataFloat( 5, SphereMaskPosition.X );
	MainBody->SetCustomPrimitiveDataFloat( 6, SphereMaskPosition.Y );
	MainBody->SetCustomPrimitiveDataFloat( 7, SphereMaskPosition.Z );
	MainBody->SetCustomPrimitiveDataFloat( 8, SphereMaskRadius );

	for( USkeletalMeshComponent* SkeletalMeshComponent : SkeletalComponents )
	{
		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 5, SphereMaskPosition.X );
		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 6, SphereMaskPosition.Y );
		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 7, SphereMaskPosition.Z );
		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 8, SphereMaskRadius );
	}
}

void AHuman::SetReverseMask( bool IsMaskReverse )
{
	if( HumanStateData.bReverseMask == IsMaskReverse ) return;

	HumanStateData.bReverseMask = IsMaskReverse;

	float ReverseMaskFloat = ( IsMaskReverse ) ? 1.f : 0.f;

	MainBody->SetCustomPrimitiveDataFloat( 9, ReverseMaskFloat );

	for( USkeletalMeshComponent* SkeletalMeshComponent : SkeletalComponents )
	{
		SkeletalMeshComponent->SetCustomPrimitiveDataFloat( 9, ReverseMaskFloat );
	}
}

void AHuman::SetEffect( float EffectNumber )
{
	USkeletalMeshComponent* HeadComponent = GetHeadComponent();
	if( HeadComponent && HeadMeshData && HumanStateData.Effect != EffectNumber )
	{
		EffectNumber = FMath::CeilToFloat(EffectNumber);
		HumanStateData.Effect = EffectNumber;

		if( EffectNumber >= 0 && EffectNumber <= 3.01)
		{
			HeadComponent->SetCustomPrimitiveDataFloat( 33, EffectNumber );

			
		}
		else
		{
			HeadComponent->SetCustomPrimitiveDataFloat( 33, 0 );
		}

		if( EffectNumber >= 0.99 && EffectNumber < 1.99)
		{
			for(uint8 EyeIndex = 0; EyeIndex < Eyes->GetInstanceCount(); EyeIndex++ )
			{
				#if WITH_EDITOR
				Eyes->SetCustomDataValue( EyeIndex, 19, HumanBodyData.EyeCustomData.MaxCryingEffect, true );
				#else
				Eyes->SetCustomDataValue( EyeIndex, 19, HumanBodyData.EyeCustomData.MaxCryingEffect );
				#endif
			}
		}
		else
		{
			for( uint8 EyeIndex = 0; EyeIndex < Eyes->GetInstanceCount(); EyeIndex++ )
			{
				#if WITH_EDITOR
				Eyes->SetCustomDataValue( EyeIndex, 19, 0, true );
				#else
				Eyes->SetCustomDataValue( EyeIndex, 19, 0 );
				#endif
			}
		}
	}
}

void AHuman::UpdateHeadAnimation()
{
	if( USkeletalMeshComponent* HeadComponent = GetHeadComponent() )
	{
		UAnimInstance* HeadAnimInstance = HeadComponent->GetAnimInstance();
		if(!HeadAnimInstance ) return;

		// Average of top lip going up and bottom lip going down
		float TeethAnimation = ( HeadAnimInstance->GetCurveValue( TEXT( "MouthUpperUpLeft" ) ) + HeadAnimInstance->GetCurveValue( TEXT( "MouthLowerDownLeft" ) )
		+ HeadAnimInstance->GetCurveValue( TEXT( "MouthUpperUpRight" ) ) + HeadAnimInstance->GetCurveValue( TEXT( "MouthLowerDownRight" ) ) ) / 4;
		HeadComponent->SetCustomPrimitiveDataFloat( 30, TeethAnimation);

		// Jaw open minus mouth closed
		float JawAnimation = HeadAnimInstance->GetCurveValue( TEXT( "JawOpen" ) ) - HeadAnimInstance->GetCurveValue( TEXT( "MouthClose" ) );
		HeadComponent->SetCustomPrimitiveDataFloat( 31, JawAnimation );

		// Tongue
		HeadComponent->SetCustomPrimitiveDataFloat( 32, HeadAnimInstance->GetCurveValue( TEXT( "TongueOut" ) ) );
	}
}

void AHuman::SetEyesEmissive( float EmissiveCoef, TArray<int32>& EyesIndex )
{
	USkeletalMeshComponent* HeadComponent = GetHeadComponent();
	if( HeadComponent && HeadMeshData )
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
	USkeletalMeshComponent* HeadComponent = GetHeadComponent();
	if( HeadComponent && HeadMeshData )
	{
		uint8 NumberOfEyes = Eyes->GetInstanceCount();

		if( NumberOfEyes <= 0 ) return;

		for( float EyeIndex : EyesIndex )
		{
			if( HumanStateData.EyesBleeding.IsValidIndex( EyeIndex ) && HumanBodyData.EyeCustomData.ScleraColorBleeding.IsValidIndex( EyeIndex ) )
			{
				if( HumanStateData.EyesBleeding[EyeIndex] == BleedingCoef || HumanBodyData.EyeCustomData.ScleraColorBleeding[EyeIndex] == HumanBodyData.EyeCustomData.ScleraColor[EyeIndex] ) continue;

				HumanStateData.EyesBleeding[EyeIndex] = BleedingCoef;

				FLinearColor ColorFinal = FMath::Lerp( HumanBodyData.EyeCustomData.ScleraColor[EyeIndex], HumanBodyData.EyeCustomData.ScleraColorBleeding[EyeIndex], BleedingCoef );

				if( Eyes->IsValidInstance( EyeIndex ) )
				{
					
					#if WITH_EDITOR
					Eyes->SetCustomDataValue( EyeIndex, 8, ColorFinal.R );
					Eyes->SetCustomDataValue( EyeIndex, 9, ColorFinal.G );
					Eyes->SetCustomDataValue( EyeIndex, 10, ColorFinal.B );
					Eyes->SetCustomDataValue( EyeIndex, 19, 0.075*BleedingCoef );
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
}

void AHuman::SetPupilScale( float PupilScale, TArray<int32>& EyesIndex )
{
	USkeletalMeshComponent* HeadComponent = GetHeadComponent();
	if( HeadComponent && HeadMeshData )
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
					Eyes->SetCustomDataValue( EyeIndex, 21, PupilScale, true );
					#else
					Eyes->SetCustomDataValue( EyeIndex, 21, PupilScale );
					#endif
				}
			}
		}
	}
}

#if WITH_EDITORONLY_DATA
void AHuman::UpdateMainBodyAnimation( UAnimSequence* AnimationToPlay, float InAnimPlayRate, FName InIphoneName )
{
	MainBodyAnimationToPlay = AnimationToPlay;
	AnimPlayRate = InAnimPlayRate;
	SetArkitName_Editor( InIphoneName );
	SetEditorAnimationBP( EditorAnimationBP );
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

void AHuman::SetEditorAnimationBP( TSubclassOf<UAnimInstance> InAnimationBP )
{
	EditorAnimationBP = InAnimationBP;

	if( EditorAnimationBP )
	{
		MainBody->SetAnimInstanceClass( EditorAnimationBP );
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

UAnimSequence* AHuman::GetAnimationPreview() const
{
	return MainBodyAnimationToPlay;
}

float AHuman::GetAnimationPlayRate() const
{
	return AnimPlayRate;
}
#endif // WITH_EDITORONLY_DATA

/*
TODO:
Fix head shadow (agane)
Fix mouth shape for MouthPucker (probably), blend badly with jaw opening
*/