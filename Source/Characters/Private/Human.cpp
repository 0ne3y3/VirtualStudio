// Fill out your copyright notice in the Description page of Project Settings.


#include "Human.h"
#include "ModularSkeletalMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC( LogCharacter, Log, All );

FName AHuman::MainBodyName(TEXT("MainBodyMesh"));
FName AHuman::EyesName( TEXT( "EyesMesh" ) );

AHuman::AHuman( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f / 30.f;

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

// Called every frame
void AHuman::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	UpdateEyes();
}

void AHuman::OnConstruction( const FTransform& Transform )
{
	Super::OnConstruction( Transform );
		
	if( HumanBodyData.MainBody.MeshData )
	{
		if(!SetupMainBody( HumanBodyData.MainBody.MeshData )) return;

		// setup SkeletalMeshModular
		SetupSkeletalMeshComponents( HumanBodyData.SkeletalMeshes );

		// setup Accessory

		// setup eyes if head
		if( HeadMeshData )
		{
			SetHeadAnimationData( HeadMeshData->AnimationData );
			SetupEyes( HeadMeshData->NumberOfEyes, GetHeadComponent() );
		}
		else
		{
			UpdateNumberOfEyeInstance(0, MainBody);
			PrimaryActorTick.bCanEverTick = false;
		}
	}
}

