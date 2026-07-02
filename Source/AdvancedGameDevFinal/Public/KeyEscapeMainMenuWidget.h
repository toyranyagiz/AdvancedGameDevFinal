#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyEscapeMainMenuWidget.generated.h"

class UButton;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API UKeyEscapeMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* StartButton;

    UPROPERTY(meta = (BindWidget))
    UButton* TutorialButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    FName GameLevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    FName TutorialLevelName;

private:
    UFUNCTION()
    void StartGame();

    UFUNCTION()
    void StartTutorial();

    UFUNCTION()
    void QuitGame();
};