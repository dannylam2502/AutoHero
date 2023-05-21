// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "CppMultiplayerManager.generated.h"

class ACppSpawnCharacterManager;

UCLASS()
class AUTOHERO_API ACppMultiplayerManager : public AActor
{
	GENERATED_BODY()
	
private:
	static ACppMultiplayerManager* i;
public:
	static ACppMultiplayerManager* I();

public:	
	// Sets default values for this actor's properties
	ACppMultiplayerManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Session")
		bool CreateSession(FName SessionName, int32 MaxNumPlayers, bool bIsLAN);

	UFUNCTION(BlueprintCallable, Category = "Session")
		void RegisterPlayer(FName SessionName);

	UFUNCTION(BlueprintCallable, Category = "Session")
		void UnregisterPlayer(FName SessionName);

	UFUNCTION(BlueprintCallable, Category = "Session")
		void FindSessions(bool bIsLAN);
	FString sessionName;

	UFUNCTION(BlueprintCallable, Category = "Session")
		bool JoinSession(FName SessionName);

	UFUNCTION(BlueprintCallable, Category = "Session")
		bool EndSession(FName SessionName);

	UFUNCTION(BlueprintCallable, Category = "Session")
		bool DestroySession(FName SessionName);

private:
	IOnlineSessionPtr OnlineSessionPtr;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnRegisterPlayerSessionComplete(FName SessionName, const TArray< FUniqueNetIdRef >& Players, bool bWasSuccessful);
	void OnUnregisterPlayerSessionComplete(FName SessionName, const TArray< FUniqueNetIdRef >& Players, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnEndSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

public:
	bool isHost;
	bool isClient;

	bool isQuitGame;

};
