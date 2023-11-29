/*
 * Copyright 1999, Michał Kowalski
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Michał Kowalski
 *
 */


#include "App.h"
#include "View.h"

#include <Alert.h>
#include <Deskbar.h>


App::App()
	:
	BApplication(APP_SIG)
{
	BDeskbar* deskbar = new BDeskbar();
	// if already installed - remove
	if (deskbar->HasItem(VIEW_SIG)) {
		status_t status = deskbar->RemoveItem(VIEW_SIG);
		if (status != B_OK)
			(new BAlert(NULL, strerror(status), "OK"))->Go(0);
	} else {
		View* replicant = new View(BRect(0, 0, 15, 15));
		status_t status = deskbar->AddItem(replicant);
		if (status != B_OK)
			(new BAlert(NULL, strerror(status), "OK"))->Go(0);
	}
}


int
main()
{
	new App();
	delete be_app;
	return 0;
}