bool AHuman::SetupMainBody( USkeletalMeshData* MeshData )
{
	UMainBodyMeshData* MainBodyData = Cast<UMainBodyMeshData>( MeshData );

	if( MainBodyData && !MainBodyData->Mesh )
	{
		USkeletalMesh* SkeletalMesh = MainBodyData->Mesh.LoadSynchronous();

		if( SkeletalMesh )
		{
			MainBody->SetSkeletalMesh( SkeletalMesh );
			UpdateMainBodyMaterials();

			UpdateMainBodyCustomData( HumanBodyData.SkinAndFaceCustomData );			

			MainBody->SetAnimInstanceClass( MainBodyData->BodyAnimationBlueprint );

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
	TArray<TSoftObjectPtr<UMaterialInstance>>& CustomMaterials = HumanBodyData.MainBody.CustomMaterials;
	TArray<TSoftObjectPtr<UMaterialInstance>>& DefaultMaterials = HumanBodyData.MainBody.MeshData->DefaultMaterials;

	for( uint8 MaterialIndex = 0; MaterialIndex < MeshMaterialNum; MaterialIndex++ )
	{
		UMaterialInstance* Material;
		if( bUseCustomMaterial && CustomMaterials[MaterialIndex] )
		{
			Material = CustomMaterials[MaterialIndex].LoadSynchronous();
			if( Material )
			{
				MainBody->SetMaterial( MaterialIndex, Material );
				continue;
			}
		}

		if( DefaultMaterials[MaterialIndex] )
		{
			Material = DefaultMaterials[MaterialIndex].LoadSynchronous();
			if( Material ) MainBody->SetMaterial( MaterialIndex, Material );
		}
	}
}

void AHuman::UpdateMainBodyCustomData( FSkinFaceCustomData& SkinFaceData )
{
	for( int32 CustomDataIndex = FSkinFaceCustomData::StartingIndexSkin; CustomDataIndex <= FSkinFaceCustomData::EndingIndexBody; CustomDataIndex++ )
	{
		MainBody->SetCustomPrimitiveDataFloat( CustomDataIndex, SkinFaceData.GetBodyCustomDataValue( CustomDataIndex ) );
	}
}

bool AHuman::SetupEyes( int32 NumberOfEyes, USkeletalMeshComponent* HeadComponent )
{
	if( !HeadMeshData || HeadMeshData->EyeMesh ) return false;

	if( NumberOfEyes < 1 ) return true;

	UStaticMesh* EyeMesh = HeadMeshData->EyeMesh.LoadSynchronous();
	if( EyeMesh )
	{
		Eyes->AttachToComponent( (HeadComponent) ? HeadComponent : MainBody, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT( "head" ) );
		Eyes->SetStaticMesh( EyeMesh );
		
		UpdateEyesMaterial(HumanBodyData.EyeCustomData.MaterialIndex);

		UpdateNumberOfEyeInstance( NumberOfEyes, HeadComponent );

		Eyes->NumCustomDataFloats = FEyeCustomData::MaxCustomData;

		UpdateEyeDefaultPosition();

		UpdateEyesCustomData( HumanBodyData.EyeCustomData, NumberOfEyes );
		
		return true;
	}

	return false;
}

void AHuman::UpdateNumberOfEyeInstance( int32 NumberOfEyes, USkeletalMeshComponent* HeadComponent)
{
	if( !HeadComponent ) return;

	int32 EyeLimit = FMath::Max( NumberOfEyes, Eyes->GetInstanceCount() );

	for( uint8 EyeIndex = 0; EyeIndex < EyeLimit; EyeIndex++ )
	{
		FName EyeSocketName = FName( FString::Printf( TEXT( "eye%d" ), EyeIndex + 1 ) );

		if( EyeIndex < NumberOfEyes )
		{
			if( Eyes->IsValidInstance( EyeIndex ) )
			{
				Eyes->UpdateInstanceTransform( EyeIndex, HeadComponent->GetSocketTransform( EyeSocketName, ERelativeTransformSpace::RTS_World ), true );
			}
			else
			{
				Eyes->AddInstance( HeadComponent->GetSocketTransform( EyeSocketName, ERelativeTransformSpace::RTS_World ), true );
			}
		}
		else
		{
			Eyes->RemoveInstance( EyeIndex );
		}
	}

	DynamicMaterialData.EyesBleeding.Empty();
	DynamicMaterialData.EyesBleeding.AddUninitialized( NumberOfEyes );
	DynamicMaterialData.PupilScale.Empty();
	DynamicMaterialData.PupilScale.AddUninitialized( NumberOfEyes );
	DynamicMaterialData.EyesEmissive.Empty();
	DynamicMaterialData.EyesEmissive.AddUninitialized( NumberOfEyes );
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
			// if it's the last instance, we set the render state dirty to update all the custom data
			if( EyeIndex == NumberOfEyes - 1 )
			{
				Eyes->SetCustomDataValue( EyeIndex, FEyeCustomData::LeftRightCustomDataIndex, 1.0f, true );
			}
			else
			{
				Eyes->SetCustomDataValue( EyeIndex, FEyeCustomData::LeftRightCustomDataIndex, 1.0f );
			}

		}
		else
		{
			// left eye
			if( EyeIndex == NumberOfEyes - 1 )
			{
				Eyes->SetCustomDataValue( EyeIndex, FEyeCustomData::LeftRightCustomDataIndex, 0.0f, true );
			}
			else
			{
				Eyes->SetCustomDataValue( EyeIndex, FEyeCustomData::LeftRightCustomDataIndex, 0.0f );
			}
		}
	}
}

void AHuman::UpdateEyeDefaultPosition()
{
	if( HeadMeshData )
	{
		for( uint8 EyeIndex = 0; EyeIndex < HumanBodyData.EyeCustomData.IrisOffset.Num(); EyeIndex++ )
		{
			HumanBodyData.EyeCustomData.IrisOffset[EyeIndex] = HeadMeshData->IrisDefaultPosition;
		}
	}
}

