/*
 * Copyright 1999, Michał Kowalski
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Michał Kowalski
 *
 */


#include "View.h"
#include "App.h"

#include <Alert.h>
#include <Deskbar.h>
#include <MenuItem.h>
#include <Screen.h>

#include <stdio.h>


View::View(BRect rect)
	:
	BView(rect, VIEW_SIG, B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS)
{
	fCurrentWorkspace = -1;
}


// Instantiate View from archive
View*
View::Instantiate(BMessage* dataMsg)
{
	if (!validate_instantiation(dataMsg, "View"))
		return NULL;

	return new View(dataMsg);
}


View::View(BMessage* message)
	:
	BView(message)
{
	SetResizingMode(B_FOLLOW_ALL_SIDES);
	SetFlags(B_WILL_DRAW);

	fCurrentWorkspace = -1;

	SetDrawingMode(B_OP_COPY);

	fFont = *be_bold_font;
	fFont.SetSize(11);
	SetFont(&fFont);
}


void
View::AttachedToWindow()
{
	SetViewColor(B_TRANSPARENT_COLOR);

	BMessenger messenger(this);
	// Use a BMessageRunner to deliver periodic messsages
	fRunner	= new BMessageRunner(messenger, new BMessage(MSG_REFRESH_DISP), 200000, -1);

	// Build popup menu
	fPopup = new BPopUpMenu("PopUpMenu", false, false);
	fPopup->AddItem(new BMenuItem("About" B_UTF8_ELLIPSIS, new BMessage(MSG_ABOUT)));
	fPopup->AddSeparatorItem();
	fPopup->AddItem(new BMenuItem("Quit", new BMessage(MSG_QUIT)));
	fPopup->SetTargetForItems(this);
}


// Remove Deskbar add-on
void
View::Remove()
{
	BDeskbar* pDeskbar = new BDeskbar();
	status_t err = pDeskbar->RemoveItem(VIEW_SIG);
	if (err != B_OK)
		(new BAlert(NULL, strerror(err), "OK"))->Go(NULL);
}


// Display popup menu on right mouse click
void
View::MouseDown(BPoint point)
{
	MakeFocus(true);

	BPoint cursor;
	uint32 buttons = 0;
	GetMouse(&cursor, &buttons, true);

	if (buttons & B_SECONDARY_MOUSE_BUTTON) {
		ConvertToScreen(&point);
		fPopup->Bounds().PrintToStream();
		fPopup->Go(point, true, true, BRect(point, point + BPoint(20, 20)), true);
	}
}


void
View::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case MSG_REFRESH_DISP:
		{
			// check if current workspace has changed
			int32 workspace = current_workspace();
			if (fCurrentWorkspace != workspace) {
				fCurrentWorkspace = workspace;
				Draw(Bounds());
			}
			break;
		}

		case MSG_QUIT:
		{
			// Remove workspace indicator from Deskbar
			Remove();
			break;
		}

		case MSG_ABOUT:
			// Display about app info
			(new BAlert("WorkspaceNum",
				 "Current workspace number in Deskbar\n"
				 "© 1999 Michal Kowalski\n\n"
				 "This app is free.",
				 "OK"))
				->Go(NULL);
			break;

		default:
			BView::MessageReceived(message);
			break;
	}
}


// Redraw current workspace number
void
View::Draw(BRect rect)
{
	if (fCurrentWorkspace < 0)
		return;

	rect = Bounds();
	rgb_color bgColor = BScreen().DesktopColor();
	SetHighColor(Parent()->ViewColor());
	SetLowColor(Parent()->ViewColor());
	// erase whole area
	FillRect(rect);

	SetHighColor(bgColor);
	FillEllipse(rect);

	char buffer[16];
	sprintf(buffer, "%d", int(fCurrentWorkspace + 1));
	float width = StringWidth(buffer);
	// set low color for right text anti-aliasing
	SetLowColor(bgColor);
	SetHighColor(255, 255, 255);
	DrawString(buffer, BPoint((rect.Width() - width + 1.0) / 2.0, 12));
}


status_t
View::Archive(BMessage* dataMsg, bool deep /* = true*/) const
{
	BView::Archive(dataMsg, deep);
	dataMsg->AddString("add_on", APP_SIG);
	dataMsg->AddString("class", "View");

	return B_OK;
}
