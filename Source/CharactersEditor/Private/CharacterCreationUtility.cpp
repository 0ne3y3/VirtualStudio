// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCreationUtility.h"
#include "Human.h"
#include "Components/DetailsView.h"
#include "Components/Button.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Engine/Selection.h"
#include "EditorAssetLibrary.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "HumanAnimationData.h"

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
			UpdateEyesArray();
		}
	}
}

void UCharacterCreationUtility::UpdateCharacter()
{
	if( UWorld* TheWorld = GetWorld() )
	{
		if( !CharacterClass ) CharacterClass = AHuman::StaticClass();

		if( CharacterSpawned && CharacterSpawned->StaticClass() != CharacterClass )
		{
			DestroyCharacter();
		}

		if( !CharacterSpawned )
		{
			FActorSpawnParameters Parameters;
			Parameters.bDeferConstruction = 1;
			CharacterSpawned = TheWorld->SpawnActor<AHuman>( CharacterClass, Parameters );

		}

		CharacterSpawned->SetHumanBodyData( CharacterProperties );
		CharacterSpawned->ResetFirstInit();
		CharacterSpawned->OnConstruction( FTransform() );
		
		if( USkeletalMeshComponent* MainBodyComponent = CharacterSpawned->GetMainBodyComponent() )
		{
			MainBodyComponent->SetAnimInstanceClass( AnimationBP );
		}

		if( AnimationPreview && IsAnimSequenceCompatible() )
		{
			CharacterSpawned->UpdateMainBodyAnimation( AnimationPreview, AnimPlayRate, ARKitName );
		}
		else
		{
			CharacterSpawned->UpdateMainBodyAnimation( nullptr, AnimPlayRate, ARKitName );
		}
	}
}

void UCharacterCreationUtility::OnPropertyChangedMainPanel(FName PropertyName)
{
	UpdateCharacter();
}

void UCharacterCreationUtility::OnPropertyChangedEditorPreview( FName PropertyName )
{
	if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, CryingEffect ) )
	{
		CharacterSpawned->SetCryingEffect( CryingEffect );
	}
	else if( PropertyName == GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, YanMadEffect ) )
	{
		CharacterSpawned->SetYanMadEffect(YanMadEffect);
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
		CharacterSpawned->SetHeightMask( HeightMask );
	}
	else if( PropertyName == TEXT("X") || PropertyName == TEXT( "Y" ) || PropertyName == TEXT( "Z" ) || PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterCreationUtility, SphereMaskRadius) )
	{
		CharacterSpawned->SetSphereMask( SphereMaskPosition , SphereMaskRadius );
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
	UE_LOG( LogCharacter, Log, TEXT( "%s" ), *PropertyName.ToString() );
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

	FModularSkeletalMeshData& BodyMesh = CharacterProperties.MainBody;

	if( BodyMesh.MeshData->Mesh )
	{
		USkeletalMesh* SkeletalMesh = BodyMesh.MeshData->Mesh.LoadSynchronous();
		if( SkeletalMesh )
		{
			if( SkeletalMesh->GetSkeleton()->IsCompatibleSkeletonByAssetData( FAssetData( AnimationPreview ) ) )
			{
				return true;
			}
		}
	}
	
	return false;
}

void UCharacterCreationUtility::UpdateEyesArray()
{
	FModularSkeletalMeshData ModularSkeletalMeshData = CharacterProperties.GetHeadData();

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