// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularSkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC( LogCharacter, Log, All );

// Sets default values for this component's properties
UModularSkeletalMeshComponent::UModularSkeletalMeshComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UModularSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UModularSkeletalMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

USkeletalMeshComponent* UModularSkeletalMeshComponent::InitializeSkeletalMeshComponent( AActor* Outer, FName BodyPartName )
{
	if( !Outer ) return nullptr;

	// Append "_skeletal" at the end of the component name
	BodyName = FName(FString::Printf(TEXT("%s_Skeletal"), *BodyPartName.ToString()));

	// try to create a new component
	BodyPartComponent = NewObject<USkeletalMeshComponent>( Outer, USkeletalMeshComponent::StaticClass(), BodyName );

	if( BodyPartComponent != nullptr )
	{
		// If Succeed, configure its default behavior.
		BodyName = BodyPartName;
		BodyPartComponent->bUseAttachParentBound = true;
		BodyPartComponent->bUseBoundsFromLeaderPoseComponent = true;
		Outer->RemoveOwnedComponent( BodyPartComponent );
		BodyPartComponent->CreationMethod = EComponentCreationMethod::Instance;
		Outer->AddOwnedComponent( BodyPartComponent );
		BodyPartComponent->RegisterComponent();

		TArray<USceneComponent*> ChildrenComponents;
		USceneComponent* OuterMainBody = nullptr;
		Outer->GetRootComponent()->GetChildrenComponents(false, ChildrenComponents);
		for( USceneComponent* ChildComponent : ChildrenComponents )
		{
			if(ChildComponent->GetFName() == TEXT( "MainBodyMesh" ) ) OuterMainBody = ChildComponent;
		}

		BodyPartComponent->AttachToComponent( OuterMainBody, FAttachmentTransformRules::SnapToTargetIncludingScale );
		RefreshLeaderComponent( Cast<USkeletalMeshComponent>( OuterMainBody ) );

		return BodyPartComponent;
	}

	UE_LOG( LogCharacter, Error, TEXT( "Failed to create USkeletalMeshComponent for %s." ), *BodyName.ToString() );
	return nullptr;
}

USkeletalMeshComponent* UModularSkeletalMeshComponent::GetBodyPartComponent()
{
	return BodyPartComponent;
}

FName UModularSkeletalMeshComponent::GetBodyName()
{
	return BodyName;
}

bool UModularSkeletalMeshComponent::SetSkeletalMesh( USkeletalMesh* Mesh )
{
	if( Mesh )
	{
		BodyPartComponent->SetSkeletalMesh( Mesh );
		return true;
	}
	return false;
}

bool UModularSkeletalMeshComponent::SetSkeletalMeshMaterials( TArray<UMaterialInstance*>& Materials )
{
	bool DoesOneMaterialFailed = false;
	for( int32 MaterialIndex = 0; MaterialIndex < Materials.Num(); MaterialIndex++ )
	{
		if( Materials[MaterialIndex] != nullptr )
		{
			BodyPartComponent->SetMaterial(MaterialIndex, Materials[MaterialIndex] );
		}
		else
		{
			DoesOneMaterialFailed = true;
		}
	}

	return !DoesOneMaterialFailed;
}

void UModularSkeletalMeshComponent::OnComponentDestroyed( bool bDestroyingHierarchy )
{
	if( BodyPartComponent ) BodyPartComponent->DestroyComponent();
	Super::OnComponentDestroyed( bDestroyingHierarchy );
}

void UModularSkeletalMeshComponent::RefreshLeaderComponent( USkeletalMeshComponent* MainBodyComponent )
{
	if( BodyPartComponent )
	{
		if( !BodyPartComponent->HasValidAnimationInstance() ) BodyPartComponent->SetLeaderPoseComponent( MainBodyComponent );
	}
}