// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayFab/CppPlayFabManager.h"

#include "Core/PlayFabClientAPI.h"
#include "Core/PlayFabMultiplayerAPI.h"

#include "SaveGame/CppGameData.h"
#include "SaveGame/CppUserData.h"

#include "UI/CppUIManager.h"
#include "UI/CppMainMenu.h"
#include "UI/CppLoginMenu.h"
#include "UI/CppRegisterMenu.h"

#include "UI/CppMessagePopup.h"

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
	
    clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
    multiplayerAPI = IPlayFabModuleInterface::Get().GetMultiplayerAPI();
}

// Called every frame
void ACppPlayFabManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region Login.
void ACppPlayFabManager::GetLogin()
{
    FString userName = ACppPlayFabManager::Instance()->loginUserName;
    FString userPassword = ACppPlayFabManager::Instance()->loginUserPassword;

    FLoginWithPlayFabRequest request;
    request.Username = userName;
    request.Password = userPassword;
    request.TitleId = titleID;

    clientAPI->LoginWithPlayFab(request,
        UPlayFabClientAPI::FLoginWithPlayFabDelegate::CreateUObject(this, &ACppPlayFabManager::OnLoginSuccess),
        FPlayFabErrorDelegate::CreateUObject(this, &ACppPlayFabManager::OnLoginError)
    );
}

void ACppPlayFabManager::OnLoginSuccess(const FLoginResult& result) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("On Login Success!"));

    ACppUIManager::Instance()->loginMenu->OnLoginSuccess();
}

void ACppPlayFabManager::OnLoginError(const FPlayFabCppError& errorResult) const
{
    GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("On Login Error!"));
    GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, errorResult.ErrorName);
    GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, errorResult.ErrorMessage);
    
    TArray<UObject*> initParams;
    ACppUIManager::Instance()->Push(ACppUIManager::Instance()->messagePopup, initParams);
    FString strContent = "Your user name or password is wrong!!!\nPlease check again.";
    ACppUIManager::Instance()->messagePopup->SetTextMessage(strContent);
}

#pragma endregion

#pragma region Register.
void ACppPlayFabManager::GetRegister()
{
    if (clientAPI == NULL)
    {
        clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
    }

    FRegisterPlayFabUserRequest request;
    request.Email = userEmail;
    request.Password = registerUserPassword;
    request.Username = registerUserName;
    request.DisplayName = registerUserName;

    bool bRegisterPlayFabUser = clientAPI->RegisterPlayFabUser(request,
        UPlayFabClientAPI::FRegisterPlayFabUserDelegate::CreateUObject(this, &ACppPlayFabManager::OnRegisterSuccess),
        FPlayFabErrorDelegate::CreateUObject(this, &ACppPlayFabManager::OnRegisterError));

    if (bRegisterPlayFabUser)
    {

    }

}

void ACppPlayFabManager::OnRegisterSuccess(const FRegisterPlayFabUserResult& result) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("On Register Success!"));

    ACppUIManager::Instance()->registerMenu->OnRegisterSuccess();
}

void ACppPlayFabManager::OnRegisterError(const FPlayFabCppError& errorResult) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("On Register Error!"));
}

#pragma endregion

#pragma region Matchmaking.
// Create matchmaking ticket.
void ACppPlayFabManager::CreateMatchmakingTicket()
{
    FCreateMatchmakingTicketRequest request;
    request.GiveUpAfterSeconds = 120;
    request.QueueName = TEXT("QuickMatch");
    /*request.Creator = PlayFab::MultiplayerModels::FMatchmakingPlayer();
    request.Creator.Entity.Id = Result.EntityToken.Get()->Entity.Get()->Id;
    request.Creator.Entity.Type = Result.EntityToken.Get()->Entity.Get()->Type;*/

    bool bCreateMatchmakingTicket = multiplayerAPI-> CreateMatchmakingTicket(request,
        UPlayFabMultiplayerAPI::FCreateMatchmakingTicketDelegate::CreateUObject(this, &ACppPlayFabManager::SuccessCreateMatchmakingTicket),
        FPlayFabErrorDelegate::CreateUObject(this, &ACppPlayFabManager::ErrorCreateMatchmakingTicket));

    if (bCreateMatchmakingTicket)
    {
    }
}

void ACppPlayFabManager::SuccessCreateMatchmakingTicket(const FCreateMatchmakingTicketResult& result) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Create Matchmaking Ticket Success!"));
}

void ACppPlayFabManager::ErrorCreateMatchmakingTicket(const FPlayFabCppError& errorResult) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Create Matchmaking Ticket Error!"));
}
//.

// Get matchmaking ticket.
void ACppPlayFabManager::GetMatchmakingTicket()
{
    FGetMatchmakingTicketRequest request;

    bool bGetMatchmakingTicket = multiplayerAPI->GetMatchmakingTicket(request,
        UPlayFabMultiplayerAPI::FGetMatchmakingTicketDelegate::CreateUObject(this, &ACppPlayFabManager::SuccessGetMatchmakingTicket),
        FPlayFabErrorDelegate::CreateUObject(this, &ACppPlayFabManager::ErrorGetMatchmakingTicket));

    if (bGetMatchmakingTicket)
    {
    }
}

void ACppPlayFabManager::SuccessGetMatchmakingTicket(const FGetMatchmakingTicketResult& result) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Get Matchmaking Ticket Success!"));
}

void ACppPlayFabManager::ErrorGetMatchmakingTicket(const FPlayFabCppError& errorResult) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Get Matchmaking Ticket Error!"));
}
//.

// Get match.
void ACppPlayFabManager::GetMatch()
{
    FGetMatchRequest request;

    bool bGetMatch = multiplayerAPI->GetMatch(request,
        UPlayFabMultiplayerAPI::FGetMatchDelegate::CreateUObject(this, &ACppPlayFabManager::SuccessGetMatch),
        FPlayFabErrorDelegate::CreateUObject(this, &ACppPlayFabManager::ErrorGetMatch));

    if (bGetMatch)
    {
    }
}

void ACppPlayFabManager::SuccessGetMatch(const FGetMatchResult& result) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Get Match Success!"));
}

void ACppPlayFabManager::ErrorGetMatch(const FPlayFabCppError& errorResult) const
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Get Match Error!"));
}
//.

#pragma endregion
