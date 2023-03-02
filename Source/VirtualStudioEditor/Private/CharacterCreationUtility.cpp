// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCreationUtility.h"
#include "Human.h"
#include "Components/DetailsView.h"
#include "Components/SinglePropertyView.h"
#include "Components/Button.h"

#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Engine/Selection.h"
#include "EditorAssetLibrary.h"

void UCharacterCreationUtility::DestroyCharacter()
{
	if( CharacterSpawned )
	{
		CharacterSpawned->Destroy();
	}

	CharacterSpawned = nullptr;
	CharacterDataAsset = nullptr;
}

void UCharacterCreationUtility::InitializeEditorWidget()
{
	CharacterDetailsView->PropertiesToShow.Add( GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, CharacterProperties ) );

	AnimationPreviewView->SetPropertyName( GET_MEMBER_NAME_CHECKED( UCharacterCreationUtility, AnimationPreview ) );

	UpdateButton->OnClicked.AddUniqueDynamic(this, &UCharacterCreationUtility::UpdateButtonClicked);
	SaveButton->OnClicked.AddUniqueDynamic( this, &UCharacterCreationUtility::SaveButtonClicked );
	LoadButton->OnClicked.AddUniqueDynamic( this, &UCharacterCreationUtility::LoadButtonClicked );
}

void UCharacterCreationUtility::UpdateButtonClicked()
{

	if( UWorld* TheWorld = GetWorld() )
	{
		if( !CharacterDataAsset )
		{
			CharacterDataAsset = NewObject<UHumanBodyData>( this, "CharacterDataAsset" );
		}

		CharacterDataAsset->CopyDataFromInterface( CharacterProperties );

		if( !CharacterSpawned )
		{
			FActorSpawnParameters Parameters;
			Parameters.bDeferConstruction = 1;
			CharacterSpawned = TheWorld->SpawnActor<AHuman>( Parameters );
		}

		if( !CharacterSpawned->GetHumanBodyData() ) CharacterSpawned->SetHumanBodyData( CharacterDataAsset );

		CharacterSpawned->OnConstruction( FTransform() );

		if( AnimationPreview && IsAnimSequenceCompatible() )
		{
			if( USkeletalMeshComponent* MainBody = CharacterSpawned->GetMainBody() )
			{
				GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, TEXT("hey"));
				FSingleAnimationPlayData AnimationData;
				AnimationData.AnimToPlay = AnimationPreview;
				MainBody->AnimationData = AnimationData;
				MainBody->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode );
			}
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

	UHumanBodyData* SelectedAssetData = nullptr;
	for( FAssetData AssetData : AssetDatas )
	{
		if( AssetData.GetClass() == UHumanBodyData::StaticClass() )
		{
			SelectedAssetData = CastChecked<UHumanBodyData>( UEditorAssetLibrary::LoadAsset( AssetData.GetObjectPathString() ) );
			if( SelectedAssetData ) break;
		}
	}

	if( SelectedAssetData )
	{
		if( !CharacterDataAsset )
		{
			CharacterDataAsset = NewObject<UHumanBodyData>( this, "CharacterDataAsset" );
		}

		CharacterDataAsset->CopyDataFromAnotherDataAsset( SelectedAssetData );

		CharacterDataAsset->CopyDataToInterface( CharacterProperties );
	}
	

}

bool UCharacterCreationUtility::IsAnimSequenceCompatible()
{

	FBodyMesh* BodyMesh = CharacterDataAsset->GetMainBody();

	if( BodyMesh && BodyMesh->MeshData && BodyMesh->MeshData->Mesh )
	{
		if( BodyMesh->MeshData->Mesh->GetSkeleton()->IsCompatibleSkeletonByAssetData( FAssetData( AnimationPreview ) ) )
		{
			return true;
		}
	}
	
	return false;
}