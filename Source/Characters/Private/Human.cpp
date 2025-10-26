// Fill out your copyright notice in the Description page of Project Settings.


#include "Human.h"
#include "Engine/AssetManager.h"
#include "MuCO/CustomizableObject.h"
#include "MuCO/CustomizableObjectInstance.h"
#include "MuCO/CustomizableObjectInstanceUsage.h"
#include "MuCO/CustomizableSkeletalComponent.h"
#include "MuCO/CustomizableSkeletalMeshActor.h"
#include "ModularMeshData.h"
#include "CharacterSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC( LogCharacter, Log, All );

AHuman::AHuman( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	//SetActorHiddenInGame( true );

	#if WITH_EDITORONLY_DATA
	bRunConstructionScriptOnDrag = false;
	#endif

	BodySkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MainBodyComponent"));
	BodySkeletalMeshComponent->bPropagateCurvesToFollowers = true;
	RootComponent = BodySkeletalMeshComponent;

	bAllowTickBeforeBeginPlay = false;
	PrimaryActorTick.bCanEverTick = true;
}

bool AHuman::ShouldTickIfViewportsOnly() const
{
	if( GetWorld() != nullptr && GetWorld()->WorldType == EWorldType::Editor )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AHuman::BeginPlay()
{
	Super::BeginPlay();

	if(MutableBodyRowName.IsNone() || !MutableCharacterProperties) return;
	LoadAndChangeCharacterObjectInstance(MutableBodyRowName);
}

void AHuman::OnConstruction( const FTransform& Transform )
{
	Super::OnConstruction( Transform );

	if(MutableBodyRowName.IsNone() || !MutableCharacterProperties) return;
	LoadAndChangeCharacterObjectInstance(MutableBodyRowName);
}

void AHuman::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AHuman::LoadAndChangeCharacterObjectInstance(FName RowName)
{
	UCharacterSubsystem* CharacterSubsystem = GEngine->GetEngineSubsystem<UCharacterSubsystem>();
	if(CharacterSubsystem)
	{
		if(CharacterSubsystem->MutableCharacterTable)
		{
			static const FString ContextString(TEXT("Finding row in character data table"));
			FMutableCharacterObject* TableRow = CharacterSubsystem->MutableCharacterTable->FindRow<FMutableCharacterObject>(RowName, ContextString);
			
			if(TableRow)
			{
				TableRow->CustomObject.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateLambda(
					[WeakThis = MakeWeakObjectPtr(this)]
					(const FSoftObjectPath& InSoftObjectPath, UObject* InObject)
					{
						if(auto* This = WeakThis.Get())
						{
							This->MutableBodyObject = CastChecked<UCustomizableObject>(InObject, ECastCheckedType::NullAllowed);
							This->ConstructCharacter();
						}
					}));
			}
		}
	}
}

void AHuman::LoadAndChangeCharacterPreset(FName RowName)
{
}

void AHuman::ChangeBodyPreset(UMutableBodyPreset* BodyPreset, bool bUpdateMesh)
{
	CreateCustomMutableCharacterProperties(BodyPreset, MutableCharacterProperties->HeadPreset);

	InitBody(bUpdateMesh);
}

void AHuman::ChangeHeadPreset(UMutableHeadPreset* HeadPreset, bool bUpdateMesh)
{
	CreateCustomMutableCharacterProperties(MutableCharacterProperties->BodyPreset, HeadPreset);

	InitHead(bUpdateMesh);
}

void AHuman::CreateCustomMutableCharacterProperties(UMutableBodyPreset* BodyPreset, UMutableHeadPreset* HeadPreset)
{
	if(!MutableCharacterProperties->PresetName.IsEqual(TEXT("Custom")))
	{
		MutableCharacterProperties = NewObject<UMutableCharacterPreset>();
	}

	MutableCharacterProperties->BodyPreset = BodyPreset;
	MutableCharacterProperties->HeadPreset = HeadPreset;
}

