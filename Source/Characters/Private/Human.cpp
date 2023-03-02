// Fill out your copyright notice in the Description page of Project Settings.


#include "Human.h"
#include "ModularSkeletalMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC( LogCharacter, Log, All );

FName AHuman::MainBodyName(TEXT("MainBodyMesh"));
FName AHuman::EyesName( TEXT( "EyesMesh" ) );

AHuman::AHuman()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CharacterScene"));

	MainBody = CreateDefaultSubobject<USkeletalMeshComponent>( AHuman::MainBodyName );
	Eyes = CreateDefaultSubobject<UInstancedStaticMeshComponent>( AHuman::EyesName );

	if( MainBody != nullptr )
	{
		MainBody->SetupAttachment(RootComponent);
		MainBody->bPropagateCurvesToFollowers = true;
		/* Apply all default parameter, see later what to set as project goes */
	}
}

void AHuman::OnConstruction( const FTransform& Transform )
{
	Super::OnConstruction( Transform );

	// return if we don't have data to create the character
	if( !HumanBodyData ) return;
	
	// Setup skeletal mesh data
	for( FBodyMesh& BodyMeshData : HumanBodyData->BodyMeshes )
	{
		if( !BodyMeshData.MeshData ) continue;

		USkeletalMesh* Mesh = BodyMeshData.MeshData->Mesh.LoadSynchronous();

		if( Mesh )
		{
			switch( BodyMeshData.MeshData->MeshType )
			{
				case EBodyPartType::MainBody:
					SetupMainBody( Mesh, Cast<UMainBodyMeshData>( BodyMeshData.MeshData ), BodyMeshData.Materials );
					break;
				case EBodyPartType::Head:
					if( bHumanHaveHead ) continue; // we check if the character already have an head, coming from the main body
					bHumanHaveHead = true;
				default:
					SetupModularBodyPart( Mesh, BodyMeshData );
					break;
			}
			
		}
	}

	if( bHumanHaveBody )
	{
		// Setup eye and accessory data
		for( FAccessoryMesh& AccessoryMeshData : HumanBodyData->BodyAccessories )
		{
			UStaticMesh* Mesh = AccessoryMeshData.MeshData->Mesh.LoadSynchronous();

			if( AccessoryMeshData.MeshData->MeshType != EBodyPartType::Eye && Mesh )
			{
				// setup accessory
			}
		}
	}
}

void AHuman::SetupMainBody( USkeletalMesh* Mesh, UMainBodyMeshData* MainBodyMeshData, TArray<TSoftObjectPtr<UMaterialInstance>>& MeshMaterials )
{
	if( !MainBodyMeshData ) return;

	MainBody->SetSkeletalMesh(Mesh);

	bHumanHaveHead = MainBodyMeshData->bHaveHead;

	if( bHumanHaveHead && !bHumanHaveEyes && MainBodyMeshData->EyeData.MeshData )
	{
		UStaticMesh* EyeMeshData = MainBodyMeshData->EyeData.MeshData->Mesh.LoadSynchronous();
		if( EyeMeshData )
		{
			bHumanHaveEyes = true;
			SetupEyes( EyeMeshData, MainBodyMeshData->EyeData, MainBody );
		}
	}

	for( uint8 MaterialIndex = 0; MaterialIndex < MeshMaterials.Num(); MaterialIndex++ )
	{
		if( MeshMaterials[MaterialIndex] )
		{
			UMaterialInstance* Material = MeshMaterials[MaterialIndex].LoadSynchronous();
			if( Material ) MainBody->SetMaterial( MaterialIndex, Material );
		}
	}

	if( MainBodyMeshData->PPAnimationData ) PPAMainBody = MainBodyMeshData->PPAnimationData;
}