void AHuman::UpdateEyes()
{
	USkeletalMeshComponent* HeadComponent = GetHeadComponent();
	if( HeadComponent && HeadMeshData )
	{
		/* Iris position(in - out, up - down) : Lerp between base position (default iris offset) and max position set in head data
		*/

		FVector2D IrisPositionLeft( HumanBodyData.EyeCustomData.GetCustomDataValue( 0, 6 ), HumanBodyData.EyeCustomData.GetCustomDataValue( 0, 7 ) );
		FVector2D IrisOffsetXLeft = FVector2D::Zero();
		FVector2D IrisOffsetYLeft = FVector2D::Zero();
		if( float EyeLookInLeft = HeadComponent->GetMorphTarget( "EyeLookInLeft" ) > 0.f )
		{
			IrisOffsetXLeft = FMath::Lerp( IrisPositionLeft, HeadMeshData->IrisMaxPositionIn, EyeLookInLeft );
		}
		else if( float EyeLookOutLeft = HeadComponent->GetMorphTarget( "EyeLookOutLeft" ) > 0.f )
		{
			IrisOffsetXLeft = FMath::Lerp( IrisPositionLeft, HeadMeshData->IrisMaxPositionOut, EyeLookOutLeft );
		}

		if( float EyeLookUpLeft = HeadComponent->GetMorphTarget( "EyeLookUpLeft" ) > 0.f )
		{
			IrisOffsetYLeft = FMath::Lerp( IrisPositionLeft, HeadMeshData->IrisMaxPositionUp, EyeLookUpLeft );
		}
		else if( float EyeLookDownLeft = HeadComponent->GetMorphTarget( "EyeLookDownLeft" ) > 0.f )
		{
			IrisOffsetYLeft = FMath::Lerp( IrisPositionLeft, HeadMeshData->IrisMaxPositionDown, EyeLookDownLeft );
		}

		IrisPositionLeft += (IrisOffsetXLeft + IrisOffsetYLeft) - IrisPositionLeft;

		FVector2D IrisPositionRight( HumanBodyData.EyeCustomData.GetCustomDataValue( 1, 6 ), HumanBodyData.EyeCustomData.GetCustomDataValue( 1, 7 ) );
		FVector2D IrisOffsetXRight = FVector2D::Zero();
		FVector2D IrisOffsetYRight = FVector2D::Zero();
		if( float EyeLookInRight = HeadComponent->GetMorphTarget( "EyeLookInRight" ) > 0.f )
		{
			IrisOffsetXRight = FMath::Lerp( IrisPositionRight, HeadMeshData->IrisMaxPositionIn, EyeLookInRight );
		}
		else if( float EyeLookOutRight = HeadComponent->GetMorphTarget( "EyeLookOutRight" ) > 0.f )
		{
			IrisOffsetXRight = FMath::Lerp( IrisPositionRight, HeadMeshData->IrisMaxPositionOut, EyeLookOutRight );
		}

		if( float EyeLookUpRight = HeadComponent->GetMorphTarget( "EyeLookUpRight" ) > 0.f )
		{
			IrisOffsetYRight = FMath::Lerp( IrisPositionRight, HeadMeshData->IrisMaxPositionUp, EyeLookUpRight );
		}
		else if( float EyeLookDownRight = HeadComponent->GetMorphTarget( "EyeLookDownRight" ) > 0.f )
		{
			IrisOffsetYRight = FMath::Lerp( IrisPositionRight, HeadMeshData->IrisMaxPositionDown, EyeLookDownRight );
		}

		IrisPositionRight += (IrisOffsetXRight + IrisOffsetYRight) - IrisPositionRight;

		/* Iris scale : if eye is wide, scale down the iris for a scared/suprise effect. Move down the eyelash shadow (Sclera dark corner)
		*/

		float EyeWideLeft = HeadComponent->GetMorphTarget( "EyeWideLeft" );
		float ScalingClampedLeft = FMath::Max(( 1 - EyeWideLeft ), HeadMeshData->EyesMinimumScale );
		FVector2D IrisScaleLeft( HumanBodyData.EyeCustomData.GetCustomDataValue( 0, 4 ) * ScalingClampedLeft, HumanBodyData.EyeCustomData.GetCustomDataValue( 0, 5 ) * ScalingClampedLeft );

		float EyeWideRight = HeadComponent->GetMorphTarget( "EyeWideRight" );
		float ScalingClampedRight = FMath::Max( ( 1 - EyeWideRight ), HeadMeshData->EyesMinimumScale );
		FVector2D IrisScaleRight( HumanBodyData.EyeCustomData.GetCustomDataValue( 1, 4 ) * ScalingClampedRight, HumanBodyData.EyeCustomData.GetCustomDataValue( 1, 5 ) * ScalingClampedRight );

		float EyelashPositionLeft = HumanBodyData.EyeCustomData.GetCustomDataValue( 0, 17 ) + ( -EyeWideLeft )  + HeadComponent->GetMorphTarget("EyeBlinkLeft");
		float EyelashPositionRight = HumanBodyData.EyeCustomData.GetCustomDataValue( 1, 17 ) + ( -EyeWideRight )  + HeadComponent->GetMorphTarget("EyeBlinkRight");
		
		/* Crying effect : remap 0-1 range to 0 - Crying max range (most likely 0 - 0.15)
		*/
		float CryingEffectCoef = FMath::GetMappedRangeValueClamped(TRange<float>(0.f, 1.f), TRange<float>( 0.f, HumanBodyData.EyeCustomData.MaxCryingEffect ), DynamicMaterialData.CryingEffect);

		int32 NumberOfEyes = Eyes->GetInstanceCount();

		/* Bleeding eye : if one eye is bleeding, lerp between base color and bleeding color
		*/
		TArray<FLinearColor> BleedingColor;
		TArray<float> TextureOpacity;
		for( uint8 EyeIndex = 0; EyeIndex < NumberOfEyes; EyeIndex++ )
		{
			if( HumanBodyData.EyeCustomData.ScleraColorBleeding.IsValidIndex( EyeIndex ) && DynamicMaterialData.EyesBleeding.IsValidIndex( EyeIndex ) )
			{
				BleedingColor.Add( FMath::Lerp( HumanBodyData.EyeCustomData.ScleraColor[EyeIndex], HumanBodyData.EyeCustomData.ScleraColorBleeding[EyeIndex], DynamicMaterialData.EyesBleeding[EyeIndex] ) );
				TextureOpacity.Add( DynamicMaterialData.EyesBleeding[EyeIndex] );
			}
			else
			{
				BleedingColor.Add( HumanBodyData.EyeCustomData.ScleraColor[EyeIndex] );
				TextureOpacity.Add( HumanBodyData.EyeCustomData.ScleraTextureOpacity[EyeIndex] );
			}
		}

		// Set custom data for all instances
		for( uint8 EyeIndex = 0; EyeIndex < NumberOfEyes; EyeIndex++ )
		{
			Eyes->SetCustomDataValue( EyeIndex, 8, BleedingColor[EyeIndex].R );
			Eyes->SetCustomDataValue( EyeIndex, 9, BleedingColor[EyeIndex].G );
			Eyes->SetCustomDataValue( EyeIndex, 10, BleedingColor[EyeIndex].B );
			Eyes->SetCustomDataValue( EyeIndex, 11, TextureOpacity[EyeIndex] );
			Eyes->SetCustomDataValue( EyeIndex, 19, CryingEffectCoef );
			if( DynamicMaterialData.EyesEmissive.IsValidIndex( EyeIndex ) ) Eyes->SetCustomDataValue( EyeIndex, 20, DynamicMaterialData.EyesEmissive[EyeIndex] ); // Eye emissive
			if( DynamicMaterialData.PupilScale.IsValidIndex( EyeIndex )) Eyes->SetCustomDataValue( EyeIndex, 21, 1 + DynamicMaterialData.PupilScale[EyeIndex]); // Eye pupil scale

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
		USkeletalMesh* SkeletalMesh = SkeletalMeshData.MeshData->Mesh.LoadSynchronous();
		if( SkeletalMesh )
		{
			USkeletalMeshComponent* BodyComponent = CreateSkeletalMeshComponent( UEnum::GetValueAsName( SkeletalMeshData.MeshData->MeshType ) );
			if( BodyComponent )
			{
				BodyComponent->SetSkeletalMesh( SkeletalMesh );

				if( SkeletalMeshData.bUseCustomMaterial )
				{
					UpdateSkeletalMeshMaterials( BodyComponent, SkeletalMeshData.CustomMaterials );
				}
				else
				{
					UpdateSkeletalMeshMaterials( BodyComponent, SkeletalMeshData.MeshData->DefaultMaterials );
				}

				if( SkeletalMeshData.MeshData->MeshType == EBodyPartType::Head )
				{
					HeadMeshData = Cast<UHeadMeshData>( SkeletalMeshData.MeshData );
					if( !HeadMeshData ) return BodyComponent->DestroyComponent();

					TArray<float> CustomFaceDatas = HumanBodyData.SkinAndFaceCustomData.CombineFaceData();
					UpdateSkeletalMeshCustomData( BodyComponent, CustomFaceDatas );

					UpdateHeadAnimInstance();
				}
				else
				{
					// Combine cloth data
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
	SkeletalMeshComponent->bUseAttachParentBound = true;
	SkeletalMeshComponent->bUseBoundsFromLeaderPoseComponent = true;

	this->RemoveOwnedComponent( SkeletalMeshComponent );
	SkeletalMeshComponent->CreationMethod = EComponentCreationMethod::Instance;
	this->AddOwnedComponent( SkeletalMeshComponent );

	SkeletalMeshComponent->AttachToComponent( MainBody, FAttachmentTransformRules::SnapToTargetIncludingScale );

	SkeletalMeshComponent->RegisterComponent();
}

void AHuman::RefreshLeaderComponent( USkeletalMeshComponent* SkeletalMeshComponent )
{
	if( !SkeletalMeshComponent->HasValidAnimationInstance() )
	{
		SkeletalMeshComponent->SetLeaderPoseComponent( MainBody );
	}
	else
	{
		SkeletalMeshComponent->SetLeaderPoseComponent( nullptr );
	}
}

void AHuman::UpdateSkeletalMeshMaterials( USkeletalMeshComponent* SkeletalMeshComponent, TArray<TSoftObjectPtr<UMaterialInstance>>& Materials )
{
	int32 MeshMaterialNum = SkeletalMeshComponent->GetSkeletalMeshAsset()->GetNumMaterials();
	for( uint8 MaterialIndex = 0; MaterialIndex < Materials.Num(); MaterialIndex++ )
	{
		if( MaterialIndex >= MeshMaterialNum ) return;

		if( Materials[MaterialIndex] )
		{
			UMaterialInstance* Material = Materials[MaterialIndex].LoadSynchronous();
			if( Material ) SkeletalMeshComponent->SetMaterial( MaterialIndex, Material );
		}
	}
}

void AHuman::UpdateSkeletalMeshCustomData( USkeletalMeshComponent* SkeletalMeshComponent, TArray<float>& CustomDatas )
{
	for( int32 DataIndex = 0; DataIndex < CustomDatas.Num(); DataIndex++ )
	{
		SkeletalMeshComponent->SetCustomPrimitiveDataFloat(DataIndex + FSkinFaceCustomData::StartingIndexSkin, CustomDatas[DataIndex]);
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
	HumanBodyData = HumanBodyData;
}

USkeletalMeshComponent* AHuman::GetMainBodyComponent() const
{
	return MainBody;
}

#if WITH_EDITORONLY_DATA
void AHuman::UpdateMainBodyAnimation( UAnimSequence* AnimationToPlay, bool ShowControlRig, float InAnimPlayRate )
{
	MainBodyAnimationToPlay = AnimationToPlay;
	bShowControlRig = ShowControlRig;
	AnimPlayRate = InAnimPlayRate;
}
#endif // WITH_EDITORONLY_DATA