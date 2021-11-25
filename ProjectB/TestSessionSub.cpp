#include "TestSessionSub.h"
#include "OnlineSubsystemUtils.h"
#include <Kismet/GameplayStatics.h>

UTestSessionSub::UTestSessionSub() 
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionCompleted)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsCompleted)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionCompleted))
{ // Complete �ݹ鿡 ���� Subsystem �Լ��� Delegate�� ���ε�
	UE_LOG(LogTemp, Log, TEXT("Session Subsystem is init"));
}

void UTestSessionSub::CreateSession(int32 PublicConnectNum, bool IsLanMatch)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}
	
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = 0;
	LastSessionSettings->NumPublicConnections = PublicConnectNum;
	LastSessionSettings->bAllowInvites = true; // �ʴ븦 ���� ����
	LastSessionSettings->bAllowJoinInProgress = true; // ���� �� �ߵ�����
	LastSessionSettings->bAllowJoinViaPresence = true; // �����𽺸� ���� ����
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true; // ģ���� �����𽺸� ���� ������ ��������
	LastSessionSettings->bIsDedicated = false; // ������ �������� ������ ȣ��Ʈ���� ����
	LastSessionSettings->bUsesPresence = true; // ������ ǥ������ ���� 
	LastSessionSettings->bIsLANMatch = IsLanMatch; // ����ġ����
	LastSessionSettings->bShouldAdvertise = true; // �� ��ġ�� �¶��ο� '����'�Ǵ��� ����

	LastSessionSettings->Set(SETTING_MAPNAME, FString("NewMap_2"), EOnlineDataAdvertisementType::ViaOnlineService);

	CreateSessionCompleteDelegateHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController(); // ��Ʈ�ѷ����� ù��° ���� �÷��̾ �޾ƿ´�
	if (localPlayer == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("localPlayer is nullptr"));
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Try Session Create"));

	// ���� ���� �õ�
	if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		// ���� ���� ����
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void UTestSessionSub::StartSession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnStartSessionCompleteEvent.Broadcast(false);
		return;
	}

	StartSessionCompleteDelegateHandle = sessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

	if (!sessionInterface->StartSession(NAME_GameSession))
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
		OnStartSessionCompleteEvent.Broadcast(false);
	}
}

void UTestSessionSub::FindSessions(int32 MaxSearchResults, bool IsLanQuery)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	FindSessionsCompleteDelegateHandle = sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IsLanQuery;

	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (localPlayer == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("localPlayer is nullptr"));
		return;
	}

	if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}


}

void UTestSessionSub::JoinGameSession(const FOnlineSessionSearchResult& SessionResult)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	JoinSessionCompleteDelegateHandle = sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (localPlayer == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("localPlayer is nullptr"));
		return;
	}

	if (!sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UTestSessionSub::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	OnCreateSessionCompleteEvent.Broadcast(Successful);
}

void UTestSessionSub::OnStartSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

	UE_LOG(LogTemp, Log, TEXT("Session Started"));
	TryTravelToCurrentSession();

	OnStartSessionCompleteEvent.Broadcast(Successful);
}

void UTestSessionSub::OnFindSessionsCompleted(bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	UE_LOG(LogTemp, Log, TEXT("Session Find End"));

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), Successful);
		return;
	}

	for (int i = 0; i < LastSessionSearch->SearchResults.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("Session : %s"), &LastSessionSearch->SearchResults[i].Session.OwningUserName);
	}

	UE_LOG(LogTemp, Log, TEXT("Move To New Session"));
	JoinGameSession(LastSessionSearch->SearchResults[0]);

	OnFindSessionsCompleteEvent.Broadcast(LastSessionSearch->SearchResults, Successful);
}

void UTestSessionSub::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	//TryTravelToCurrentSession();

	OnJoinGameSessionCompleteEvent.Broadcast(Result);
}

bool UTestSessionSub::TryTravelToCurrentSession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		return false;
	}

	// �÷��� �� ���� ������ �޾ƿ´�
	FString connectString;
	if (!sessionInterface->GetResolvedConnectString(NAME_GameSession, connectString))
	{
		return false;
	}

	// ���� ����
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->ClientTravel(connectString, TRAVEL_Absolute);
	return true;
}
