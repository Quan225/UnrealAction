#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillObject.generated.h"

UCLASS()
class PROJECTB_API USkillObject : public UObject
{
	GENERATED_BODY()
	
public:
	USkillObject();
	UPROPERTY()
	FString SkillName;

public:
	virtual void ExecuteSkill();
	virtual void ExecuteSkill(class AGameCharacter* Caster);

	// ��Ƽ���̿��� ȣ���ϴ� �̺�Ʈ
	virtual void CustomHitEvent(class AGameCharacter* Caster);
	virtual void CustomSpawnEvent(class AGameCharacter* Caster);

protected:
	/* ������ ��ġ�� �� ������ �浹ü�� �߻����� �浹�� ���� ��ȯ�Ѵ�. */
	TArray<class AEnemy*> CheckTargetBySphere(UWorld* World, AActor* Caster, FVector SphereCenter, float SphereRadius, bool bDrawDebugObj = false);
};
