// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularSkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC( LogCharacter, Log, All );

UModularSkeletalMeshComponent::UModularSkeletalMeshComponent( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	SetComponentTickEnabled(false);
}

void UModularSkeletalMeshComponent::SetTextureParameterOnDynamicMaterials( FName MaterialName, UTexture2D* Texture2D )
{
	for( auto DynMaterial : DynamicMaterialInstances )
	{
		if( DynMaterial ) DynMaterial->SetTextureParameterValue( MaterialName , Texture2D );
	}
}

void UModularSkeletalMeshComponent::SetModularSkeletalMesh(USkeletalMesh* InSkeletalMesh)
{
	SetSkeletalMesh( InSkeletalMesh );
	UpdateDynamicMaterialInstance( InSkeletalMesh->GetNumMaterials() );
}

void UModularSkeletalMeshComponent::UpdateDynamicMaterialInstance( int32 NumbersOfEntries )
{
	if( NumbersOfEntries != DynamicMaterialInstances.Num() )
	{
		DynamicMaterialInstances.Empty();
		DynamicMaterialInstances.Init(nullptr, NumbersOfEntries);
	}
	else
	{
		DynamicMaterialInstances.Reset();
	}
}

void UModularSkeletalMeshComponent::SetIsFullySetup( bool inIsFullSetup )
{
	bIsFullySetup = inIsFullSetup;
}

bool UModularSkeletalMeshComponent::IsAllSetup()
{
	return bIsFullySetup;
}

TArray<UMaterialInstanceDynamic*>& UModularSkeletalMeshComponent::GetAllDynamicMaterialInstances()
{
	return DynamicMaterialInstances;
}

UMaterialInstanceDynamic* UModularSkeletalMeshComponent::GetDynamicMaterialInstance( int32 MatIndex )
{
	return ( DynamicMaterialInstances.IsValidIndex(MatIndex) ) ? DynamicMaterialInstances[MatIndex] : nullptr;
}

void UModularSkeletalMeshComponent::SetDynamicMaterialInstance( int32 MatIndex, UMaterialInstanceDynamic* InDynamicMaterialInstance )
{
	if( DynamicMaterialInstances.IsValidIndex( MatIndex ) )
	{
		DynamicMaterialInstances[MatIndex] = InDynamicMaterialInstance;
		SetMaterial(MatIndex, InDynamicMaterialInstance);
	}
}

void UModularSkeletalMeshComponent::SetDynamicMaterialInstanceByName( FName SlotName, UMaterialInstanceDynamic* InDynamicMaterialInstance )
{
	int32 SlotIndex = GetMaterialIndex( SlotName );
	if( DynamicMaterialInstances.IsValidIndex( SlotIndex ) )
	{
		DynamicMaterialInstances[SlotIndex] = InDynamicMaterialInstance;
		SetMaterial( SlotIndex, InDynamicMaterialInstance );
	}
}
