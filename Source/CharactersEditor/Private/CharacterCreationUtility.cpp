// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCreationUtility.h"
#include "Components/DetailsView.h"
#include "Components/Button.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Engine/Selection.h"
#include "EditorAssetLibrary.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "HumanAnimationData.h"
#include "Human.h"
#include "ModularSkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC( LogCharacter, Log, All );

void UCharacterCreationUtility::DestroyCharacter()
{
	if( CharacterSpawned )
	{
		CharacterSpawned->Destroy();
	}

	CharacterSpawned = nullptr;
}

void UCharacterCreationUtility::InitializeEditorWidget()
{
	CharacterDetailsView->CategoriesToShow.Add( "Character" );

	AnimationDetailsView->CategoriesToShow.Add( "Animation" );

	EditorDetailsView->CategoriesToShow.Add( "Preview|Effect" );
	EditorDetailsView->CategoriesToShow.Add( "Preview|Wetness" );
	EditorDetailsView->CategoriesToShow.Add( "Preview|Mask" );

	SaveButton->OnClicked.AddUniqueDynamic( this, &UCharacterCreationUtility::SaveButtonClicked );
	LoadButton->OnClicked.AddUniqueDynamic( this, &UCharacterCreationUtility::LoadButtonClicked );
	BlinkButton->OnClicked.AddUniqueDynamic( this, &UCharacterCreationUtility::BlinkButtonClicked );
	RefreshButton->OnClicked.AddUniqueDynamic( this, &UCharacterCreationUtility::UpdateCharacter );
}

void UCharacterCreationUtility::NativeConstruct()
{
	Super::NativeConstruct();

	if( UWorld* TheWorld = GetWorld() )
	{
		TArray<AActor*> FoundActors;
		
		UGameplayStatics::GetAllActorsOfClass( TheWorld, AHuman::StaticClass(), FoundActors );

		if( FoundActors.Num() > 0 )
		{
			CharacterSpawned = Cast<AHuman>(FoundActors[0]);
			CharacterProperties = CharacterSpawned->GetHumanBodyData();
			CharacterClass = FoundActors[0]->GetClass();
			AnimationPreview = CharacterSpawned->MainBodyAnimation_Editor;
			AnimPlayRate = CharacterSpawned->AnimationPlayRate_Editor;
			AnimationBP = CharacterSpawned->AnimationBP_Editor;

			UpdateEyesArray();
		}
	}
}

void UCharacterCreationUtility::UpdateCharacter()
{
	if( UWorld* TheWorld = GetWorld() )
	{
		if( !CharacterClass ) CharacterClass = AHuman::StaticClass();

		if( CharacterSpawned && CharacterSpawned->GetClass() != CharacterClass )
		{
			DestroyCharacter();
		}

		if( !CharacterSpawned )
		{
			FActorSpawnParameters Parameters;
			Parameters.bDeferConstruction = 1;
			CharacterSpawned = TheWorld->SpawnActor<AHuman>( CharacterClass, Parameters );
		}

		CharacterSpawned->SetEditorAnimationBP_Editor( AnimationBP );
		CharacterSpawned->SetHumanBodyData( CharacterProperties );

		CharacterSpawned->ConstructHumanCharacter();

		UpdateEyesArray();
		UpdatePreviewData();

		if( AnimationPreview && IsAnimSequenceCompatible() )
		{
			CharacterSpawned->UpdateMainBodyAnimation_Editor( AnimationPreview, AnimPlayRate, ARKitName );
		}
		else
		{
			CharacterSpawned->UpdateMainBodyAnimation_Editor( nullptr, AnimPlayRate, ARKitName );
		}
	}
}

void UCharacterCreationUtility::OnPropertyChangedMainPanel(FName PropertyName)
{
	UpdateCharacter();
}

