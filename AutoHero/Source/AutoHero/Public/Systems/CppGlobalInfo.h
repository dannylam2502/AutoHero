// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AUTOHERO_API CppGlobalInfo
{
private:
	static CppGlobalInfo* i;
public:
	static CppGlobalInfo* I();

public:
	CppGlobalInfo();
	~CppGlobalInfo();

};