#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyEscapeMenuActor.generated.h"

class UKeyEscapeMainMenuWidget;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeMenuActor : public AActor
{
    GENERATED_BODY()

public:
    AKeyEscapeMenuActor();

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu")
    TSubclassOf<UKeyEscapeMainMenuWidget> MainMenuWidgetClass;

    UPROPERTY()
    UKeyEscapeMainMenuWidget* MainMenuWidget;
};