/*
 * Copyright 1999, Michał Kowalski
 * Copyright 2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Michał Kowalski
 *		Humdinger
 */


#include "App.h"
#include "View.h"

#include <Alert.h>
#include <Catalog.h>
#include <Deskbar.h>
#include <Roster.h>

#include <stdio.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Application"

App::App()
	:
	BApplication(kApplicationSignature)
{
	BDeskbar deskbar;

	if (!deskbar.IsRunning())
		return;

	// if the replicant is already running, remove it
	if (deskbar.HasItem(kViewSignature))
		_RemoveViewFromDeskbar();
	else
		_AddViewToDeskbar();
}

void
App::_AddViewToDeskbar()
{
	app_info appInfo;
	be_app->GetAppInfo(&appInfo);

	BDeskbar deskbar;

	status_t res = deskbar.AddItem(&appInfo.ref);
	if (res != B_OK)
		printf("Failed adding deskbar icon: %" B_PRId32 "\n", res);
}


void
App::_RemoveViewFromDeskbar()
{
	BDeskbar deskbar;
	int32 found_id;
	if (deskbar.GetItemInfo(kViewSignature, &found_id) == B_OK) {
		status_t err = deskbar.RemoveItem(found_id);
		if (err != B_OK) {
			printf("WorkspaceNumber: Error removing replicant id "
				   "%" B_PRId32 ": %s\n",
				found_id, strerror(err));
		}
	}
}


int
main()
{
	new App();
	delete be_app;
	return 0;
}
