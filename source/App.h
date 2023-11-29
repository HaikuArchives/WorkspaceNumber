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
#define kClassName "WorkspaceNumber"
#define kViewSignature "WorkspaceNumberView"


class App : BApplication {
public:
				App();
};

#endif // APP_H
