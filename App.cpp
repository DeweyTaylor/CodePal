// (c) 2017 Dewey Taylor

#include "App.h"

App::App()
	:	BApplication(APP_SIG)
{
	_InstallMimeType();
	MainWindow *mainwin = new MainWindow();
	fWindowList = new WindowRef();
	fWindowList->Window = mainwin;
	fWindowList->Next = NULL;
	mainwin->Show();
}

void
App::_InstallMimeType()
{
	// TODO: fix
	BMimeType mt;
	mt.SetType(PROJ_SIG);
	mt.SetShortDescription("CodePal project");
	mt.SetLongDescription("Project file for CodePal projects");
	mt.SetPreferredApp(APP_SIG);

	BMessage msg;
	msg.AddString("extensions", "proj");
	mt.SetFileExtensions(&msg);
	mt.Install();
	return;

	/*image_info ii;
	int32 cookie = 0;
	while (get_next_image_info(0, &cookie, &ii) == B_OK)
	{
		if (ii.type == B_APP_IMAGE)
			break;
	}

	BPath mypath(ii.name);
	entry_ref theref;
	get_ref_for_path(mypath.Path(), &theref);

	mt.SetAppHint(&theref);
	if (B_OK != mt.SetPreferredApp(APP_SIG))
	{
		printf("app set error\n");
	}

	mt.Install();

	BFile f(mypath.Path(), B_READ_WRITE);
	BAppFileInfo afi(&f);
	afi.SetSignature(APP_SIG);
	*/
}

void
App::RefsReceived(BMessage* msg)
{
	// compare path in message to existing windows
	WindowRef* winref = fWindowList;
	//entry_ref ref;
	BPath p = BPath();
	msg->FindFlat("be:refs", &p);

	while (winref != NULL)
	{
		BPath path = winref->Window->GetProjectPath();

		if (p == path)
		{
			// if paths match, focus on existing window
			winref->Window->Activate(true);
			return;
		}
	}
	// if no match, create new window and load project
	winref = new WindowRef();
	winref->Window = new MainWindow();
	BMessage* message = new BMessage(B_REFS_RECEIVED);
	message->AddFlat("refs", &p);
	winref->Window->PostMessage(message);
	winref->Next = fWindowList;
	fWindowList = winref;
}

int
main(void)
{
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
