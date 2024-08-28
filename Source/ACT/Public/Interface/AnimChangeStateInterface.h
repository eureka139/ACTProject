#pragma once

#include "CoreMinimal.h"
#include "ACT/ACTData/ACTEnum.h"
#include "UObject/Interface.h"
#include "AnimChangeStateInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAnimChangeStateInterface : public UInterface
{
	GENERATED_BODY()
};

class ACT_API IAnimChangeStateInterface
{
	GENERATED_BODY()

public:

	virtual void ChangeState(EPlayerState IACTPlayerState);

};
