// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CppGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/Button.h"
#include "Engine/LevelStreaming.h"

#include "AutoHero/AutoHeroPlayerController.h"
#include "AutoHero/AutoHeroGameMode.h"
#include "AutoHero/AutoHeroCharacter.h"
#include "Systems/CppGlobalInfo.h"

#pragma region Multiplayer.
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"  
#pragma endregion

#pragma region UI manager.
// Menu.
#include "UI/CppLoginMenu.h"
#include "UI/CppRegisterMenu.h"
#include "UI/CppMainMenu.h"
#include "UI/CppPVPMenu.h"
#include "UI/CppPVEMenu.h"
#include "UI/CppBattleMenu.h"
#include "UI/CppSummaryMenu.h"
#include "UI/CppMultiplayerMenu.h"
#include "UI/CppExitGamePlayMenu.h"

// Popup.
#include "UI/CppSettingPopup.h"
#include "UI/CppRewardGiftPopup.h"
#include "UI/CppMessagePopup.h"
#include "UI/CppChatBoxPopup.h"
#include "UI/CppBlockPopup.h"
#pragma endregion

#include "PlayFab/CppPlayFabManager.h"

UCppGameInstance* UCppGameInstance::i;
UCppGameInstance* UCppGameInstance::I()
{
	return i;
}

UCppGameInstance::UCppGameInstance()
{
    i = this;
}

void UCppGameInstance::Init()
{
	Super::Init();

    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UCppGameInstance::OnMapLoaded);
}

void UCppGameInstance::OnStart()
{
    Super::OnStart();

#pragma region Multiplayer.
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        OnlineSessionPtr = OnlineSubsystem->GetSessionInterface();

        OnlineSessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UCppGameInstance::OnCreateSessionComplete);
        OnlineSessionPtr->OnRegisterPlayersCompleteDelegates.AddUObject(this, &UCppGameInstance::OnRegisterPlayerSessionComplete);
        OnlineSessionPtr->OnUnregisterPlayersCompleteDelegates.AddUObject(this, &UCppGameInstance::OnUnregisterPlayerSessionComplete);
        OnlineSessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UCppGameInstance::OnFindSessionsComplete);
        OnlineSessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UCppGameInstance::OnJoinSessionComplete);
        OnlineSessionPtr->OnEndSessionCompleteDelegates.AddUObject(this, &UCppGameInstance::OnEndSessionComplete);
        OnlineSessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UCppGameInstance::OnDestroySessionComplete);
    }
#pragma endregion

#pragma region UI manager.
    initCallback.BindUObject(this, &UCppGameInstance::OnInitCallBack);
    popCallback.BindUObject(this, &UCppGameInstance::OnPopupCallBack);

    /*UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

    SetInputUI();*/

    LoadDataUI();

    // Init push menu.
    Push(multiplayerMenu);
#pragma endregion
}

#pragma region Multiplayer.
bool UCppGameInstance::CreateSession(FName SessionName, int32 MaxNumPlayers, bool bIsLAN)
{
    if (!OnlineSessionPtr.IsValid())
    {
        return false;
    }

#pragma region Old
    /*FOnlineSessionSettings SessionSettings;
    SessionSettings.bIsLANMatch = bIsLAN;
    SessionSettings.bUsesPresence = true;
    SessionSettings.NumPublicConnections = MaxNumPlayers;
    SessionSettings.bShouldAdvertise = true;
    OnlineSessionPtr->CreateSession(0, SessionName, SessionSettings);*/
#pragma endregion

#pragma region New
    UE_LOG(LogTemp, Warning, TEXT("CreateServer"));
    FOnlineSessionSettings SessionSettings;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bIsDedicated = false;
    SessionSettings.bIsLANMatch = bIsLAN;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.NumPublicConnections = MaxNumPlayers;

    OnlineSessionPtr->CreateSession(0, SessionName, SessionSettings);
#pragma endregion

    return true;
}

void UCppGameInstance::RegisterPlayer(FName SessionName)
{
    if (!OnlineSessionPtr.IsValid())
    {
        return;
    }

    ULocalPlayer* localPlayer = AAutoHeroGameMode::I()->PlayerControllerClass.GetDefaultObject()->GetLocalPlayer();
    FUniqueNetIdRepl PlayerIdRepl = localPlayer->GetPreferredUniqueNetId();
    TSharedPtr<const FUniqueNetId> PlayerId;
    if (PlayerIdRepl.IsValid())
    {
        PlayerId = PlayerIdRepl.GetUniqueNetId();
    }

    OnlineSessionPtr->RegisterPlayer(SessionName, *PlayerId, true);
}

