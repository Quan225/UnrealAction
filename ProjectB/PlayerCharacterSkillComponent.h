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
	// �޺� ����
	int CurChainStep; // ���� �޺� �ܰ�
	bool bIsChainNext; // ���� �޺� �Է� ����
	bool bEnableChainPeriod; // �޺� �Է� ���� ����
	bool bIsChainOtherSkill; // �ٸ� ��Ÿ�ָ� ���� ��ų ���� ����
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
