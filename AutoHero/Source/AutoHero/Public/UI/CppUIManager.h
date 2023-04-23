// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "GameFramework/Pawn.h"
#include "CppUIManager.generated.h"

#pragma region Menu.
class UCppLoginMenu;
class UCppRegisterMenu;
class UCppMainMenu;
class UCppPVPMenu;
class UCppPVEMenu;
class UCppBattleMenu;
class UCppSummaryMenu;
#pragma endregion

#pragma region Popup.
class UCppSettingPopup;
class UCppRewardGiftPopup;
class UCppMessagePopup;
class UCppChatBoxPopup;
#pragma endregion

DECLARE_DELEGATE(InitCallback);
DECLARE_DELEGATE(PopCallback);

UCLASS()
class AUTOHERO_API ACppUIManager : public APawn
{
	GENERATED_BODY()

private:
	static ACppUIManager* i;
public:
	static ACppUIManager* I();

private:
	void SetNullAllVariable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Sets default values for this pawn's properties
	ACppUIManager();

private:
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppLoginMenu> loginMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppRegisterMenu> registerMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppMainMenu> mainMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppPVPMenu> pvpMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppPVEMenu> pveMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppBattleMenu> battleMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppSummaryMenu> summaryMenuClass;

private:
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppSettingPopup> settingPopupClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppRewardGiftPopup> rewardGiftPopupClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppMessagePopup> messagePopupClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppChatBoxPopup> chatBoxPopupClass;

#pragma region Menu.
public:
	TArray<UCppBaseMenu*> arrayMenu;

	UCppLoginMenu* loginMenu;
	UCppRegisterMenu* registerMenu;
	UCppMainMenu* mainMenu;
	UCppPVPMenu* pvpMenu;
	UCppPVEMenu* pveMenu;
	UCppBattleMenu* battleMenu;
	UCppSummaryMenu* summaryMenu;
#pragma endregion

#pragma region Popup.
public:
	UCppSettingPopup* settingPopup;
	UCppRewardGiftPopup* rewardGiftPopup;
	UCppMessagePopup* messagePopup;
	UCppChatBoxPopup* chatBoxPopup;
	
#pragma endregion

public:
	UCppBaseMenu* SetupMenu(UCppBaseMenu* menu, TSubclassOf<class UCppBaseMenu> menuClass);

public:
	static void Push(UCppBaseMenu* menu);
	static void Pop(UCppBaseMenu* menu);
	static void PopAll();

	static void SetInputUI();
	static void SetInputGameplay();

	static void QuitGame();

public:
	InitCallback initCallback;
	PopCallback popCallback;

public:
	UFUNCTION() void OnInitCallBack();
	UFUNCTION() void OnPopupCallBack();

};