void UCppGameInstance::UnregisterPlayer(FName SessionName)
{
    if (!OnlineSessionPtr.IsValid())
    {
        return;
    }

    ULocalPlayer* localPlayer = AAutoHeroGameMode::I()->PlayerControllerClass.GetDefaultObject()->GetLocalPlayer();
    FUniqueNetIdRepl PlayerIdRepl = localPlayer->GetPreferredUniqueNetId();
    TSharedPtr<const FUniqueNetId> PlayerId;
    if (PlayerIdRepl.IsValid())
    {
        PlayerId = PlayerIdRepl.GetUniqueNetId();
    }
    OnlineSessionPtr->UnregisterPlayer(SessionName, *PlayerId);
}

void UCppGameInstance::FindSessions(bool bIsLAN)
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

bool UCppGameInstance::JoinSessionNew(FName SessionName)
{
    if (!OnlineSessionPtr.IsValid() || !SessionSearch.IsValid())
    {
        return false;
    }

#pragma region Old
    for (int32 j = 0; j < SessionSearch->SearchResults.Num(); j++)
    {
        FString FoundSessionName = SessionSearch->SearchResults[j].Session.GetSessionIdStr();
        sessionName = FoundSessionName;
        if (FoundSessionName.Equals(SessionName.ToString(), ESearchCase::IgnoreCase))
        {
            return OnlineSessionPtr->JoinSession(0, SessionName, SessionSearch->SearchResults[j]);
        }
    }
#pragma endregion

#pragma region New
    /*SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
    SessionSearch->MaxSearchResults = 10000;
    SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

    OnlineSessionPtr->FindSessions(0, SessionSearch.ToSharedRef());*/
#pragma endregion



    return false;
}

bool UCppGameInstance::EndSession(FName SessionName)
{
    if (!OnlineSessionPtr.IsValid())
    {
        return false;
    }

    if (OnlineSessionPtr->GetNamedSession(SessionName) != nullptr)
    {
        return OnlineSessionPtr->EndSession(SessionName);
    }
    else
    {
        return false;
    }
}

bool UCppGameInstance::DestroySession(FName SessionName)
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

void UCppGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    isHost = bWasSuccessful;
    isClient = false;

    if (isHost)
    {
        UE_LOG(LogTemp, Log, TEXT("Session created successfully: %s"), *SessionName.ToString());

        sessionName = SessionName.ToString();

        ServerTravel(UCppGlobalInfo::nameLevelMain.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to create session: %s"), *SessionName.ToString());
    }

    Pop(blockPopup);
}

void UCppGameInstance::OnRegisterPlayerSessionComplete(FName SessionName, const TArray<FUniqueNetIdRef>& Players, bool bWasSuccessful)
{
    if (bWasSuccessful && SessionSearch.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("Register player successfully: %s"), *SessionName.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to register player successfully: %s"), *SessionName.ToString());
    }
}

void UCppGameInstance::OnUnregisterPlayerSessionComplete(FName SessionName, const TArray<FUniqueNetIdRef>& Players, bool bWasSuccessful)
{
    if (bWasSuccessful && SessionSearch.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("Unregister player successfully: %s"), *SessionName.ToString());

        isHost = false;
        isClient = false;

        UCppGameInstance::I()->UnLoadLevelStreamingByName(UCppGlobalInfo::nameLevelGameplay);

        Pop(blockPopup);
        Pop(exitGamePlayMenu);
        Push(multiplayerMenu);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to unregister player successfully: %s"), *SessionName.ToString());
    }
}

void UCppGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    isHost = false;
    isClient = false;

    if (bWasSuccessful && SessionSearch.IsValid())
    {
        for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
            FString FoundSessionName = SearchResult.Session.GetSessionIdStr();
            sessionName = FoundSessionName;
            UE_LOG(LogTemp, Log, TEXT("Found session: %s"), *FoundSessionName);
        }

        if (SessionSearch->SearchResults.Num() > 0)
        {
            multiplayerMenu->GetBtnJoinSession()->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to find sessions"));
    }

    Pop(blockPopup);
}

void UCppGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    isHost = false;
    isClient = Result == EOnJoinSessionCompleteResult::Success;

    if (isClient)
    {
        UE_LOG(LogTemp, Log, TEXT("Joined session: %s"), *SessionName.ToString());

        SetInputGameplay();

        LoadLevelStreamingByName(UCppGlobalInfo::nameLevelGameplay);
        /*OnMapLoaded(GetWorld());*/
        /*AAutoHeroGameMode::I()->SpawnCharacter();*/

        FString joinAddress = "";
        OnlineSessionPtr->GetResolvedConnectString(SessionName, joinAddress);
        ClientTravel(joinAddress);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to join session: %s"), *SessionName.ToString());
    }

    Pop(blockPopup);
}

