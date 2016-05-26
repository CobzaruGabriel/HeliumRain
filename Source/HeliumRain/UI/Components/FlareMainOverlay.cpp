
#include "../../Flare.h"
#include "FlareMainOverlay.h"
#include "../Components/FlareObjectiveInfo.h"

#include "../../Player/FlareHUD.h"
#include "../../Player/FlareMenuManager.h"

#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Runtime/Engine/Classes/Engine/RendererSettings.h"

#define LOCTEXT_NAMESPACE "FlareMainOverlay"


/*----------------------------------------------------
	Construct
----------------------------------------------------*/

void SFlareMainOverlay::Construct(const FArguments& InArgs)
{
	// Data
	MenuManager = InArgs._MenuManager;
	const FFlareStyleCatalog& Theme = FFlareStyleSet::GetDefaultTheme();
	TitleButtonWidth = 2.0f;
	TitleButtonHeight = 2.666667f;

	// Create the layout
	ChildSlot
	.VAlign(VAlign_Top)
	.HAlign(HAlign_Fill)
	[
		SNew(SVerticalBox)

		// Menu list
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(FMargin(0))
		.VAlign(VAlign_Top)
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(FMargin(0))
			.BorderImage(&Theme.BackgroundBrush)
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(0, 0, 0, 8))
				.BorderImage(&Theme.BackgroundBrush)
				[
					SAssignNew(MenuList, SHorizontalBox)

					// Title
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.HAlign(HAlign_Fill)
						.WidthOverride(0.9 * Theme.ContentWidth)
						[
							SNew(SHorizontalBox)

							// Title icon
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								SNew(SImage)
								.Image(this, &SFlareMainOverlay::GetCurrentMenuIcon)
							]

							// Title text
							+ SHorizontalBox::Slot()
							.VAlign(VAlign_Center)
							.AutoWidth()
							.Padding(FMargin(20, 0))
							[
								SNew(SVerticalBox)

								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(Theme.SmallContentPadding)
								[
									SNew(STextBlock)
									.TextStyle(&Theme.TitleFont)
									.Text(this, &SFlareMainOverlay::GetCurrentMenuName)
								]

								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(Theme.SmallContentPadding)
								[
									SNew(STextBlock)
									.TextStyle(&Theme.TextFont)
									.Text(this, &SFlareMainOverlay::GetSpacecraftInfo)
								]
							]
						]
					]
				]
			]
		]
	
		// Notifications
		+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Right)
		.Padding(FMargin(0, 200, 0, 0))
		[
			SNew(SBox)
			.HeightOverride(800)
			.VAlign(VAlign_Top)
			.Visibility(EVisibility::SelfHitTestInvisible)
			[
				SNew(SVerticalBox)

				// Objective
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SFlareObjectiveInfo)
					.PC(MenuManager->GetPC())
					.Visibility(EVisibility::SelfHitTestInvisible)
				]

				// Notifications
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(NotificationContainer, SVerticalBox)
				]
			]
		]
	];

	// Add menus
	TSharedPtr<SFlareButton> Button;
	AddMenuLink(EFlareMenu::MENU_Ship);
	AddMenuLink(EFlareMenu::MENU_Sector);
	AddMenuLink(EFlareMenu::MENU_Orbit);
	AddMenuLink(EFlareMenu::MENU_Leaderboard);
	AddMenuLink(EFlareMenu::MENU_Company);
	AddMenuLink(EFlareMenu::MENU_Fleet);
	AddMenuLink(EFlareMenu::MENU_Settings);
	AddMenuLink(EFlareMenu::MENU_Main);
	
	// Back button
	MenuList->AddSlot()
	.HAlign(HAlign_Right)
	[
		SAssignNew(Button, SFlareButton)
		.Width(TitleButtonWidth)
		.Height(TitleButtonHeight)
		.Transparent(true)
		.OnClicked(this, &SFlareMainOverlay::OnBack)
	];
	SetupMenuLink(Button, FFlareStyleSet::GetIcon("Back"), LOCTEXT("BackButtonTitle", "Back"));
	
	// Exit button
	MenuList->AddSlot()
	.HAlign(HAlign_Right)
	.AutoWidth()
	[
		SAssignNew(Button, SFlareButton)
		.Width(TitleButtonWidth)
		.Height(TitleButtonHeight)
		.Transparent(true)
		.OnClicked(this, &SFlareMainOverlay::OnOpenMenu, EFlareMenu::MENU_Exit)
	];
	SetupMenuLink(Button, AFlareMenuManager::GetMenuIcon(EFlareMenu::MENU_Exit), AFlareMenuManager::GetMenuName(EFlareMenu::MENU_Exit));

	// Init
	Close();
}

void SFlareMainOverlay::AddMenuLink(EFlareMenu::Type Menu)
{
	TSharedPtr<SFlareButton> Button;

	// Create button
	MenuList->AddSlot()
	.AutoWidth()
	.HAlign(HAlign_Left)
	[
		SAssignNew(Button, SFlareButton)
		.Width(TitleButtonWidth)
		.Height(TitleButtonHeight)
		.Transparent(true)
		.OnClicked(this, &SFlareMainOverlay::OnOpenMenu, Menu)
	];

	// Fill button contents
	SetupMenuLink(Button, AFlareMenuManager::GetMenuIcon(Menu), AFlareMenuManager::GetMenuName(Menu));
}

