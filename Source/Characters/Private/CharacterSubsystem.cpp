// Copyright 0ne3y3 Lucas. All Rights Reserved.

#include "CharacterSubsystem.h"

FModularSkeletalMeshData& UCharacterSubsystem::GetDefaultModularSkeletalMeshData()
{
	return ModularSkeletalMeshData;
}

FModularMainBodyData& UCharacterSubsystem::GetDefaultModularMainBodyData()
{
	return ModularMainBodyData;
}

FModularStaticMeshData& UCharacterSubsystem::GetDefaultModularStaticMeshData()
{
	return ModularStaticMeshData;
}