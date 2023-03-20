// Fill out your copyright notice in the Description page of Project Settings.


#include "LiveLinkHuman.h"
#include "Features/IModularFeatures.h"
#include "Roles/LiveLinkBasicRole.h"
#include "ILiveLinkClient.h"

DEFINE_LOG_CATEGORY_STATIC( LogCharacter, Log, All );

ALiveLinkHuman::ALiveLinkHuman( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{

	IModularFeatures& ModularFeatures = IModularFeatures::Get();
	if( ModularFeatures.IsModularFeatureAvailable( ILiveLinkClient::ModularFeatureName ) )
	{
		CachedLiveLinkClient = &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>( ILiveLinkClient::ModularFeatureName );
	}
}

// Called every frame
void ALiveLinkHuman::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if( bUseHeadRotation )
	{
		if( CachedLiveLinkClient && FaceARKitSubjectName.IsValid() && !FaceARKitSubjectName.IsNone() )
		{
			FLiveLinkSubjectFrameData FaceARKitBasicData;
			if( CachedLiveLinkClient->EvaluateFrame_AnyThread( FaceARKitSubjectName, ULiveLinkBasicRole::StaticClass(), FaceARKitBasicData ) )
			{
				float TempHeadYaw = 0;
				float TempHeadPitch = 0;
				float TempHeadRoll = 0;

				FLiveLinkBaseStaticData* StaticData = FaceARKitBasicData.StaticData.Cast<FLiveLinkBaseStaticData>();
				FLiveLinkBaseFrameData* FrameData = FaceARKitBasicData.FrameData.Cast<FLiveLinkBaseFrameData>();

				StaticData->FindPropertyValue( *FrameData, "headYaw", TempHeadYaw );
				StaticData->FindPropertyValue( *FrameData, "headPitch", TempHeadPitch );
				StaticData->FindPropertyValue( *FrameData, "headRoll", TempHeadRoll );

				HeadRoll = TempHeadYaw * 50.f;
				HeadPitch = TempHeadRoll * 50.f;
				HeadYaw = TempHeadPitch * -50.f;
			}
		}
	}
	else
	{
		HeadRoll = 0;
		HeadPitch = 0;
		HeadYaw = 0;
	}
}