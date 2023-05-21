// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CppGlobalInfo.h"

UCppGlobalInfo* UCppGlobalInfo::i;
UCppGlobalInfo* UCppGlobalInfo::I()
{
	return i;
}

const FName UCppGlobalInfo::nameLevelMain = TEXT("Main");
const FName UCppGlobalInfo::nameLevelGameplay = TEXT("Gameplay");

UCppGlobalInfo::UCppGlobalInfo()
{
	i = this;
}

UCppGlobalInfo::~UCppGlobalInfo()
{
	i = nullptr;
}
