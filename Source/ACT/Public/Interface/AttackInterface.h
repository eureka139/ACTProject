#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAttackInterface : public UInterface
{
	GENERATED_BODY()
};

class ACT_API IAttackInterface
{
	GENERATED_BODY()

public:

	virtual void CanCombo();

	virtual void ResetCombo();

};
