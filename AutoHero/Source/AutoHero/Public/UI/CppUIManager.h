// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CppBaseMenu.h"
#include "GameFramework/Pawn.h"
#include "CppUIManager.generated.h"

class UCppMainMenu;
class UCppSettingPopup;

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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere) TSubclassOf<class UCppMainMenu> mainMenuClass;
	UCppMainMenu* mainMenu;

	UPROPERTY(EditAnywhere) TSubclassOf<class UCppSettingPopup> settingPopupClass;
	UCppSettingPopup* settingPopup;

public:
	static void Push(UCppBaseMenu* menu);
	static void Pop(UCppBaseMenu* menu);

};
