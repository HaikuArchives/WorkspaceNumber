// Display current workspace number in Deskbar
//
// (c) 1999 Michal Kowalski


#include "View.h"
#include <stdio.h>
#include "App.h"
#include <MenuItem.h>
#include <Deskbar.h>
#include <Alert.h>
#include <Screen.h>


CView::CView(BRect rect)
 : BView(rect, VIEW_SIG, B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS)
{
	m_nCurrWorkspace = -1;
}


// Instantiate CView from archive
CView* CView::Instantiate(BMessage* pData)
{
	if (!validate_instantiation(pData, "CView"))
		return NULL;
	return new CView(pData);
}


CView::CView(BMessage* pMessage) : BView(pMessage)
{
	SetResizingMode(B_FOLLOW_ALL_SIDES);
	SetFlags(B_WILL_DRAW);

	m_nCurrWorkspace = -1;

	SetDrawingMode(B_OP_COPY);

	m_Font = *be_bold_font;
	m_Font.SetSize(11);
	SetFont(&m_Font);
}


void CView::AttachedToWindow()
{
	SetViewColor(B_TRANSPARENT_COLOR);

	BMessenger messenger(this);
	// Use a BMessageRunner to deliver periodic messsages
	m_pMessageRunner.reset(new BMessageRunner(messenger, new BMessage(MSG_REFRESH_DISP), 200000, -1));

	// Build popup menu
	m_pPopupMenu.reset(new BPopUpMenu("PopUpMenu", false, false));
	m_pPopupMenu->AddItem(new BMenuItem("About...", new BMessage(MSG_ABOUT)));
	m_pPopupMenu->AddSeparatorItem();
	m_pPopupMenu->AddItem(new BMenuItem("Quit", new BMessage(MSG_QUIT)));
	m_pPopupMenu->SetTargetForItems(this);
}


// Remove Deskbar add-on
void CView::Remove()
{
	auto_ptr<BDeskbar> pDeskbar(new BDeskbar());
	status_t err = pDeskbar->RemoveItem(VIEW_SIG);
	if (err != B_OK)
		(new BAlert(NULL, strerror(err), "OK"))->Go(NULL);
}


// Display popup menu on right mouse click
void CView::MouseDown(BPoint ptPoint)
{
	MakeFocus(true);

	BPoint ptCursor;
	uint32 uButtons= 0;
	GetMouse(&ptCursor, &uButtons, true);

	if (uButtons & B_SECONDARY_MOUSE_BUTTON)
	{
		ConvertToScreen(&ptPoint);
		m_pPopupMenu->Bounds().PrintToStream();
		m_pPopupMenu->Go(ptPoint, true, true, BRect(ptPoint,ptPoint+BPoint(20,20)), true);
	}
}


void CView::MessageReceived(BMessage* pMsg)
{
	switch (pMsg->what)
	{
		case MSG_REFRESH_DISP:
		{
			// check if current workspace has changed
			int32 nCurrWorkspace= current_workspace();
			if (m_nCurrWorkspace != nCurrWorkspace)
			{
				m_nCurrWorkspace = nCurrWorkspace;
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
				"This app is free.", "OK"))->Go(NULL);
			break;

		default:
			CView::MessageReceived(pMsg);
			break;
	}
}


// Redraw current workspace number
void CView::Draw(BRect rect)
{
	if (m_nCurrWorkspace < 0)
		return;

	rect = Bounds();
	rgb_color rgbBlue=BScreen().DesktopColor();
	SetHighColor(Parent()->ViewColor());
	SetLowColor(Parent()->ViewColor());
	FillRect(rect);					// erase whole area
	SetHighColor(rgbBlue);
	FillEllipse(rect);				// draw circle
	char szBuf[16];
	sprintf(szBuf, "%d", int(m_nCurrWorkspace + 1));
	float fWidth= StringWidth(szBuf);
	SetLowColor(rgbBlue);			// set low color for right text anti-aliasing
	SetHighColor(255, 255, 255);	// white text color might not be readable sometimes...
	DrawString(szBuf, BPoint((rect.Width() - fWidth + 1.0) / 2.0, 12));
}


status_t CView::Archive(BMessage* pData, bool bDeep/* = true*/) const
{
	BView::Archive(pData, bDeep);
	pData->AddString("add_on", APP_SIG);
	pData->AddString("class", "CView");
	return B_OK;
}