void UCppGameInstance::OnEndSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Session end successfully: %s"), *SessionName.ToString());

        isHost = false;
        isClient = false;

        if (isQuitGame)
        {
            Pop(blockPopup);
            UKismetSystemLibrary::QuitGame(i->GetWorld(), UGameplayStatics::GetPlayerController(i->GetWorld(), 0), EQuitPreference::Quit, false);
        }
        else
        {
            UCppGameInstance::I()->UnLoadLevelStreamingByName(UCppGlobalInfo::nameLevelGameplay);

            Pop(blockPopup);
            Pop(exitGamePlayMenu);
            Push(multiplayerMenu);
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to end session: %s"), *SessionName.ToString());
    }
}

void UCppGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Session destroyed successfully: %s"), *SessionName.ToString());

        isHost = false;
        isClient = false;

        if (isQuitGame)
        {
            Pop(blockPopup);
            UKismetSystemLibrary::QuitGame(i->GetWorld(), UGameplayStatics::GetPlayerController(i->GetWorld(), 0), EQuitPreference::Quit, false);
        }
        else
        {
            UCppGameInstance::I()->UnLoadLevelStreamingByName(UCppGlobalInfo::nameLevelGameplay);

            Pop(blockPopup);
            Pop(exitGamePlayMenu);
            Push(multiplayerMenu);
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to destroy session: %s"), *SessionName.ToString());
    }
}
#pragma endregion

#pragma region UI manager.
void UCppGameInstance::LoadDataUI()
{
    // Menu.
    loginMenu = dynamic_cast<UCppLoginMenu*>(SetupMenu(loginMenu, loginMenuClass));
    registerMenu = dynamic_cast<UCppRegisterMenu*>(SetupMenu(registerMenu, registerMenuClass));
    mainMenu = dynamic_cast<UCppMainMenu*>(SetupMenu(mainMenu, mainMenuClass));
    pvpMenu = dynamic_cast<UCppPVPMenu*>(SetupMenu(pvpMenu, pvpMenuClass));
    pveMenu = dynamic_cast<UCppPVEMenu*>(SetupMenu(pveMenu, pveMenuClass));
    battleMenu = dynamic_cast<UCppBattleMenu*>(SetupMenu(battleMenu, battleMenuClass));
    summaryMenu = dynamic_cast<UCppSummaryMenu*>(SetupMenu(summaryMenu, summaryMenuClass));
    multiplayerMenu = dynamic_cast<UCppMultiplayerMenu*>(SetupMenu(multiplayerMenu, multiplayerMenuClass));
    exitGamePlayMenu = dynamic_cast<UCppExitGamePlayMenu*>(SetupMenu(exitGamePlayMenu, exitGamePlayMenuClass));

    // Popup.
    settingPopup = dynamic_cast<UCppSettingPopup*>(SetupMenu(settingPopup, settingPopupClass));
    rewardGiftPopup = dynamic_cast<UCppRewardGiftPopup*>(SetupMenu(rewardGiftPopup, rewardGiftPopupClass));
    messagePopup = dynamic_cast<UCppMessagePopup*>(SetupMenu(messagePopup, messagePopupClass));
    chatBoxPopup = dynamic_cast<UCppChatBoxPopup*>(SetupMenu(chatBoxPopup, chatBoxPopupClass));
    blockPopup = dynamic_cast<UCppBlockPopup*>(SetupMenu(blockPopup, blockPopupClass));
}

UCppBaseMenu* UCppGameInstance::SetupMenu(UCppBaseMenu* menu, TSubclassOf<class UCppBaseMenu> menuClass)
{
    menu = CreateWidget<UCppBaseMenu>(GetWorld(), menuClass);
    /*check(menu);*/
    menu->Setup();
    return menu;
}

void UCppGameInstance::Push(UCppBaseMenu* menu)
{
    menu->Init();
}

void UCppGameInstance::Pop(UCppBaseMenu* menu)
{
    menu->Pop();
}

