// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplayer/CppMultiplayerManager.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "AutoHero/AutoHeroGameMode.h"
#include "Kismet/GameplayStatics.h"

#include "UI/CppMultiplayerMenu.h"
#include "UI/CppUIManager.h"

ACppMultiplayerManager* ACppMultiplayerManager::i;
ACppMultiplayerManager* ACppMultiplayerManager::I()
{
    return i;
}

// Sets default values
ACppMultiplayerManager::ACppMultiplayerManager()
{
    i = this;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACppMultiplayerManager::BeginPlay()
{
	Super::BeginPlay();
	
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		OnlineSessionPtr = OnlineSubsystem->GetSessionInterface();
	}
}

// Called every frame
void ACppMultiplayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region Sessions
bool ACppMultiplayerManager::CreateSession(FName SessionName, int32 MaxNumPlayers, bool bIsLAN)
{
    if (!OnlineSessionPtr.IsValid())
    {
        return false;
    }

    FOnlineSessionSettings SessionSettings;
    SessionSettings.bIsLANMatch = bIsLAN;
    SessionSettings.bUsesPresence = true;
    SessionSettings.NumPublicConnections = MaxNumPlayers;
    SessionSettings.bShouldAdvertise = true;

    OnlineSessionPtr->CreateSession(0, SessionName, SessionSettings);
    OnCreateSessionCompleteDelegateHandle = OnlineSessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &ACppMultiplayerManager::OnCreateSessionComplete);

    return true;
}

void ACppMultiplayerManager::FindSessions(bool bIsLAN)
{
    if (!OnlineSessionPtr.IsValid())
    {
        return;
    }

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->bIsLanQuery = bIsLAN;
    SessionSearch->MaxSearchResults = 100;
    SessionSearch->PingBucketSize = 50;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    OnlineSessionPtr->FindSessions(0, SessionSearch.ToSharedRef());
    OnFindSessionsCompleteDelegateHandle = OnlineSessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &ACppMultiplayerManager::OnFindSessionsComplete);
}

bool ACppMultiplayerManager::JoinSession(FName SessionName)
{
    if (!OnlineSessionPtr.IsValid() || !SessionSearch.IsValid())
    {
        return false;
    }

    OnJoinSessionCompleteDelegateHandle = OnlineSessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &ACppMultiplayerManager::OnJoinSessionComplete);

    for (int32 j = 0; j < SessionSearch->SearchResults.Num(); j++)
    {
        FString FoundSessionName = SessionSearch->SearchResults[j].Session.GetSessionIdStr();
        if (FoundSessionName.Equals(SessionName.ToString(), ESearchCase::IgnoreCase))
        {
            return OnlineSessionPtr->JoinSession(0, SessionName, SessionSearch->SearchResults[j]);
        }
    }

    return false;
}

bool ACppMultiplayerManager::DestroySession(FName SessionName)
{
    if (!OnlineSessionPtr.IsValid())
    {
        return false;
    }

    OnDestroySessionCompleteDelegateHandle = OnlineSessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &ACppMultiplayerManager::OnDestroySessionComplete);

    return OnlineSessionPtr->DestroySession(SessionName);
}
#pragma endregion

#pragma region Listener.
void ACppMultiplayerManager::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    isHost = bWasSuccessful;
    isClent = false;

    if (isHost)
    {
        UE_LOG(LogTemp, Log, TEXT("Session created successfully: %s"), *SessionName.ToString());

        UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
        if (World)
        {
            AAutoHeroGameMode* autoHeroGameMode = dynamic_cast<AAutoHeroGameMode*>(UGameplayStatics::GetGameMode(World));
            if (autoHeroGameMode)
            {
                autoHeroGameMode->CreatePlayer();
            }
        }

        ACppUIManager::I()->Pop(ACppUIManager::I()->multiplayerMenu);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to create session: %s"), *SessionName.ToString());
    }

    OnlineSessionPtr->OnCreateSessionCompleteDelegates.Remove(OnCreateSessionCompleteDelegateHandle);
}

void ACppMultiplayerManager::OnFindSessionsComplete(bool bWasSuccessful)
{
    isHost = false;
    isClent = false;

    if (bWasSuccessful && SessionSearch.IsValid())
    {
        for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
            FString FoundSessionName = SearchResult.Session.GetSessionIdStr();
            UE_LOG(LogTemp, Log, TEXT("Found session: %s"), *FoundSessionName);
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to find sessions"));
    }

    OnlineSessionPtr->OnFindSessionsCompleteDelegates.Remove(OnFindSessionsCompleteDelegateHandle);
}

void ACppMultiplayerManager::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    isHost = false;
    isClent = Result == EOnJoinSessionCompleteResult::Success;

    if (isClent)
    {
        UE_LOG(LogTemp, Log, TEXT("Joined session: %s"), *SessionName.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to join session: %s"), *SessionName.ToString());
    }

    OnlineSessionPtr->OnJoinSessionCompleteDelegates.Remove(OnJoinSessionCompleteDelegateHandle);
}

void ACppMultiplayerManager::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    isHost = false;
    isClent = false;

    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Session destroyed successfully: %s"), *SessionName.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to destroy session: %s"), *SessionName.ToString());
    }

    OnlineSessionPtr->OnDestroySessionCompleteDelegates.Remove(OnDestroySessionCompleteDelegateHandle);
}
#pragma endregion
