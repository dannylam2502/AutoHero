// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "GameFramework/Pawn.h"
#include "CppUIManager.generated.h"

class UCppMainMenu;
class UCppPVPMenu;
class UCppPVEMenu;
class UCppBattleMenu;
class UCppSummaryMenu;

class UCppSettingPopup;
class UCppRewardGiftPopup;

UCLASS()
class AUTOHERO_API ACppUIManager : public APawn
{
	GENERATED_BODY()

private:
	static ACppUIManager* instance;
public:
	static ACppUIManager* Instance();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Sets default values for this pawn's properties
	ACppUIManager();

private:
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppMainMenu> mainMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppPVPMenu> pvpMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppPVEMenu> pveMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppBattleMenu> battleMenuClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppSummaryMenu> summaryMenuClass;

private:
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppSettingPopup> settingPopupClass;
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppRewardGiftPopup> rewardGiftPopupClass;

#pragma region Menu.
public:
	TArray<UCppBaseMenu*> arrayMenu;

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
};