void SFlareMainOverlay::SetupMenuLink(TSharedPtr<SFlareButton> Button, const FSlateBrush* Icon, FText Text)
{
	const FFlareStyleCatalog& Theme = FFlareStyleSet::GetDefaultTheme();

	Button->GetContainer()->SetHAlign(HAlign_Center);
	Button->GetContainer()->SetVAlign(VAlign_Fill);
	Button->GetContainer()->SetPadding(FMargin(0, 25));

	Button->GetContainer()->SetContent(
		SNew(SVerticalBox)

		// Icon
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		[
			SNew(SBox)
			.WidthOverride(64)
			.HeightOverride(64)
			[
				SNew(SImage)
				.Image(Icon)
			]
		]

		// Text
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.TextStyle(&Theme.SmallFont)
			.Text(Text)
		]
	);
}


/*----------------------------------------------------
	Interaction
----------------------------------------------------*/

void SFlareMainOverlay::Open()
{
	IsOverlayVisible = true;
	MenuList->SetVisibility(EVisibility::Visible);
}

void SFlareMainOverlay::Close()
{
	IsOverlayVisible = false;
	MenuList->SetVisibility(EVisibility::Collapsed);
}

bool SFlareMainOverlay::IsOpen() const
{
	return IsOverlayVisible;
}

void SFlareMainOverlay::Notify(FText Text, FText Info, FName Tag, EFlareNotification::Type Type, float Timeout, EFlareMenu::Type TargetMenu, void* TargetInfo, FName TargetSpacecraft)
{
	// Remove notification with the same tag.
	if (Tag != NAME_None)
	{
		for (int Index = 0; Index < NotificationData.Num(); Index++)
		{
			if (NotificationData[Index]->IsDuplicate(Tag))
			{
				NotificationData[Index]->Finish();
				FLOG("SFlareMainOverlay::Notify : ignoring because it's duplicate");
			}
		}
	}

	// Add notification
	TSharedPtr<SFlareNotification> NotificationEntry;
	NotificationContainer->AddSlot()
	.AutoHeight()
	[
		SAssignNew(NotificationEntry, SFlareNotification)
		.MenuManager(MenuManager.Get())
		.Notifier(this)
		.Text(Text)
		.Info(Info)
		.Type(Type)
		.Tag(Tag)
		.Timeout(Timeout)
		.TargetMenu(TargetMenu)
		.TargetInfo(TargetInfo)
		.TargetSpacecraft(TargetSpacecraft)
	];

	// Store a reference to it
	NotificationData.Add(NotificationEntry);
}

void SFlareMainOverlay::FlushNotifications()
{
	for (auto& NotificationEntry : NotificationData)
	{
		NotificationEntry->Finish();
	}
}


/*----------------------------------------------------
	Callbacks
----------------------------------------------------*/

void SFlareMainOverlay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	// Get 2D position
	FVector2D MousePosition = MenuManager->GetPC()->GetMousePosition();
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));

	// Visibility check
	if (!IsOverlayVisible || (MousePosition.Y / ViewportScale) > AFlareMenuManager::GetMainOverlayHeight())
	{
		SetVisibility(EVisibility::HitTestInvisible);
	}
	else
	{
		SetVisibility(EVisibility::Visible);
	}

	// Destroy notifications when they're done with the animation
	int32 NotificationCount = 0;
	for (auto& NotificationEntry : NotificationData)
	{
		if (NotificationEntry->IsFinished())
		{
			NotificationContainer->RemoveSlot(NotificationEntry.ToSharedRef());
		}
		else
		{
			NotificationCount++;
		}
	}

	// Clean up the list when no notification is active
	if (NotificationCount == 0)
	{
		NotificationData.Empty();
	}
}

FText SFlareMainOverlay::GetCurrentMenuName() const
{
	FText Name;
	
	if (MenuManager->IsMenuOpen())
	{
		Name = AFlareMenuManager::GetMenuName(MenuManager->GetCurrentMenu());
	}
	else
	{
		Name = LOCTEXT("FlyingText", "Flying");
	}

	return FText::FromString(Name.ToString().ToUpper());
}

const FSlateBrush* SFlareMainOverlay::GetCurrentMenuIcon() const
{
	if (MenuManager->IsMenuOpen())
	{
		return AFlareMenuManager::GetMenuIcon(MenuManager->GetCurrentMenu());
	}
	else
	{
		return AFlareMenuManager::GetMenuIcon(EFlareMenu::MENU_Sector);
	}
}

FText SFlareMainOverlay::GetSpacecraftInfo() const
{
	AFlarePlayerController* PC = MenuManager->GetPC();

	if (MenuManager->IsMenuOpen() && MenuManager->GetCurrentMenu() == EFlareMenu::MENU_None)
	{
		return FText();
	}
	else if (PC->GetShipPawn())
	{
		return PC->GetShipPawn()->GetShipStatus();
	}
	else
	{
		// TODO : this should not happen anymore
		return FText::FromString(FString("No player ship found"));
	}

	return FText();
}

void SFlareMainOverlay::OnOpenMenu(EFlareMenu::Type Menu)
{
	if (MenuManager->IsSpacecraftMenu(Menu))
	{
		if (Menu == EFlareMenu::MENU_Ship)
		{
			AFlarePlayerController* PC = MenuManager->GetPC();
			MenuManager->OpenMenuSpacecraft(Menu, PC->GetShipPawn());
		}
		else
		{
			MenuManager->OpenMenuSpacecraft(Menu);
		}
	}
	else
	{
		MenuManager->OpenMenu(Menu);
	}
}

void SFlareMainOverlay::OnBack()
{
	MenuManager->Back();
}


/*----------------------------------------------------
	Getters
----------------------------------------------------*/

bool SFlareMainOverlay::IsFirstNotification(SFlareNotification* Notification)
{
	for (int i = 0; i < NotificationData.Num(); i++)
	{
		if (!NotificationData[i]->IsFinished())
		{
			return NotificationData[i].Get() == Notification;
		}
	}
	return false;
}

#undef LOCTEXT_NAMESPACE
