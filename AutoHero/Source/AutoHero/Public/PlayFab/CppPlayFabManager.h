// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "CppPlayFabManager.generated.h"

UCLASS()
class AUTOHERO_API ACppPlayFabManager : public AActor
{
	GENERATED_BODY()
	
private:
	static ACppPlayFabManager* instance;
public:
	static ACppPlayFabManager* Instance();

public:	
	// Sets default values for this actor's properties
	ACppPlayFabManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	const FString titleID = "B7C3F";

	PlayFabClientPtr clientAPI;

#pragma region Login.
public:
	FString loginUserName;
	FString loginUserPassword;

	void GetLogin();
	void OnLoginSuccess(const PlayFab::ClientModels::FLoginResult& result) const;
	void OnLoginError(const PlayFab::FPlayFabCppError& errorResult) const;
#pragma endregion

#pragma region Register.
public:
	FString userEmail;
	FString registerUserPassword;
	FString registerUserName;

	void GetRegister();
	void OnRegisterSuccess(const PlayFab::ClientModels::FRegisterPlayFabUserResult& result) const;
	void OnRegisterError(const PlayFab::FPlayFabCppError& errorResult) const;

#pragma endregion


};
