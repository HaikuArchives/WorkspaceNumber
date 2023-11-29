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


#define APP_SIG "application/x-vnd.MKowalski-Workspace"
#define VIEW_SIG "CurrentWorkspaceNo"


class App : BApplication {
public:
				App();
};

#endif // APP_H
