
#include "../../Flare.h"
#include "FlareConfirmationBox.h"


#define LOCTEXT_NAMESPACE "FlareConfirmationBox"


/*----------------------------------------------------
	Construct
----------------------------------------------------*/

void SFlareConfirmationBox::Construct(const FArguments& InArgs)
{
	const FFlareStyleCatalog& Theme = FFlareStyleSet::GetDefaultTheme();
	FullHide = InArgs._FullHide;

	// Create the layout
	ChildSlot
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SHorizontalBox)

		// Back button
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.AutoWidth()
		[
			SAssignNew(CancelButton, SFlareButton)
			.HelpText(LOCTEXT("Cancel", "Cancel"))
			.Text(InArgs._CancelText)
			.OnClicked(InArgs._OnCancelled)
		]

		// Buy button
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		[
			SAssignNew(ConfirmButton, SFlareButton)
			.HelpText(LOCTEXT("Confirm", "Confirm"))
			.OnClicked(InArgs._OnConfirmed)
			.Width(10)
			.Height(1)
		]

		// Transfert button
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		[
			SAssignNew(ConfirmFreeButton, SFlareButton)
			.HelpText(LOCTEXT("Confirm", "Confirm"))
			.Text(InArgs._ConfirmFreeText)
			.OnClicked(InArgs._OnConfirmed)
			.Width(10)
			.Height(1)
		]
	];

	// Buy button content
	ConfirmButton->GetContainer()->SetContent(
		SNew(SHorizontalBox)

		// Confirmation
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(Theme.ContentPadding)
		[
			SNew(STextBlock)
			.Text(InArgs._ConfirmText)
			.TextStyle(&Theme.TextFont)
		]

		// Cost icon
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		.Padding(Theme.ContentPadding)
		.AutoWidth()
		[
			SNew(SImage).Image(FFlareStyleSet::GetIcon("Cost"))
		]

		// Cost amount
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		.Padding(Theme.ContentPadding)
		.AutoWidth()
		[
			SAssignNew(CostLabel, STextBlock)
			.TextStyle(&Theme.TextFont)
		]
	);

	// Default is hidden
	Hide();
}


/*----------------------------------------------------
	Content
----------------------------------------------------*/

void SFlareConfirmationBox::Show(float Amount)
{
	if (Amount > 0)
	{
		CostLabel->SetText("+" + FString::FromInt(Amount));
		ConfirmButton->SetVisibility(EVisibility::Visible);
	}
	else
	{
		ConfirmFreeButton->SetVisibility(EVisibility::Visible);
	}


	CancelButton->SetVisibility(EVisibility::Visible);
}

void SFlareConfirmationBox::Hide()
{
	ConfirmButton->SetVisibility(EVisibility::Hidden);
	ConfirmFreeButton->SetVisibility(EVisibility::Hidden);

	if (FullHide)
	{
		CancelButton->SetVisibility(EVisibility::Hidden);
	}
}


#undef LOCTEXT_NAMESPACE