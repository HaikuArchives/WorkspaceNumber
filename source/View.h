/*
 * Copyright 1999, Michał Kowalski
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Michał Kowalski
 *
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
	virtual void 	MessageReceived(BMessage* message);
	virtual void 	Draw(BRect rect);
	virtual status_t Archive(BMessage* dataMsg, bool deep = true) const;
	virtual void 	MouseDown(BPoint point);

	static View* 	Instantiate(BMessage* dataMsg);

private:
			void	Remove();

	BMessageRunner*	fRunner;
	int32			fCurrentWorkspace;
	BPopUpMenu*		fPopup;

};

#endif // VIEW_H

