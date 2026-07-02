#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyEscapePauseMenuWidget.generated.h"

class UButton;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API UKeyEscapePauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pause Menu")
    FName MainMenuLevelName;

private:
    UFUNCTION()
    void ResumeGame();

    UFUNCTION()
    void ReturnToMainMenu();

    UFUNCTION()
    void QuitGame();
};