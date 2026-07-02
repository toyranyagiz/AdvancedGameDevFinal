#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyEscapeNoteWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API UKeyEscapeNoteWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Note")
    void SetupNote(const FText& NewTitle, const FText& NewBody);

protected:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* NoteTitleText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* NoteBodyText;

    UPROPERTY(meta = (BindWidget))
    UButton* CloseButton;

private:
    UFUNCTION()
    void CloseNote();
};