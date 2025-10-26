// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularMeshData.h"
#include "CharacterSubsystem.h"
#include "CharacterSettings.h"

DEFINE_LOG_CATEGORY_STATIC( LogCharacter, Log, All );

FMutableIrisTexture::FMutableIrisTexture()
{
	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();
	if(!CharacterSettings) return;

	IrisTexture = CharacterSettings->DefaultTexture;
	HeightTexture = CharacterSettings->DefaultGrayscaleTexture;
}

FMutableHighlightTexture::FMutableHighlightTexture()
{
	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();
	if(!CharacterSettings) return;

	HighlightTexture = CharacterSettings->DefaultGrayscaleTexture;
}

FMutableCETexture::FMutableCETexture()
{
	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();
	if(!CharacterSettings) return;

	CETexture = CharacterSettings->DefaultGrayscaleTexture;
}

FMutableHumanHead::FMutableHumanHead() :
	BodyType(EBodyType::Female),
	FaceEffectMin(163.5f),
	FaceEffectMax(172.5f),
	NoseMaskContrast(20.f),
	IrisDefaultPositionX(0.2944f),
	IrisDefaultPositionY(0.041f),
	EyesScaleX(0.64f),
	EyesScaleY(0.65f),
	HighlightMaxScaleX(0.200000f),
	HighlightMaxScaleY(0.150000f),
	HighlightMaxRotation(0.025000f),
	ScleraDefaultDarkCornerOffsetY(0.517987f),
	ScleraDarkCornerEyeCloseWeight(0.2f),
	ScleraDarkCornerEyeWideWeight(0.0258f),
	PupilOffsetMin(0.650000f),
	PupilOffsetMax(-0.400000f)
{
	IrisMaxPositionInOut = FVector2D(0.492f, 0.1144f);
	IrisMaxPositionUpDown = FVector2D(0.1938f, -0.143f);
	CameraEffectTransform = FLinearColor(1.800000f, 1.800000f, 0.000000f, -0.397200f);
	ScleraTextureTransform = FLinearColor(0.982837f, 2.061069f, 0.121600f, -0.104000f);

	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();
	if(!CharacterSettings) return;

	HeadMesh = CharacterSettings->DefaultMutableHeadMesh;
	DefaultHeadMaterial = CharacterSettings->DefaultMutableHeadMat;
	HeadTexture = CharacterSettings->DefaultGrayscaleTexture;
	DefaultEyeMaterial = CharacterSettings->DefaultMutableEyeMat;
	DefaultDetailsTexture = CharacterSettings->DefaultGrayscaleTexture;
	DefaultScleraTexture = CharacterSettings->DefaultGrayscaleTexture;
	HairShaved = CharacterSettings->DefaultGrayscaleTexture;
	HeadMask = CharacterSettings->DefaultGrayscaleTexture;
}

