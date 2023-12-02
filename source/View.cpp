/*
 * Copyright 1999, Michał Kowalski
 * Copyright 2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Michał Kowalski
 *		Humdinger
 */


#include "View.h"
#include "App.h"

#include <AboutWindow.h>
#include <Alert.h>
#include <Catalog.h>
#include <ColorConversion.h>
#include <Deskbar.h>
#include <MenuItem.h>
#include <Roster.h>
#include <Screen.h>

#include <stdio.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Application"


extern "C" BView* instantiate_deskbar_item(float maxWidth, float maxHeight);


BView*
instantiate_deskbar_item(float maxWidth, float maxHeight)
{
	return new View(BRect(0, 0, maxHeight - 1, maxHeight - 1));
}


View::View(BRect rect)
	:
	BView(rect, kViewSignature, B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS)
{
}


View::View(BMessage* archive)
	:
	BView(archive)
{
	BFont font = *be_bold_font;
	SetFont(&font);
}


void
View::AttachedToWindow()
{
	SetViewColor(B_TRANSPARENT_COLOR);

	// Build popup menu
	fPopup = new BPopUpMenu("PopUpMenu", false, false);
	fPopup->AddItem(new BMenuItem(B_TRANSLATE("About" B_UTF8_ELLIPSIS), new BMessage(MSG_ABOUT)));
	fPopup->AddSeparatorItem();
	fPopup->AddItem(new BMenuItem(B_TRANSLATE("Quit"), new BMessage(MSG_QUIT)));
	fPopup->SetTargetForItems(this);
}


void
View::Draw(BRect rect)
{
	rect = Bounds();
	rect.InsetBy(0.5, 0.5);
	rgb_color bgColor = BScreen().DesktopColor();
	SetLowColor(Parent()->ViewColor());

	SetHighColor(bgColor);
	FillEllipse(rect);

	// determine color for stroke / number string
	int brightness = BPrivate::perceptual_brightness(bgColor);
	if (brightness > 127)
		SetHighColor(16, 16, 16);
	else
		SetHighColor(240, 240, 240);

	StrokeEllipse(rect.InsetByCopy(1, 1));

	char buffer[16];
	sprintf(buffer, "%d", int(current_workspace() + 1));
	float width = StringWidth(buffer);
	// set low color for right text anti-aliasing
	SetLowColor(bgColor);

	BFont font;
	GetFont(&font);
	font_height fontHeight;
	font.GetHeight(&fontHeight);
	float y = (rect.bottom + rect.top
		+ ceilf(fontHeight.ascent) - ceilf(fontHeight.descent)) / 2 + 1;
	float x = (rect.Width() - width) / 2.0 + 1;
	DrawString(buffer, BPoint(x, y));
}


void
View::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case B_MOUSE_WHEEL_CHANGED:
		{
			float deltaY = message->FindFloat("be:wheel_delta_y");
			if (deltaY > 0.1)
				activate_workspace(current_workspace() + 1);
			else if (deltaY < -0.1)
				activate_workspace(current_workspace() - 1);
		} break;

		case B_WORKSPACE_ACTIVATED:
			Draw(Bounds());
			break;

		case MSG_QUIT:
			_Remove();
			break;

		case MSG_ABOUT:
			_ShowAbout();
			break;

		default:
			BView::MessageReceived(message);
			break;
	}
}


void
View::MouseDown(BPoint point)
{
	BPoint cursor;
	uint32 buttons = 0;
	GetMouse(&cursor, &buttons, true);

	if (buttons & B_SECONDARY_MOUSE_BUTTON) {
		ConvertToScreen(&point);
		fPopup->Go(point, true, true, BRect(point, point + BPoint(20, 20)), true);
	}
	if (buttons & B_PRIMARY_MOUSE_BUTTON)
		be_roster->Launch("application/x-vnd.Be-WORK"); // launch Workspaces applet
}


status_t
View::Archive(BMessage* dataMsg, bool deep) const
{
	status_t status = BView::Archive(dataMsg, deep);

	if (status == B_OK)
		status = dataMsg->AddString("add_on", kApplicationSignature);

	if (status == B_OK)
		status = dataMsg->AddString("class", kViewSignature);

	return status;
}


View*
View::Instantiate(BMessage* dataMsg)
{
	if (!validate_instantiation(dataMsg, kViewSignature))
		return NULL;

	return new View(dataMsg);
}


void
View::_Remove()
{
	BDeskbar* deskbar = new BDeskbar();
	status_t status = deskbar->RemoveItem(kViewSignature);
	if (status != B_OK)
		(new BAlert(NULL, strerror(status), B_TRANSLATE("OK")))->Go(NULL);
}


void
View::_ShowAbout()
{
	BAboutWindow* aboutwindow
		= new BAboutWindow(B_TRANSLATE_SYSTEM_NAME("WorkspaceNumber"), kApplicationSignature);

	const char* authors[] = {
		"Michal Kowalski",
		"Humdinger",
		NULL
	};

	aboutwindow->AddCopyright(1999, "Michal Kowalski");
	aboutwindow->AddAuthors(authors);

	aboutwindow->Show();
}
