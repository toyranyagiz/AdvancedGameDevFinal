#pragma once

#include "CoreMinimal.h"
#include "KeyEscapeItemBase.h"
#include "KeyEscapeFoodItem.generated.h"

class USoundBase;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeFoodItem : public AKeyEscapeItemBase
{
    GENERATED_BODY()

public:
    AKeyEscapeFoodItem();

public:
    virtual void UseItem(AKeyEscapeCharacter* UserCharacter) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food")
    float HealAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* EatSound;
};