FMutableHumanHeadProperties::FMutableHumanHeadProperties() :
	EyeColorCurveIndex(3),
	EyesMinimumScale(0.45f),
	EyeMetallic(0.f),
	IrisSpecular(0.35f),
	IrisRoughness(0.6f),
	IrisHueShift(1.f),
	ScleraDarkCornerHardness(16.f),
	ScleraSpecular(0.35f),
	ScleraRoughness(0.6f),
	CameraEffectOpacity(0.5f),
	CameraEffectSpeed(0.3f),
	LipsLineDarkness(1.f),
	MouthAndTongueSpecular(0.28f),
	MouthAndTongueRoughness(0.8f),
	BeardAndHairShavedOpacity(0.f)
{
	TeethColor = FLinearColor(0.838542, 0.838542, 0.838542, 1.000000);
	TongueColor = FLinearColor(0.760417, 0.304959, 0.328074, 1.000000);
	MouthInsideColor = FLinearColor(0.447917, 0.093043, 0.118731, 1.000000);
	CheekColor = FLinearColor(0.968750, 0.456204, 0.388509, 1.000000);
	LipsColor = FLinearColor(0.817708, 0.147642, 0.136285, 1.000000);
	EyelidColor = FLinearColor(0.656250, 0.401823, 0.352051, 1.000000);
	NoseColor = FLinearColor(0.052083, 0.004944, 0.000000, 1.000000);
	EyelashBaseColor = FLinearColor(0.007018, 0.004053, 0.010000, 1.000000);
	EyelashSecondaryColor = FLinearColor(0.125000, 0.001378, 0.013216, 1.000000);
	EyelashTertiaryColor = FLinearColor(0.239583, 0.109809, 0.163996, 1.000000);
	EyebrowBaseColor = FLinearColor(0.245810, 0.142822, 0.609375, 1.000000);
	EyebrowSecondaryColor = FLinearColor(0.245810, 0.142822, 0.609375, 1.000000);
	EyebrowTertiaryColor = FLinearColor(0.245810, 0.142822, 0.609375, 1.000000);
	BeardColor = FLinearColor(0.012000, 0.008400, 0.005188, 0.500000);
	
	ScleraColor = FLinearColor(0.800082, 0.785775, 0.807292, 1.000000);
	ScleraTextureColor = FLinearColor(0.880208, 0.027797, 0.018338, 0.00000);
	ScleraDarkCornerColor = FLinearColor(0.370942, 0.342773, 0.421875, 1.000000);
	HighlightColor = FLinearColor(0.765625, 0.765625, 0.733724, 1.000000);
	HighlightBorderColor = FLinearColor(0.703125, 0.117188, 0.212933, 1.000000);
	CameraEffectTopColor = FLinearColor(0.401042, 0.158746, 0.334816, 1.000000);
	CameraEffectMiddleColor = FLinearColor(0.864583, 0.279188, 0.697303, 1.000000);
	CameraEffectBottomColor = FLinearColor(0.963542, 0.778985, 0.506863, 1.000000);
}

FMutableHumanSkin::FMutableHumanSkin() :
	SkinColorCurveIndex(0.f),
	SkinLineartDarkness(0.45f),
	SkinOutlineDarkness(0.1f)
{
	SkinName = FName("White1");
	SkinSubsurfaceTintColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
	FaceEffectYandereColor = FLinearColor(0.066727, 0.057818, 0.093750, 1.00000);
	FaceEffectMadColor = FLinearColor(0.671875, 0.044764, 0.048558, 1.000000);
}

FMutableHumanBody::FMutableHumanBody() :
	BodyType(EBodyType::Female)
{
	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();

	if(!CharacterSettings) return;

	BobyMesh = CharacterSettings->DefaultMutableBodyMesh;
	BodyDefaultMaterial = CharacterSettings->DefaultMutableSkinMat;
	GrayscaleSkinTexture = CharacterSettings->DefaultGrayscaleTexture;
	NailsMaskTexture = CharacterSettings->DefaultGrayscaleTexture;
	TopHeadTexture = CharacterSettings->DefaultGrayscaleTexture;
}

FMutableHumanBodyProperties::FMutableHumanBodyProperties() :
	SkinBaseMetallic(0.f),
	SkinBaseSpecular(0.f),
	SkinBaseRoughness(1.f),
	SkinSubsurfaceOpacity(1.f)
{
	NailsHandLeftColor = FLinearColor(1.000000, 0.968414, 0.958769, 1.000000);
	NailsHandRightColor = FLinearColor(1.000000, 0.968414, 0.958769, 1.000000);
	NailsFootLeftColor = FLinearColor(1.000000, 0.968414, 0.958769, 1.000000);
	NailsFootRightColor = FLinearColor(1.000000, 0.968414, 0.958769, 1.000000);
}

UMutableHeadPreset::UMutableHeadPreset()
{
	PresetName = FName("Custom");
	HeadMeshRowName = FName("Riko");
	IrisTextureRowName = FName("Riko");
	HighlightTextureRowName = FName("Riko");
	CETextureRowName = FName("Riko");
	HeadProperties = FMutableHumanHeadProperties();
}

UMutableBodyPreset::UMutableBodyPreset()
{
	PresetName = FName("Custom");
	BodyMeshRowName = FName("Female_Small");
	SkinColorRowName = FName("White2");
	BodyProperties = FMutableHumanBodyProperties();
}

UMutableCharacterPreset::UMutableCharacterPreset()
{
	PresetName = FName("Custom");
}
