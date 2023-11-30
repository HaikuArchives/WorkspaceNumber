/*
 * Copyright 1999, Michał Kowalski
 * Copyright 2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Michał Kowalski
 *		Humdinger
 */
#ifndef VIEW_H
#define VIEW_H

#include <MessageRunner.h>
#include <PopUpMenu.h>
#include <View.h>


enum {
	MSG_REFRESH_DISP = 0,
	MSG_QUIT,
	MSG_ABOUT
};


class View : public BView
{
public:
					View(BRect rect);
					View(BMessage* archive);
					~View();

	virtual void 	AttachedToWindow();
	virtual void 	Draw(BRect rect);
	virtual void 	MessageReceived(BMessage* message);
	virtual void 	MouseDown(BPoint point);

	virtual status_t Archive(BMessage* dataMsg, bool deep = true) const;
	static View* 	Instantiate(BMessage* dataMsg);

private:
			void	_Remove();
			void	_ShowAbout();

	int32			fCurrentWorkspace;
	BPopUpMenu*		fPopup;
	BMessageRunner*	fRunner;
};

#endif // VIEW_H