void UCppGameInstance::SetInputUI()
{
    UGameplayStatics::GetPlayerController(i->GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
}

void UCppGameInstance::SetInputGameplay()
{
    UGameplayStatics::GetPlayerController(i->GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
}

void UCppGameInstance::QuitGame()
{
    if (i->isHost)
    {
        Push(i->blockPopup);
        i->isQuitGame = true;
        i->DestroySession(FName(*i->sessionName));
    }
    else
    {
        UKismetSystemLibrary::QuitGame(i->GetWorld(), UGameplayStatics::GetPlayerController(i->GetWorld(), 0), EQuitPreference::Quit, false);
    }
}

void UCppGameInstance::OnInitCallBack()
{
}

void UCppGameInstance::OnPopupCallBack()
{
}
#pragma endregion

void UCppGameInstance::ServerTravel(const FString& levelName)
{
    GetWorld()->ServerTravel("/Game/Level/" + levelName + "?listen");
}
void UCppGameInstance::ClientTravel(const FString& joinAddress)
{
    if (joinAddress == "") return;

    if (APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        playerController->ClientTravel(joinAddress, TRAVEL_Absolute);
    }
}

void UCppGameInstance::OnMapLoaded(UWorld* LoadedWorld)
{
    if (isHost || isClient)
    {
        LoadDataUI();
        LoadLevelStreamingByName(UCppGlobalInfo::nameLevelGameplay);
        Pop(multiplayerMenu);
        Push(exitGamePlayMenu);
        SetInputGameplay();

#pragma region Old
        //if (isClient)
        //{
        //    TArray<AActor*> FoundActors;
        //    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAutoHeroCharacter::StaticClass(), FoundActors);
        //    for (AActor* Actor : FoundActors)
        //    {
        //        AAutoHeroCharacter* Character = dynamic_cast<AAutoHeroCharacter*>(Actor);
        //        if (Character)
        //        {
        //            // Sử dụng nhân vật ở đây.
        //        }
        //    }
        //}
#pragma endregion

#pragma region New
        if (isClient)
        {
            FTimerDelegate TimerDel;
            FTimerHandle fTimerHandle;
            TimerDel.BindLambda([this]()
            {
                UWorld* world = GetWorld(); // get the current UWorld instance.

                TArray<AActor*> FoundActors;
                UGameplayStatics::GetAllActorsOfClass(world, AAutoHeroCharacter::StaticClass(), FoundActors);

                // Lấy hệ thống trực tuyến
                IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
                if (OnlineSubsystem)
                {
                    // Lấy giao diện phiên trực tuyến
                    IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
                    if (SessionInterface.IsValid())
                    {
                        // Lấy thông tin phiên
                        FOnlineSessionSettings* SessionSettings = SessionInterface->GetSessionSettings(FName(*sessionName));
                        if (SessionSettings)
                        {
                            // Sử dụng thông tin phiên ở đây
                        }
                    }
                }

                if (FoundActors.Num() > 1)
                {
                    AAutoHeroCharacter* Character = dynamic_cast<AAutoHeroCharacter*>(FoundActors[0]);
                    AAutoHeroGameMode::I()->myCharacter = Character;

                    APlayerController* playerController = world->GetFirstPlayerController();
                    if (playerController)
                    {
                        playerController->SetViewTargetWithBlend(Character);
                        playerController->Possess(Character);
                        playerController->SetInputMode(FInputModeGameAndUI());

                        AAutoHeroGameMode::I()->PlayerControllerClass = playerController->GetClass(); 
                    }
                }
            });

            // Thiết lập một hẹn giờ để gọi hàm trên sau 2 giây.
            GetWorld()->GetTimerManager().SetTimer(fTimerHandle, TimerDel, 3.0f, false);
        }
#pragma endregion

    }
}

void UCppGameInstance::LoadLevelStreamingByName(const FName& levelName)
{
    UWorld* world = GetWorld(); // Get a pointer to the world.
    if (world != nullptr) // Ensure the world exists.
    {
        ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(world, levelName);
        if (Level != nullptr) // Ensure the level exists.
        {
            Level->bShouldBlockOnLoad = false; // The game will not freeze while the level is loading.
            // Set the level to be loaded and visible initially.
            Level->SetShouldBeLoaded(true);
            Level->SetShouldBeVisible(true);

            // Flush level streaming to ensure the level is loaded.
            world->FlushLevelStreaming(EFlushLevelStreamingType::Full);
        }
    }
}

void UCppGameInstance::UnLoadLevelStreamingByName(const FName& levelName)
{
    UWorld* world = GetWorld(); // Get a pointer to the world.
    if (world != nullptr) // Ensure the world exists.
    {
        ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(world, levelName);
        if (Level != nullptr) // Ensure the level exists.
        {
            // Set the level to not be loaded and not be visible.
            Level->SetShouldBeLoaded(false);
            Level->SetShouldBeVisible(false);

            // Flush level streaming to ensure the level is unloaded.
            world->FlushLevelStreaming(EFlushLevelStreamingType::Full);
        }
    }
}