void UCharacterCreationUtility::OnPropertyChangedEditorPreview( FName PropertyName )
{
	if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, Effect ) )
	{
		CharacterSpawned->SetFaceEffect( (float)Effect/10.f );
	}
	else if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, MainWetness ) )
	{
		CharacterSpawned->SetGlobalWetness( MainWetness );
	}
	else if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, HeightWetness ) || PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, HeightWetnessOpacity ) )
	{
		CharacterSpawned->SetHeightWetness( HeightWetness, HeightWetnessOpacity );
	}
	else if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, UnderRoof ) )
	{
		CharacterSpawned->SetIsUnderRoof( UnderRoof );
	}
	else if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, HeightMask ) )
	{
		CharacterSpawned->SetHeightMaskGlobal( HeightMask );
	}
	else if( PropertyName == TEXT("X") || PropertyName == TEXT( "Y" ) || PropertyName == TEXT( "Z" ) || PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterCreationUtility, SphereMaskRadius) )
	{
		CharacterSpawned->SetSphereMaskGlobal( SphereMaskPosition , SphereMaskRadius );
	}
	else if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, EyesBleeding ) )
	{
		for(uint8 I=0; I < EyesBleeding.Num(); I++ )
		{
			TArray<int32> IndexArray;
			IndexArray.Add(I);
			CharacterSpawned->SetEyesBleeding( EyesBleeding[I], IndexArray );
		}
	}
	else if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, EyesEmissive ) )
	{
		for( uint8 I=0; I < EyesEmissive.Num(); I++ )
		{
			TArray<int32> IndexArray;
			IndexArray.Add( I );
			CharacterSpawned->SetEyesEmissive( EyesEmissive[I], IndexArray );
		}
	}
	else if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, PupilScale ) )
	{
		for( uint8 I=0; I < PupilScale.Num(); I++ )
		{
			TArray<int32> IndexArray;
			IndexArray.Add( I );
			CharacterSpawned->SetPupilScale( PupilScale[I], IndexArray );
		}
	}
}

void UCharacterCreationUtility::OnPropertyChangedAnimation( FName PropertyName )
{
	if(!CharacterSpawned )
	{
		UE_LOG( LogCharacter, Error, TEXT( "ERROR : No character spawned !" ) );
		return;
	}

	CharacterSpawned->SetEditorAnimationBP_Editor( AnimationBP );

	if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, AnimPlayRate ) || PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, AnimationPreview ) )
	{
		if( AnimationPreview )
		{
			if( IsAnimSequenceCompatible() )
			{	
				CharacterSpawned->UpdateMainBodyAnimation_Editor( AnimationPreview, AnimPlayRate, ARKitName );
			}
			else
			{
				UE_LOG( LogCharacter, Error, TEXT( "ERROR : Preview animation not compatible with this skeleton !" ) );
			}
		}
		else
		{
			CharacterSpawned->UpdateMainBodyAnimation_Editor( nullptr, AnimPlayRate, ARKitName );
		}
	}
	else if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, AnimationBP ) )
	{
		if( UModularSkeletalMeshComponent* MainBodyComponent = CharacterSpawned->GetMainBodyComponent() )
		{
			MainBodyComponent->SetAnimInstanceClass( AnimationBP );
		}
	}
	else if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, ARKitName ) )
	{
		CharacterSpawned->UpdateMainBodyAnimation_Editor( AnimationPreview, AnimPlayRate, ARKitName );
	}
}

void UCharacterCreationUtility::SaveButtonClicked()
{
	
}

void UCharacterCreationUtility::LoadButtonClicked()
{

	TArray<FAssetData> AssetDatas;
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>( "ContentBrowser" );
	IContentBrowserSingleton& ContentBrowserSingleton = ContentBrowserModule.Get();
	ContentBrowserSingleton.GetSelectedAssets( AssetDatas );

	UHumanBodyPresetData* SelectedAssetData = nullptr;
	for( FAssetData AssetData : AssetDatas )
	{
		if( AssetData.GetClass() == UHumanBodyPresetData::StaticClass() )
		{
			SelectedAssetData = CastChecked<UHumanBodyPresetData>( UEditorAssetLibrary::LoadAsset( AssetData.GetObjectPathString() ) );
			if( SelectedAssetData ) break;
		}
	}

	if( SelectedAssetData )
	{
		CharacterProperties = SelectedAssetData->BodyData;

		UpdateCharacter();
	}
}

