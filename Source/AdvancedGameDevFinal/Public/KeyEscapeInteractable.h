#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KeyEscapeInteractable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UKeyEscapeInteractable : public UInterface
{
    GENERATED_BODY()
};

class ADVANCEDGAMEDEVFINAL_API IKeyEscapeInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Interact(AActor* Interactor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    FText GetInteractionText() const;
};
