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

	UpdateButton->OnClicked.AddUniqueDynamic(this, &UCharacterCreationUtility::UpdateButtonClicked);
	SaveButton->OnClicked.AddUniqueDynamic( this, &UCharacterCreationUtility::SaveButtonClicked );
	LoadButton->OnClicked.AddUniqueDynamic( this, &UCharacterCreationUtility::LoadButtonClicked );
}

void UCharacterCreationUtility::UpdateButtonClicked()
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

		;
		if( USkeletalMeshComponent* MainBodyComponent = CharacterSpawned->GetMainBodyComponent() )
		{
			MainBodyComponent->SetAnimInstanceClass( AnimationBP );
		}

		CharacterSpawned->SetHumanBodyData( CharacterProperties );
		CharacterSpawned->OnConstruction( FTransform() );

		if( AnimationPreview && IsAnimSequenceCompatible() )
		{
			CharacterSpawned->UpdateMainBodyAnimation( AnimationPreview, bShowControlRig, AnimPlayRate );
		}
		else
		{
			CharacterSpawned->UpdateMainBodyAnimation( nullptr, bShowControlRig, AnimPlayRate );
		}
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

		// update ?
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