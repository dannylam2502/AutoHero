// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "Core/PlayFabMultiplayerDataModels.h"
#include "CppPlayFabManager.generated.h"

using namespace PlayFab;
using namespace PlayFab::ClientModels;
using namespace PlayFab::MultiplayerModels;

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
	PlayFabMultiplayerPtr multiplayerAPI;

#pragma region Login.
public:
	FString loginUserName;
	FString loginUserPassword;

	void GetLogin();
	void OnLoginSuccess(const FLoginResult& result) const;
	void OnLoginError(const FPlayFabCppError& errorResult) const;
#pragma endregion

#pragma region Register.
public:
	FString userEmail;
	FString registerUserPassword;
	FString registerUserName;

	void GetRegister();
	void OnRegisterSuccess(const FRegisterPlayFabUserResult& result) const;
	void OnRegisterError(const FPlayFabCppError& errorResult) const;

#pragma endregion

#pragma region Matchmaking.
public:
	// Create matchmaking ticket.
	void CreateMatchmakingTicket();
	void SuccessCreateMatchmakingTicket(const FCreateMatchmakingTicketResult& result) const;
	void ErrorCreateMatchmakingTicket(const FPlayFabCppError& errorResult) const;
	//.

	// Get matchmaking ticket.
	void GetMatchmakingTicket();
	void SuccessGetMatchmakingTicket(const FGetMatchmakingTicketResult& result) const;
	void ErrorGetMatchmakingTicket(const FPlayFabCppError& errorResult) const;
	//.

	// Get match.
	void GetMatch();
	void SuccessGetMatch(const FGetMatchResult& result) const;
	void ErrorGetMatch(const FPlayFabCppError& errorResult) const;
	//.

#pragma endregion

};
