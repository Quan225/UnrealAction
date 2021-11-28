#include "ProjectBGameInstance.h"
#include "SkillObject/SkillObject.h"
#include "SkillData.h"
#include "CharacterSkillComponent.h"

UProjectBGameInstance::UProjectBGameInstance()
{
	LoadEnemyInfo();
	LoadCharacterSkill();
	LoadItemInfo();
	LoadStatInfo();
	LoadSoundInfo();
}

void UProjectBGameInstance::Init()
{
	Super::Init();
}

FCharacterStatInfo UProjectBGameInstance::GetStatInfo(FString CharacterName)
{
	if (StatInfo.Contains(CharacterName))
		return StatInfo[CharacterName];

	return FCharacterStatInfo();
}

FCharacterSoundInfo UProjectBGameInstance::GetSoundInfo(FString CharacterName)
{
	if (SoundInfo.Contains(CharacterName))
		return SoundInfo[CharacterName];

	return FCharacterSoundInfo();
}

FEnemyInfo UProjectBGameInstance::GetEnemyInfo(FString EnemyName)
{
	for (int32 i = 0; i < EnemyInfo.Num(); i++)
	{
		if (EnemyInfo[i].CharacterName == EnemyName)
			return EnemyInfo[i];
	}
	return FEnemyInfo();
}

FItemDataTable UProjectBGameInstance::GetItemInfoByIdx(int32 ItemIdx)
{
	for (int32 i = 0; i < ItemInfo.Num(); i++)
	{
		if (ItemInfo[i].ItemIdx == ItemIdx)
			return ItemInfo[i];
	}

	return FItemDataTable();
}

FItemDataTable UProjectBGameInstance::GetItemInfoByName(FString ItemName)
{
	for (int32 i = 0; i < ItemInfo.Num(); i++)
	{
		if (ItemInfo[i].ItemName == ItemName)
			return ItemInfo[i];
	}

	return FItemDataTable();
}

TArray<FCharacterSkillInfo> UProjectBGameInstance::GetSkillInfo(FString CharacterName)
{
	if(!SkillInfo.Contains(CharacterName))
		return TArray<FCharacterSkillInfo>();

	return SkillInfo[CharacterName].Infos;
}

TArray<FCharacterSkillInstance> UProjectBGameInstance::GetSkillClass(FString CharacterName)
{
	if(!SkillInstanceInfo.Contains(CharacterName))
		return TArray<FCharacterSkillInstance>();

	return SkillInstanceInfo[CharacterName].Instances;
}

USkillObject* UProjectBGameInstance::FindSkillObject(FString CharacterName, FString SkillName, int32 ChainStep)
{
	if (!SkillInstanceInfo.Contains(CharacterName))
		return nullptr;

	for (const auto& inst : SkillInstanceInfo[CharacterName].Instances)
	{
		/*UE_LOG(LogTemp, Log, TEXT("%s : %d"), *inst.SkillObject->SkillName, inst.SkillObject->ChainStep);
		UE_LOG(LogTemp, Log, TEXT("%s : %d"), *SkillName, ChainStep);*/

		if (!inst.SkillName.Equals(SkillName))
			continue;

		if (inst.ChainStep != ChainStep)
			continue;

		return inst.SkillObject;
	}
	
	return nullptr;
}

bool UProjectBGameInstance::LoadEnemyInfo()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> tb(TEXT("DataTable'/Game/BluePrint/EnemyCharacterInfo.EnemyCharacterInfo'"));
	if (!tb.Succeeded())
		return false;

	FString context;
	TArray<FCharacterInfo*> row;
	tb.Object->GetAllRows(context, row);

	for (int32 i = 0; i < row.Num(); i++) {
		FEnemyInfo NewInfo;
		NewInfo.CharacterName = row[i]->CharacterName;
		NewInfo.Hp = row[i]->Hp;
		NewInfo.AnimInstRoute = row[i]->AnimInstRoute;
		NewInfo.SkeletalMesh = row[i]->SkeletalMesh;
		NewInfo.BTree = row[i]->BTree;
		NewInfo.BBoard = row[i]->BBoard;
		NewInfo.Effect = row[i]->Effect;

		EnemyInfo.Add(NewInfo);
	}

	return true;
}

bool UProjectBGameInstance::LoadItemInfo()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> tb(TEXT("DataTable'/Game/BluePrint/ItemData.ItemData'"));
	if (!tb.Succeeded())
		return false;

	FString context;
	TArray<FItemDataTable*> datas;
	tb.Object->GetAllRows(context, datas);

	for (int32 i = 0; i < datas.Num(); i++) {
		FItemDataTable itemTemp;
		itemTemp.ItemIdx = datas[i]->ItemIdx;
		itemTemp.ItemName = datas[i]->ItemName;
		itemTemp.Thumbnail = datas[i]->Thumbnail;
		itemTemp.InWorldStaticMesh = datas[i]->InWorldStaticMesh;
		itemTemp.ItemType = datas[i]->ItemType;
		itemTemp.bIsStackable = datas[i]->bIsStackable;
		itemTemp.ItemEffect = datas[i]->ItemEffect;
		itemTemp.Description = datas[i]->Description;

		ItemInfo.Add(itemTemp);
	}

	return true;
}

