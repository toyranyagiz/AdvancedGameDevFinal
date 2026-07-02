#pragma once

#include "CoreMinimal.h"
#include "KeyEscapeItemBase.h"
#include "KeyEscapeMolotovItem.generated.h"

class AKeyEscapeMolotovProjectile;
class USoundBase;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeMolotovItem : public AKeyEscapeItemBase
{
    GENERATED_BODY()

public:
    AKeyEscapeMolotovItem();

public:
    virtual void UseItem(AKeyEscapeCharacter* UserCharacter) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Molotov")
    TSubclassOf<AKeyEscapeMolotovProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Molotov")
    float ThrowSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* ThrowSound;

};