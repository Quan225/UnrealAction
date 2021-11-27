#pragma once

#include "CoreMinimal.h"
#include "CharacterSkillComponent.h"
#include "PlayerCharacterSkillComponent.generated.h"


UCLASS()
class PROJECTB_API UPlayerCharacterSkillComponent : public UCharacterSkillComponent
{
	GENERATED_BODY()
public:
		UPlayerCharacterSkillComponent();

protected:
	virtual void BeginPlay() override;

protected:
	class APlayerCharacter* OwnerPlayerCharacter;

protected:
	// 콤보 관련
	int CurChainStep; // 현재 콤보 단계
	bool bIsChainNext; // 다음 콤보 입력 여부
	bool bEnableChainPeriod; // 콤보 입력 가능 여부
	bool bIsChainOtherSkill; // 다른 몽타주를 가진 스킬 연계 여부
	EChainType OtherChainType;

	bool ChainOtherSkill();

public:
	virtual void ResetChainFlags() override;
	void JumpMontageSection();

	void SetEnableChainPeriod(bool EnablePeriod) { bEnableChainPeriod = EnablePeriod; }
	bool GetEnableChainPeriod() { return bEnableChainPeriod; }


public:
	virtual bool UseSkill(EAxisInputType AxisInput, EButtonInputType ButtonInput) override;

protected:
	FSkillSequence* FindChainSkillSequence(EAxisInputType AxisInput, EButtonInputType ButtonInput);
};
