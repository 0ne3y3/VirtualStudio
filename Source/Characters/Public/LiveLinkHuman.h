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
	FORCEINLINE void SetHeadAnimationData( UARKitPresetData* InFaceARKitData ) override { FaceARKitData = InFaceARKitData; };

	FORCEINLINE UARKitPresetData* GetHeadAnimationData() const override { return FaceARKitData; };

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
	bool bUseHeadRotation = true;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
	bool bReinitializeCurveData;

	//Cached information to generate LiveLinkData
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
	FName FaceARKitSubjectName;

	// Called every frame
	virtual void Tick( float DeltaTime ) override;

	protected:
	UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "ARKitData" )
	float HeadYaw;

	UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "ARKitData" )
	float HeadPitch;

	UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "ARKitData" )
	float HeadRoll;

	UPROPERTY( VisibleDefaultsOnly, BlueprintReadWrite, Category = "Animation" )
	TObjectPtr<UARKitPresetData> FaceARKitData;

	class ILiveLinkClient* CachedLiveLinkClient;
};