void AHuman::SetupEyes( UStaticMesh* Mesh, FAccessoryMesh& BodyMeshData, USkeletalMeshComponent* ParentMeshComponent )
{
	if( BodyMeshData.NumberOfEyes < 1 ) return;

	Eyes->AttachToComponent( ParentMeshComponent , FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("head"));

	Eyes->SetStaticMesh( Mesh );
	Eyes->NumCustomDataFloats = FEyeCustomData::MaxCustomData;
	
	if( BodyMeshData.Materials.IsValidIndex( 0 ) )
	{
		UMaterialInstance* Material = BodyMeshData.Materials[0].LoadSynchronous();
		if( Material ) Eyes->SetMaterial( 0, Material );
	}
		
	for( uint8 EyeIndex = 0; EyeIndex < BodyMeshData.NumberOfEyes; EyeIndex++ )
	{
		FName EyeSocketName = FName( FString::Printf( TEXT( "eye%d" ), EyeIndex+1 ) );
		if( !ParentMeshComponent->DoesSocketExist( EyeSocketName ) ) continue;

		Eyes->AddInstance( ParentMeshComponent->GetSocketTransform(EyeSocketName, ERelativeTransformSpace::RTS_World), true);
		
		for( uint8 CustomDataIndex = 0; CustomDataIndex < Eyes->NumCustomDataFloats; CustomDataIndex++ )
		{
			Eyes->SetCustomDataValue( EyeIndex, CustomDataIndex, HumanBodyData->EyeCustomData.GetCustomDataValue( EyeIndex, CustomDataIndex ) );
		}

		// Impair numbers are right eye, so we need to reverse the UV.x for the iris (because the socket transform is reversed too)
		if( EyeIndex % 2 > 0)
		{
			// right eye
			// if it's the last instance, we set the render state dirty to update all the custom data
			if( EyeIndex == BodyMeshData.NumberOfEyes - 1 )
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
			if( EyeIndex == BodyMeshData.NumberOfEyes - 1 )
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

void AHuman::SetupModularBodyPart( USkeletalMesh* Mesh, FBodyMesh& BodyMeshData )
{
	UModularSkeletalMeshComponent* BodyComponent = CreateNewBodyPart( UEnum::GetValueAsName( BodyMeshData.MeshData->MeshType ) );

	if( BodyComponent )
	{
		BodyComponent->SetSkeletalMesh( Mesh );

		TArray<UMaterialInstance*> MeshMaterialsOverride;
		for( uint8 MaterialIndex = 0; MaterialIndex < BodyMeshData.Materials.Num(); MaterialIndex++ )
		{
			if( BodyMeshData.Materials[MaterialIndex] ) MeshMaterialsOverride.AddUnique( BodyMeshData.Materials[MaterialIndex].LoadSynchronous() );
		}

		BodyComponent->SetSkeletalMeshMaterials( MeshMaterialsOverride );

		UHeadMeshData* HeadBodyMeshData = Cast<UHeadMeshData>( BodyMeshData.MeshData );
		if( HeadBodyMeshData && !bHumanHaveEyes && HeadBodyMeshData->EyeData.MeshData )
		{
			UStaticMesh* EyeMeshData = HeadBodyMeshData->EyeData.MeshData->Mesh.LoadSynchronous();
			if( EyeMeshData )
			{
				bHumanHaveEyes = true;
				SetupEyes( EyeMeshData, HeadBodyMeshData->EyeData, BodyComponent->GetBodyPartComponent() );
			}
		}
	}
}

UModularSkeletalMeshComponent* AHuman::CreateNewBodyPart( FName BodyPartName )
{
	// check if the body part already exist, return it if it's already here
	for( UModularSkeletalMeshComponent* BodyPart : BodyParts )
	{
		if( BodyPartName == BodyPart->GetFName() )
		{
			return BodyPart;
		}
	}

	UModularSkeletalMeshComponent* BodyComponentCreated = NewObject<UModularSkeletalMeshComponent>( this, UModularSkeletalMeshComponent::StaticClass(), BodyPartName);

	if( BodyComponentCreated != nullptr )
	{
		if( BodyComponentCreated->InitializeSkeletalMeshComponent( this, BodyPartName ) )
		{
			BodyParts.Add( BodyComponentCreated );
			return BodyComponentCreated;
		}
		else
		{
			BodyComponentCreated->DestroyComponent(true);
			return nullptr;
		}
	}

	UE_LOG( LogCharacter, Error, TEXT( "Error when creating UModularSkeletalMeshComponent" ) );
	return nullptr;
}