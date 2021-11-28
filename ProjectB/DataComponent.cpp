#include "DataComponent.h"
#include <Engine/DataTable.h>
#include "SkillData.h"
#include "GameCharacter.h"
#include "Enemy.h"
#include "PlayerCharacter.h"

UDataComponent::UDataComponent()
{
}

void UDataComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UDataComponent::Initialize(FString InitCharacterName)
{
	GameInstance = Cast<UProjectBGameInstance>(GetWorld()->GetGameInstance());
	CharacterName = InitCharacterName;

	LoadStatData();
	LoadSkillData();
	LoadSoundData();

	return true;
}

bool UDataComponent::LoadSoundData()
{
	if (GameInstance == nullptr)
	{
		GameInstance = Cast<UProjectBGameInstance>(GetWorld()->GetGameInstance());
		if (GameInstance == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Instance Null"));
			return false;
		}
	}

	SoundData = GameInstance->GetSoundInfo(CharacterName);

	return true;
}

bool UDataComponent::LoadSkillData()
{
	if (GameInstance == nullptr)
	{
		GameInstance = Cast<UProjectBGameInstance>(GetWorld()->GetGameInstance());
		if (GameInstance == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Instance Null"));
			return false;
		}
	}

	AGameCharacter* OnwerActor = Cast<AGameCharacter>(GetOwner());

	if (OnwerActor->IsA(APlayerCharacter::StaticClass()))
	{
		APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(GetOwner());

		SkillData = GameInstance->GetSkillInfo(CharacterName);
		for (const auto& SD : SkillData)
		{
			OwnerCharacter->SkillComponent->AddChainData(SD.PreChainType, SD.ChainType, SD.AxisType, SD.ButtonType, SD.RequireStemina, CharacterName, SD.SkillName, SD.ComboStep);
		}

		return true;
	}
	else
	{
		AEnemy* OwnerCharacter = Cast<AEnemy>(OnwerActor);

		SkillData = GameInstance->GetSkillInfo(CharacterName);
		for (const auto& SD : SkillData)
		{
			OwnerCharacter->SkillComponent->AddChainData(SD.PreChainType, SD.ChainType, SD.AxisType, SD.ButtonType, SD.RequireStemina, CharacterName, SD.SkillName, SD.ComboStep);
		}

		return true;
	}
}

bool UDataComponent::LoadStatData()
{
	if (GameInstance == nullptr)
	{
		GameInstance = Cast<UProjectBGameInstance>(GetWorld()->GetGameInstance());
		if (GameInstance == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Instance Null"));
			return false;
		}
	}

	StatData = GameInstance->GetStatInfo(CharacterName);

	return true;
}

bool UDataComponent::GetCharacterStat(float& DefaultHp, float& DefaultStemina)
{
	DefaultHp = StatData.Hp;
	DefaultStemina = StatData.Stemina;
	return true;
}

UAnimMontage* UDataComponent::FindSkillMontage(FString SkillName)
{
	for (const auto& sd : SkillData)
	{
		if (sd.SkillName == SkillName)
		{
			return sd.AnimMontage;
		}
	}
	return nullptr;
}

UParticleSystem* UDataComponent::FindImpactEffect(FString SkillName)
{
	for (const auto& sd : SkillData)
	{
		if (sd.SkillName == SkillName)
		{
			return sd.ImpactEffect;
		}
	}
	return nullptr;
}

USoundCue* UDataComponent::FindImpactSound(FString SkillName)
{
	for (const auto& sd : SkillData)
	{
		if (sd.SkillName == SkillName)
		{
			return sd.ImpactSound;
		}
	}
	return nullptr;
}

FName UDataComponent::FindSkillMontageSection(FString SkillName, int32 ComboStep)
{
	for (const auto& sd : SkillData)
	{
		if (sd.SkillName == SkillName && sd.ComboStep == ComboStep)
		{
			return sd.SectionName;
		}
	}
	return "";
}

USoundCue* UDataComponent::FindCharacterSound(ECharacterSoundType SoundType)
{
	switch (SoundType)
	{
	case ECharacterSoundType::Pain:
		return SoundData.Pain;

	default:
		return nullptr;
	}

	return nullptr;
}

