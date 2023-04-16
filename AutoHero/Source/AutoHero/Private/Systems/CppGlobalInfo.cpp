// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CppGlobalInfo.h"

CppGlobalInfo* CppGlobalInfo::i;
CppGlobalInfo* CppGlobalInfo::I()
{
	return i;
}

CppGlobalInfo::CppGlobalInfo()
{
	i = this;
}

CppGlobalInfo::~CppGlobalInfo()
{
}