bool UProjectBGameInstance::LoadCharacterSkill()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> tb(TEXT("DataTable'/Game/BluePrint/ProgrammerOnly/CharacterSkill.CharacterSkill'"));
	if (!tb.Succeeded())
		return false;

	FString context;
	TArray<FCharacterSkill*> row;
	tb.Object->GetAllRows(context, row);

	for (int32 i = 0; i < row.Num(); i++) {
		if (LoadSkillInstanceInfo(row[i]->CharacterName, row[i]->SkillClass) == false)
			continue;
		
		LoadSkillInfo(row[i]->CharacterName, row[i]->SkillInfo);
	}

	return true;
}

bool UProjectBGameInstance::LoadSkillInstanceInfo(FString CharacterName, FString FileRoute)
{
	ConstructorHelpers::FObjectFinder<UDataTable> tb(*FileRoute);
	if (!tb.Succeeded())
		return false;

	FString context;
	TArray<FCharacterSkillClass*> datas;
	tb.Object->GetAllRows(context, datas);

	for (int32 i = 0; i < datas.Num(); i++) {
		FCharacterSkillInstance skillTemp;

		ConstructorHelpers::FClassFinder<USkillObject>loadSKillObject(*datas[i]->ReferenceRoute);
		if (loadSKillObject.Class != nullptr)
		{
			skillTemp.SkillObject = loadSKillObject.Class.GetDefaultObject();
			
			if (skillTemp.SkillObject == nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("%s Is not loaded : can't load object"), *FString(datas[i]->SkillName));
				continue;
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("%s Is not loaded : can't find skill reference"), *FString(datas[i]->SkillName));
			continue;
		}

		skillTemp.SkillObject->SkillName = datas[i]->SkillName;
		skillTemp.SkillName = datas[i]->SkillName;
		skillTemp.ChainStep = datas[i]->ChainStep;

		if (SkillInstanceInfo.Contains(CharacterName))
		{
			SkillInstanceInfo[CharacterName].Instances.Add(skillTemp);
		}
		else
		{
			FSkillInstanceArray instanceArray;
			instanceArray.Instances.Add(skillTemp);

			SkillInstanceInfo.Add(CharacterName, instanceArray);
		}
		
	}

	return true;
}

bool UProjectBGameInstance::LoadSkillInfo(FString CharacterName, FString FileRoute)
{
	ConstructorHelpers::FObjectFinder<UDataTable> infoTable(*FileRoute);
	if (!infoTable.Succeeded())
		return false;

	UDataTable* SkillDataTable = infoTable.Object;
	FString context;
	TArray<FSkillDataTable*> datas;
	SkillDataTable->GetAllRows<FSkillDataTable>(context, datas);

	for (int32 i = 0; i < datas.Num(); i++) {
		FCharacterSkillInfo info;
		info.SkillName = datas[i]->SkillName;
		info.AnimMontage = datas[i]->AnimMontage;
		info.ImpactEffect = datas[i]->ImpactEffect;
		info.ImpactSound = datas[i]->ImpactSound;
		info.ComboStep = datas[i]->ComboStep;
		info.RequireStamina = datas[i]->RequireStamina;
		info.Damage = datas[i]->Damage;
		info.SectionName = datas[i]->SectionName;
		
		/* Skill Mapping*/
		info.PreChainType = datas[i]->PreChainType;
		info.ChainType = datas[i]->ChainType;
		info.AxisType = datas[i]->AxisType;
		info.ButtonType = datas[i]->ButtonType;

		/* Effect */
		info.bIsForceCancel = false;

		if (!datas[i]->Effect.IsEmpty())
		{
			TArray<FString> EffectArr = {};
			datas[i]->Effect.ParseIntoArray(EffectArr, TEXT(" "));

			for (int32 j = 0; j < EffectArr.Num(); j++)
			{
				FString CurEffect = *EffectArr[j];
				if (CurEffect.Compare(TEXT("bIsForceCancel")) == 0)
				{
					info.bIsForceCancel = true;
				}
			}
		}

		if (SkillInfo.Contains(CharacterName))
		{
			SkillInfo[CharacterName].Infos.Add(info);
		}
		else
		{
			FSkillInfoArray infoArray;
			infoArray.Infos.Add(info);

			SkillInfo.Add(CharacterName, infoArray);
		}
	}


	return true;
}

bool UProjectBGameInstance::LoadStatInfo()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> tb(TEXT("DataTable'/Game/BluePrint/CharacterStatData.CharacterStatData'"));
	if (!tb.Succeeded())
		return false;

	FString context;
	TArray<FCharacterStatDataTable*> row;
	tb.Object->GetAllRows(context, row);

	for (int32 i = 0; i < row.Num(); i++) {
		FCharacterStatInfo TempStatInfo;
		
		TempStatInfo.Hp = row[i]->Hp;
		TempStatInfo.Stamina = row[i]->Stamina;

		StatInfo.Add(row[i]->CharacterName, TempStatInfo);
	}
	return true;
}

bool UProjectBGameInstance::LoadSoundInfo()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> tb(TEXT("DataTable'/Game/BluePrint/CharacterSoundData.CharacterSoundData'"));
	if (!tb.Succeeded())
		return false;

	FString context;
	TArray<FCharacterSound*> row;
	tb.Object->GetAllRows(context, row);

	for (int32 i = 0; i < row.Num(); i++) {
		FCharacterSoundInfo NewInfo;
		NewInfo.Pain = row[i]->Pain;

		SoundInfo.Add(row[i]->CharacterName, NewInfo);
	}

	return true;
}