void AHuman::ConstructCharacter()
{
	if(MutableBodyObject)
	{
		MutableBodyInstance = NewObject<UCustomizableObjectInstance>();
		MutableBodyInstance->SetObject(MutableBodyObject);

		MutableBodyComponent = NewObject<UCustomizableSkeletalComponent>(UCustomizableSkeletalComponent::StaticClass());
		MutableBodyComponent->SetComponentName(TEXT("MainBody"));
		MutableBodyComponent->AttachToComponent(BodySkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
		MutableBodyComponent->CustomizableObjectInstance = MutableBodyInstance;
		
		InitBody(false);
		InitHead(true);
	}
}

void AHuman::UpdateMutableSkinMaterial(FString RowName)
{
	//MutableBodyInstance->SetIntParameterSelectedOption(FString(""), RowName);
}

void AHuman::UpdateMutableSkinColor(FString RowName)
{
	MutableBodyInstance->SetIntParameterSelectedOption(FString("SkinColorTable"), RowName);
}

void AHuman::UpdateMutableHeadMesh(FString RowName)
{
	MutableBodyInstance->SetIntParameterSelectedOption(FString("HeadTable"), RowName);
}

void AHuman::UpdateMutableIrisTexture(FString RowName)
{
	MutableBodyInstance->SetIntParameterSelectedOption(FString("IrisTextureTable"), RowName);
}

void AHuman::UpdateMutableHighlightTexture(FString RowName)
{
	MutableBodyInstance->SetIntParameterSelectedOption(FString("HighlightTextureTable"), RowName);
}

void AHuman::UpdateMutableCETexture(FString RowName)
{
	MutableBodyInstance->SetIntParameterSelectedOption(FString("CETextureTable"), RowName);
}

void AHuman::UpdateMutableBodyMesh(FString RowName)
{
	MutableBodyInstance->SetIntParameterSelectedOption(FString("BodyTable"), RowName);
}

void AHuman::InitBody(bool bUpdateMesh)
{
	if(MutableBodyInstance)
	{
		UpdateMutableBodyMesh(MutableCharacterProperties->BodyPreset->BodyMeshRowName.ToString());
		//UpdateMutableSkinMaterial(MutableCharacterProperties->BodyPreset->SkinMaterialRowName.ToString());
		UpdateMutableSkinColor(MutableCharacterProperties->BodyPreset->SkinColorRowName.ToString());

		MutableBodyInstance->SetColorParameterSelectedOption(FString("NailsHandLeftColor"), MutableCharacterProperties->BodyPreset->BodyProperties.NailsHandLeftColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("NailsHandRightColor"), MutableCharacterProperties->BodyPreset->BodyProperties.NailsHandRightColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("NailsFootLeftColor"), MutableCharacterProperties->BodyPreset->BodyProperties.NailsFootLeftColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("NailsFootRightColor"), MutableCharacterProperties->BodyPreset->BodyProperties.NailsFootRightColor);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("SkinBaseSpecular"), MutableCharacterProperties->BodyPreset->BodyProperties.SkinBaseSpecular);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("SkinBaseRoughness"), MutableCharacterProperties->BodyPreset->BodyProperties.SkinBaseRoughness);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("SkinBaseMetallic"), MutableCharacterProperties->BodyPreset->BodyProperties.SkinBaseMetallic);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("SkinSubsurfaceOpacity"), MutableCharacterProperties->BodyPreset->BodyProperties.SkinSubsurfaceOpacity);
		
		if(bUpdateMesh && MutableBodyComponent) MutableBodyComponent->UpdateSkeletalMeshAsync();
	}
}

