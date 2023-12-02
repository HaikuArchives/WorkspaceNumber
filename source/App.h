/*
 * Copyright 1999, Michał Kowalski
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Michał Kowalski
 *
 */
#ifndef APP_H
#define APP_H


#include <Application.h>


#define kApplicationSignature "application/x-vnd.MKowalski-Workspace"
#define kViewSignature "WorkspaceNumberView"


class App : public BApplication {
public:
					App();

private:
	void			_AddViewToDeskbar();
	void			_RemoveViewFromDeskbar();
};

#endif // APP_H
