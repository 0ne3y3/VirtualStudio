// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ModularSkeletalMeshComponent.generated.h"

/**
 * Component class to holding a body part of a modular character.
 */
UCLASS( ClassGroup=( ModularCharacter ), meta=(BlueprintSpawnableComponent) )
class CHARACTERS_API UModularSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

	UModularSkeletalMeshComponent( const FObjectInitializer& ObjectInitializer );

	public:	

	UFUNCTION( BlueprintCallable )
	void SetTextureParameterOnDynamicMaterials(FName MaterialName, UTexture2D* Texture2D );

	UFUNCTION( BlueprintCallable )
	void SetModularSkeletalMesh(USkeletalMesh* InSkeletalMesh);

	UFUNCTION( BlueprintCallable )
	TArray<UMaterialInstanceDynamic*>& GetAllDynamicMaterialInstances();

	UFUNCTION( BlueprintCallable )
	UMaterialInstanceDynamic* GetDynamicMaterialInstance( int32 MatIndex );

	UFUNCTION( BlueprintCallable )
	void SetDynamicMaterialInstance( int32 MatIndex, UMaterialInstanceDynamic* InDynamicMaterialInstance );

	UFUNCTION( BlueprintCallable )
	void SetDynamicMaterialInstanceByName( FName SlotName, UMaterialInstanceDynamic* InDynamicMaterialInstance );

	void UpdateDynamicMaterialInstance( int32 NumbersOfEntries );

	UFUNCTION( BlueprintCallable, BlueprintSetter )
	void SetIsFullySetup(bool inIsFullSetup );

	UFUNCTION( BlueprintCallable, BlueprintGetter)
	bool IsAllSetup();

	protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterialInstances;

	UPROPERTY(BlueprintReadWrite, BlueprintGetter = IsAllSetup, BlueprintSetter = SetIsFullySetup )
	bool bIsFullySetup = false;
};
