// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplayer/CppMultiplayerManager.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "AutoHero/AutoHeroGameMode.h"
#include "System/CppSpawnCharacterManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "UI/CppMultiplayerMenu.h"
#include "UI/CppExitGamePlayMenu.h"
#include "UI/CppUIManager.h"
#include "UI/CppBlockPopup.h"

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

        OnlineSessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &ACppMultiplayerManager::OnCreateSessionComplete);
        OnlineSessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &ACppMultiplayerManager::OnFindSessionsComplete);
        OnlineSessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &ACppMultiplayerManager::OnJoinSessionComplete);
        OnlineSessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &ACppMultiplayerManager::OnDestroySessionComplete);
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
}

bool ACppMultiplayerManager::JoinSession(FName SessionName)
{
    if (!OnlineSessionPtr.IsValid() || !SessionSearch.IsValid())
    {
        return false;
    }

    for (int32 j = 0; j < SessionSearch->SearchResults.Num(); j++)
    {
        FString FoundSessionName = SessionSearch->SearchResults[j].Session.GetSessionIdStr();
        sessionName = FoundSessionName;
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

    if (OnlineSessionPtr->GetNamedSession(SessionName) != nullptr)
    {
        return OnlineSessionPtr->DestroySession(SessionName);
    }
    else
    {
        return false;
    }
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

        sessionName = SessionName.ToString();

        /*for (int32 j = 0; j < SessionSearch->SearchResults.Num(); j++)
        {
            FString FoundSessionName = SessionSearch->SearchResults[j].Session.GetSessionIdStr();
            sessionName = FoundSessionName;
        }*/

        ACppSpawnCharacterManager::I()->LoadCharacter();

        ACppUIManager::I()->Pop(ACppUIManager::I()->multiplayerMenu);
        ACppUIManager::I()->Push(ACppUIManager::I()->exitGamePlayMenu);
        ACppUIManager::I()->SetInputGameplay();
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to create session: %s"), *SessionName.ToString());
    }

    ACppUIManager::I()->Pop(ACppUIManager::I()->blockPopup);
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
            sessionName = FoundSessionName;
            UE_LOG(LogTemp, Log, TEXT("Found session: %s"), *FoundSessionName);
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to find sessions"));
    }

    ACppUIManager::I()->Pop(ACppUIManager::I()->blockPopup);
}

void ACppMultiplayerManager::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    isHost = false;
    isClent = Result == EOnJoinSessionCompleteResult::Success;

    if (isClent)
    {
        UE_LOG(LogTemp, Log, TEXT("Joined session: %s"), *SessionName.ToString());

        ACppSpawnCharacterManager::I()->LoadCharacter();

        ACppUIManager::I()->Pop(ACppUIManager::I()->multiplayerMenu);
        ACppUIManager::I()->Push(ACppUIManager::I()->exitGamePlayMenu);
        ACppUIManager::I()->SetInputGameplay();
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to join session: %s"), *SessionName.ToString());
    }

    ACppUIManager::I()->Pop(ACppUIManager::I()->blockPopup);
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

    if (isQuitGame)
    {
        ACppUIManager::I()->Pop(ACppUIManager::I()->blockPopup);
        UKismetSystemLibrary::QuitGame(i->GetWorld(), UGameplayStatics::GetPlayerController(i->GetWorld(), 0), EQuitPreference::Quit, false);
    }
    else
    {
        ACppUIManager::I()->Pop(ACppUIManager::I()->blockPopup);
        ACppUIManager::I()->Pop(ACppUIManager::I()->exitGamePlayMenu);
        ACppUIManager::I()->Push(ACppUIManager::I()->multiplayerMenu);
    }
}
#pragma endregion