void AHuman::InitHead(bool bUpdateMesh)
{
	if(MutableBodyInstance)
	{
		UpdateMutableHeadMesh(MutableCharacterProperties->HeadPreset->HeadMeshRowName.ToString());
		UpdateMutableIrisTexture(MutableCharacterProperties->HeadPreset->IrisTextureRowName.ToString());
		UpdateMutableHighlightTexture(MutableCharacterProperties->HeadPreset->HighlightTextureRowName.ToString());
		UpdateMutableCETexture(MutableCharacterProperties->HeadPreset->CETextureRowName.ToString());

		MutableBodyInstance->SetColorParameterSelectedOption(FString("ScleraColor"), MutableCharacterProperties->HeadPreset->HeadProperties.ScleraColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("ScleraTextureColor"), MutableCharacterProperties->HeadPreset->HeadProperties.ScleraTextureColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("ScleraDarkCornerColor"), MutableCharacterProperties->HeadPreset->HeadProperties.ScleraDarkCornerColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("HighlightColor"), MutableCharacterProperties->HeadPreset->HeadProperties.HighlightColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("HighlightBorderColor"), MutableCharacterProperties->HeadPreset->HeadProperties.HighlightBorderColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("CameraEffectTopColor"), MutableCharacterProperties->HeadPreset->HeadProperties.CameraEffectTopColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("CameraEffectMiddleColor"), MutableCharacterProperties->HeadPreset->HeadProperties.CameraEffectMiddleColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("CameraEffectBottomColor"), MutableCharacterProperties->HeadPreset->HeadProperties.CameraEffectBottomColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("TeethColor"), MutableCharacterProperties->HeadPreset->HeadProperties.TeethColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("TongueColor"), MutableCharacterProperties->HeadPreset->HeadProperties.TongueColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("MouthInsideColor"), MutableCharacterProperties->HeadPreset->HeadProperties.MouthInsideColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("CheekColor"), MutableCharacterProperties->HeadPreset->HeadProperties.CheekColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("LipsColor"), MutableCharacterProperties->HeadPreset->HeadProperties.LipsColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("EyelidColor"), MutableCharacterProperties->HeadPreset->HeadProperties.EyelidColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("NoseColor"), MutableCharacterProperties->HeadPreset->HeadProperties.NoseColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("EyelashBaseColor"), MutableCharacterProperties->HeadPreset->HeadProperties.EyelashBaseColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("EyelashSecondaryColor"), MutableCharacterProperties->HeadPreset->HeadProperties.EyelashSecondaryColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("EyelashTertiaryColor"), MutableCharacterProperties->HeadPreset->HeadProperties.EyelashTertiaryColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("EyebrowBaseColor"), MutableCharacterProperties->HeadPreset->HeadProperties.EyebrowBaseColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("EyebrowSecondaryColor"), MutableCharacterProperties->HeadPreset->HeadProperties.EyebrowSecondaryColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("EyebrowTertiaryColor"), MutableCharacterProperties->HeadPreset->HeadProperties.EyebrowTertiaryColor);
		MutableBodyInstance->SetColorParameterSelectedOption(FString("BeardColor"), MutableCharacterProperties->HeadPreset->HeadProperties.BeardColor);
		
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("EyeColorCurveIndex"), (float)MutableCharacterProperties->HeadPreset->HeadProperties.EyeColorCurveIndex);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("EyeMetallic"), MutableCharacterProperties->HeadPreset->HeadProperties.EyeMetallic);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("IrisSpecular"), MutableCharacterProperties->HeadPreset->HeadProperties.IrisSpecular);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("IrisRoughness"), MutableCharacterProperties->HeadPreset->HeadProperties.IrisRoughness);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("IrisHueShift"), MutableCharacterProperties->HeadPreset->HeadProperties.IrisHueShift);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("ScleraDarkCornerHardness"), MutableCharacterProperties->HeadPreset->HeadProperties.ScleraDarkCornerHardness);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("ScleraSpecular"), MutableCharacterProperties->HeadPreset->HeadProperties.ScleraSpecular);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("ScleraRoughness"), MutableCharacterProperties->HeadPreset->HeadProperties.ScleraRoughness);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("CameraEffectOpacity"), MutableCharacterProperties->HeadPreset->HeadProperties.CameraEffectOpacity);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("CameraEffectSpeed"), MutableCharacterProperties->HeadPreset->HeadProperties.CameraEffectSpeed);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("LipsLineDarkness"), MutableCharacterProperties->HeadPreset->HeadProperties.LipsLineDarkness);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("MouthAndTongueSpecular"), MutableCharacterProperties->HeadPreset->HeadProperties.MouthAndTongueSpecular);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("MouthAndTongueRoughness"), MutableCharacterProperties->HeadPreset->HeadProperties.MouthAndTongueRoughness);
		MutableBodyInstance->SetFloatParameterSelectedOption(FString("BeardAndHairShavedOpacity"), MutableCharacterProperties->HeadPreset->HeadProperties.BeardAndHairShavedOpacity);

		if(bUpdateMesh && MutableBodyComponent) MutableBodyComponent->UpdateSkeletalMeshAsync();
	}
}
