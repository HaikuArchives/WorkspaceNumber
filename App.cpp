// Display current workspace number in Deskbar
//
// (c) 1999 Michal Kowalski


#include "App.h"
#include <Deskbar.h>
#include <Alert.h>
#include "View.h"



CApp::CApp() : BApplication(APP_SIG)
{
	auto_ptr<BDeskbar> pDeskbar(new BDeskbar());
	if (pDeskbar->HasItem(VIEW_SIG))		// if already installed--remove
	{
		status_t err = pDeskbar->RemoveItem(VIEW_SIG);
		if (err != B_OK)
			(new BAlert(NULL, strerror(err), "OK"))->Go(0);
	}
	else
	{
		auto_ptr<CView> pReplicant(new CView(BRect(0,0,15,15)));
		status_t err= pDeskbar->AddItem(pReplicant.get());
		if (err != B_OK)
			(new BAlert(NULL, strerror(err), "OK"))->Go(0);
	}
}


int main()
{
	new CApp();
	delete be_app;
	return 0;
}
