#include "KeyEscapeNoteWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "KeyEscapeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UKeyEscapeNoteWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CloseButton)
    {
        CloseButton->OnClicked.AddDynamic(this, &UKeyEscapeNoteWidget::CloseNote);
    }
}

void UKeyEscapeNoteWidget::SetupNote(const FText& NewTitle, const FText& NewBody)
{
    if (NoteTitleText)
    {
        NoteTitleText->SetText(NewTitle);
    }

    if (NoteBodyText)
    {
        NoteBodyText->SetAutoWrapText(true);
        NoteBodyText->SetWrapTextAt(650.0f);
        NoteBodyText->SetText(NewBody);
    }
}

void UKeyEscapeNoteWidget::CloseNote()
{
    APlayerController* PlayerController = GetOwningPlayer();

    if (PlayerController)
    {
        AKeyEscapeCharacter* Character = Cast<AKeyEscapeCharacter>(
            PlayerController->GetPawn()
        );

        if (Character)
        {
            Character->SetReadingNote(false);
        }

        FInputModeGameOnly GameInputMode;
        PlayerController->SetInputMode(GameInputMode);
        PlayerController->bShowMouseCursor = false;
    }

    RemoveFromParent();
}