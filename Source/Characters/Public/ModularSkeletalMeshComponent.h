// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ModularSkeletalMeshComponent.generated.h"

/**
 * Component class to holding a body part of a modular character.
 */
UCLASS( ClassGroup=( Custom ), meta=(BlueprintSpawnableComponent) )
class CHARACTERS_API UModularSkeletalMeshComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UModularSkeletalMeshComponent();

	USkeletalMeshComponent* GetBodyPartComponent();

	FName GetBodyName();

	UFUNCTION()
	bool SetSkeletalMesh( USkeletalMesh* Mesh );

	UFUNCTION()
	bool SetSkeletalMeshMaterials( TArray<UMaterialInstance*>& Materials );

	UFUNCTION()
	USkeletalMeshComponent* InitializeSkeletalMeshComponent( AActor* Outer, FName BodyPartName );

	UFUNCTION()
	void RefreshLeaderComponent( USkeletalMeshComponent* MainBodyComponent );

	/**
	 * Called when a component is destroyed
	 *
	 * @param	bDestroyingHierarchy  - True if the entire component hierarchy is being torn down, allows avoiding expensive operations
	 */
	virtual void OnComponentDestroyed( bool bDestroyingHierarchy ) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<USkeletalMeshComponent> BodyPartComponent;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	FName BodyName;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
