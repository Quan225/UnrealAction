#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TestSessionSub.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnStartSessionComplete, bool, Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FCSOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FCSOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);


UCLASS()
class PROJECTB_API UTestSessionSub : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UTestSessionSub();

	void CreateSession(int32 PublicConnectNum, bool IsLanMatch);
	void StartSession();
	void FindSessions(int32 MaxSearchResults, bool IsLanQuery);
	void JoinGameSession(const FOnlineSessionSearchResult& SessionResult);

	// Delegate BroadCast�� �����Ű�� ���� ���
	FCSOnCreateSessionComplete OnCreateSessionCompleteEvent;
	FCSOnStartSessionComplete OnStartSessionCompleteEvent;
	FCSOnFindSessionsComplete OnFindSessionsCompleteEvent;
	FCSOnJoinSessionComplete OnJoinGameSessionCompleteEvent;

protected:
	// Delegate�� bind�� �Լ�
	void OnCreateSessionCompleted(FName SessionName, bool Successful);
	void OnStartSessionCompleted(FName SessionName, bool Successful);
	void OnFindSessionsCompleted(bool Successful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	bool TryTravelToCurrentSession();

private:
	// Delegate�� Delegate Handle
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
};
