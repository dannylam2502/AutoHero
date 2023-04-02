// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayFab/CppPlayFabManager.h"
#include "Core/PlayFabClientAPI.h"
#include "SaveGame/CppGameData.h"
#include "SaveGame/CppUserData.h"

#include "UI/CppUIManager.h"
#include "UI/CppMainMenu.h"
#include "UI/CppLoginMenu.h"
#include "UI/CppRegisterMenu.h"

ACppPlayFabManager* ACppPlayFabManager::instance;

ACppPlayFabManager* ACppPlayFabManager::Instance()
{
	return instance;
}

// Sets default values
ACppPlayFabManager::ACppPlayFabManager()
{
    instance = this;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACppPlayFabManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACppPlayFabManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region Login.
void ACppPlayFabManager::GetLogin()
{
    clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

    FString userName = ACppPlayFabManager::Instance()->loginUserName;
    FString userPassword = ACppPlayFabManager::Instance()->loginUserPassword;

    PlayFab::ClientModels::FLoginWithPlayFabRequest request;
    request.Username = userName;
    request.Password = userPassword;
    request.TitleId = titleID;

    clientAPI->LoginWithPlayFab(request,
        PlayFab::UPlayFabClientAPI::FLoginWithPlayFabDelegate::CreateUObject(this, &ACppPlayFabManager::OnLoginSuccess),
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &ACppPlayFabManager::OnLoginError)
    );
}

void ACppPlayFabManager::OnLoginSuccess(const PlayFab::ClientModels::FLoginResult& result) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("On Login Success!"));

    ACppUIManager::Instance()->loginMenu->OnLoginSuccess();
}

void ACppPlayFabManager::OnLoginError(const PlayFab::FPlayFabCppError& errorResult) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("On Login Error!"));
    GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Blue, errorResult.ErrorName);
    GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Blue, errorResult.ErrorMessage);
    //GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Blue, errorResult.ErrorCode.ToString());
}

#pragma endregion

#pragma region Register.
void ACppPlayFabManager::GetRegister()
{
    if (clientAPI == NULL)
    {
        clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
    }

    PlayFab::ClientModels::FRegisterPlayFabUserRequest request;
    request.Email = userEmail;
    request.Password = registerUserPassword;
    request.Username = registerUserName;
    request.DisplayName = registerUserName;

    clientAPI->RegisterPlayFabUser(request,
        PlayFab::UPlayFabClientAPI::FRegisterPlayFabUserDelegate::CreateUObject(this, &ACppPlayFabManager::OnRegisterSuccess),
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &ACppPlayFabManager::OnRegisterError)
    );
}

void ACppPlayFabManager::OnRegisterSuccess(const PlayFab::ClientModels::FRegisterPlayFabUserResult& result) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("On Register Success!"));

    ACppUIManager::Instance()->registerMenu->OnRegisterSuccess();
}

void ACppPlayFabManager::OnRegisterError(const PlayFab::FPlayFabCppError& errorResult) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("On Register Error!"));
}

#pragma endregion

