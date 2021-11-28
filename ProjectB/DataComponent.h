#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include "SkillData.h"
#include <Engine/Classes/Animation/AnimMontage.h>
#include "SkillObject/SkillObject.h"
#include "ProjectBGameInstance.h"
#include "Components/ActorComponent.h"
#include "DataComponent.generated.h"

enum class ECharacterSoundType : uint8
{
	Pain = 0,
	Count = 1
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTB_API UDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDataComponent();

protected:
	virtual void BeginPlay() override;

public:
	bool Initialize(FString InitCharacterName);

	virtual bool LoadSoundData();
	virtual bool LoadSkillData();
    virtual bool LoadStatData();

    bool GetCharacterStat(float& DefaultHp, float& DefaultStamina);
	void SetCharacterName(FString NewName) { CharacterName = NewName; }
	FString GetCharacterName() { return CharacterName; }

	// FCharacterSkillInfo FindSkillInfo(FString SkillName, int32 ChainStep = 0);
    UAnimMontage* FindSkillMontage(FString SkillName);
	UParticleSystem* FindImpactEffect(FString SkillName);
	USoundCue* FindImpactSound(FString SkillName);
    FName FindSkillMontageSection(FString SkillName, int32 ComboStep);
	USoundCue* FindCharacterSound(ECharacterSoundType SoundType);

protected:
	FCharacterStatInfo StatData;
	FCharacterSoundInfo SoundData;
	TArray<FCharacterSkillInfo> SkillData;
	UProjectBGameInstance* GameInstance;

protected:
	FString CharacterName; // todo : GameCharacter와 중복된 값으로 존재중, 통합할지 고민해볼것
};