// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#pragma region Multiplayer.
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"  
#pragma endregion

#pragma region UI manager.
#include "UI/CppBaseMenu.h"  
#pragma endregion

#include "CppGameInstance.generated.h"

#pragma region Multiplayer.
class ACppSpawnCharacterManager;
#pragma endregion

#pragma region UI manager.
class UCppLoginMenu;
class UCppRegisterMenu;
class UCppMainMenu;
class UCppPVPMenu;
class UCppPVEMenu;
class UCppBattleMenu;
class UCppSummaryMenu;
class UCppMultiplayerMenu;
class UCppExitGamePlayMenu;

class UCppSettingPopup;
class UCppRewardGiftPopup;
class UCppMessagePopup;
class UCppChatBoxPopup;
class UCppBlockPopup;

DECLARE_DELEGATE(InitCallback);
DECLARE_DELEGATE(PopCallback);
#pragma endregion

class AAutoHeroGameMode;

/**
 * 
 */
UCLASS()
class AUTOHERO_API UCppGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	static UCppGameInstance* i;
public:
	static UCppGameInstance* I();

public:

	UCppGameInstance();

protected:
	virtual void Init() override;
	virtual void OnStart() override;

public:
	AAutoHeroGameMode* gameMode;

#pragma region Multiplayer.
public:
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
		bool JoinSessionNew(FName SessionName);

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
#pragma endregion

#pragma region UI manager.
	// Menu.
private:
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppLoginMenu> loginMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppRegisterMenu> registerMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppMainMenu> mainMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppPVPMenu> pvpMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppPVEMenu> pveMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppBattleMenu> battleMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppSummaryMenu> summaryMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppMultiplayerMenu> multiplayerMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppExitGamePlayMenu> exitGamePlayMenuClass;

public:
	UCppLoginMenu* loginMenu;
	UCppRegisterMenu* registerMenu;
	UCppMainMenu* mainMenu;
	UCppPVPMenu* pvpMenu;
	UCppPVEMenu* pveMenu;
	UCppBattleMenu* battleMenu;
	UCppSummaryMenu* summaryMenu;
	UCppMultiplayerMenu* multiplayerMenu;
	UCppExitGamePlayMenu* exitGamePlayMenu;

	// Popup.
private:
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppSettingPopup> settingPopupClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppRewardGiftPopup> rewardGiftPopupClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppMessagePopup> messagePopupClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppChatBoxPopup> chatBoxPopupClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppBlockPopup> blockPopupClass;

public:
	UCppSettingPopup* settingPopup;
	UCppRewardGiftPopup* rewardGiftPopup;
	UCppMessagePopup* messagePopup;
	UCppChatBoxPopup* chatBoxPopup;
	UCppBlockPopup* blockPopup;

public:
	void LoadDataUI();
	UCppBaseMenu* SetupMenu(UCppBaseMenu* menu, TSubclassOf<class UCppBaseMenu> menuClass);

public:
	static void Push(UCppBaseMenu* menu);
	static void Pop(UCppBaseMenu* menu);

	static void SetInputUI();
	static void SetInputGameplay();

	static void QuitGame();

public:
	InitCallback initCallback;
	PopCallback popCallback;

public:
	UFUNCTION() void OnInitCallBack();
	UFUNCTION() void OnPopupCallBack();
#pragma endregion

public:
	void ServerTravel(const FString& levelName);
	void ClientTravel(const FString& joinAddress);
	UFUNCTION() void OnMapLoaded(UWorld* LoadedWorld);

public:
	void LoadLevelStreamingByName(const FName& levelName);
	void UnLoadLevelStreamingByName(const FName& levelName);

};
