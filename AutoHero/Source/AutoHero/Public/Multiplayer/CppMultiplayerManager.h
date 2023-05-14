// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "CppMultiplayerManager.generated.h"

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
		void FindSessions(bool bIsLAN);

	UFUNCTION(BlueprintCallable, Category = "Session")
		bool JoinSession(FName SessionName);

	UFUNCTION(BlueprintCallable, Category = "Session")
		bool DestroySession(FName SessionName);

private:
	IOnlineSessionPtr OnlineSessionPtr;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;


	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

private:
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

public:
	bool isHost;
	bool isClent;

};
