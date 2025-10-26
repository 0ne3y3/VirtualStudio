// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Human.h"
#include "HumanAnimationData.h"
#include "LiveLinkHuman.generated.h"


/**
 * 
 */
UCLASS()
class CHARACTERS_API ALiveLinkHuman : public AHuman
{
	GENERATED_BODY()

	/** Default UObject constructor. */
	ALiveLinkHuman( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

	public:
	//FORCEINLINE void SetHeadAnimationData( UARKitPresetData* InFaceARKitData ) override { FaceARKitData = InFaceARKitData; };

	// Called every frame
	virtual void Tick( float DeltaTime ) override;

	protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
	bool bUseHeadRotation = true;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
	FName FaceARKitSubjectName;

	UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "ARKitData" )
	FRotator HeadRotation;

	private:
	class ILiveLinkClient* CachedLiveLinkClient;
};