bool UCharacterCreationUtility::IsAnimSequenceCompatible()
{
	FModularMainBodyData& BodyMesh = CharacterProperties.MainBody;

	if( BodyMesh.MeshData )
	{
		USkeletalMesh* SkeletalMesh = BodyMesh.MeshData->Mesh.LoadSynchronous();
		if( SkeletalMesh )
		{
			if( SkeletalMesh->GetSkeleton()->IsCompatibleForEditor( FAssetData( AnimationPreview ) ) )
			{
				return true;
			}
		}
	}
	
	return false;
}

void UCharacterCreationUtility::UpdateEyesArray()
{
	FModularSkeletalMeshData ModularSkeletalMeshData = CharacterProperties.GetSkeletalMeshData(EBodyPartType::Head);

	if( UHeadMeshData* HeadData = Cast<UHeadMeshData>( ModularSkeletalMeshData.MeshData ) )
	{
		uint8 NumberOfEye = HeadData->NumberOfEyes;

		TArray<float> EyesBleedingTemp = EyesBleeding;
		TArray<float> EyesEmissiveTemp = EyesEmissive;
		TArray<float> PupilScaleTemp = PupilScale;

		EyesBleeding.Empty();
		EyesEmissive.Empty();
		PupilScale.Empty();

		for( uint8 I = 0; I < NumberOfEye; I++ )
		{
			EyesBleeding.Add(  EyesBleedingTemp.IsValidIndex( I )  ? EyesBleedingTemp[I] : 0 );
			EyesEmissive.Add( EyesEmissiveTemp.IsValidIndex( I )  ? EyesEmissiveTemp[I] : 0 );
			PupilScale.Add( PupilScaleTemp.IsValidIndex( I )  ? PupilScaleTemp[I] : 1 );
		}
	}
}

void UCharacterCreationUtility::UpdatePreviewData()
{
	if( !CharacterSpawned ) return;

	CharacterSpawned->SetFaceEffect( (float)Effect/10.f );
	CharacterSpawned->SetGlobalWetness( MainWetness );
	CharacterSpawned->SetHeightWetness( HeightWetness, HeightWetnessOpacity );
	CharacterSpawned->SetIsUnderRoof( UnderRoof );
	CharacterSpawned->SetHeightMaskGlobal( HeightMask );
	CharacterSpawned->SetSphereMaskGlobal( SphereMaskPosition, SphereMaskRadius );
	CharacterSpawned->SetSphereMaskHardness( SphereMaskHardness );

	for( uint8 I = 0; I < EyesBleeding.Num(); I++ )
	{
		TArray<int32> IndexArray;
		IndexArray.Add( I );
		CharacterSpawned->SetEyesBleeding( EyesBleeding[I], IndexArray );
	}

	for( uint8 I = 0; I < EyesEmissive.Num(); I++ )
	{
		TArray<int32> IndexArray;
		IndexArray.Add( I );
		CharacterSpawned->SetEyesEmissive( EyesEmissive[I], IndexArray );
	}

	for( uint8 I = 0; I < PupilScale.Num(); I++ )
	{
		TArray<int32> IndexArray;
		IndexArray.Add( I );
		CharacterSpawned->SetPupilScale( PupilScale[I], IndexArray );
	}
}

void UCharacterCreationUtility::BlinkButtonClicked()
{
	if( CharacterSpawned )
	{
		CharacterSpawned->bTestBlink_Editor = true;
	}
}