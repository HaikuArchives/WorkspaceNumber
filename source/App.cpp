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
#include <Catalog.h>
#include <Deskbar.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Application"

App::App()
	:
	BApplication(kApplicationSignature)
{
	BDeskbar* deskbar = new BDeskbar();
	// if already installed - remove
	if (deskbar->HasItem(kViewSignature)) {
		status_t status = deskbar->RemoveItem(kViewSignature);
		if (status != B_OK)
			(new BAlert(NULL, strerror(status), B_TRANSLATE("OK")))->Go(0);
	} else {
		float height = deskbar->MaxItemHeight();
		View* replicant = new View(BRect(0, 0, height, height));
		status_t status = deskbar->AddItem(replicant);
		if (status != B_OK)
			(new BAlert(NULL, strerror(status), B_TRANSLATE("OK")))->Go(0);
	}
}


int
main()
{
	new App();
	delete be_app;
	return 0;
}
