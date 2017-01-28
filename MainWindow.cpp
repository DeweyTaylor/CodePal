// (c) 2017 Dewey Taylor

#include "MainWindow.h"

#include <Application.h>
#include <Menu.h>
#include <MenuItem.h>
#include <View.h>
#include <GroupLayoutBuilder.h>

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,500,400),"CodePal",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	BRect r(Bounds());
	r.bottom = 20;
	fMenuBar = new BMenuBar(r,"menubar");

	fProjectMenu = new BMenu("Project");
	fSettingsMenu = new BMenu("Settings");

	fProjectMenu->AddItem(new BMenuItem("New Project...", new BMessage(NEWPROJECT_MSG)));
	fProjectMenu->AddItem(new BMenuItem("Open Project...", new BMessage(OPENPROJECT_MSG)));
	fProjectMenu->AddSeparatorItem();
	fProjectMenu->AddItem(new BMenuItem("Exit", new BMessage(EXIT_MSG)));

	fMenuBar->AddItem(fProjectMenu);
	fMenuBar->AddItem(fSettingsMenu);

	fMainView = new MainView(BRect(0,0,1,1), "main view", this);

	fProjectTabView = new BTabView(BRect(0,0,1,1), "project view");
	fDebugTabView = new BTabView(BRect(0,0,1,1), "debug view");
	fLeftTabView = new BTabView(BRect(0,0,1,1), "left view");
	fRightTabView = new BTabView(BRect(0,0,1,1), "right view");
	fOutputTabView = new BTabView(BRect(0,0,1,1), "output view");
	fProjectTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fDebugTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fLeftTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fRightTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fOutputTabView->SetExplicitMinSize(BSize(1.0, 1.0));

	SetLayout(new BGroupLayout(B_HORIZONTAL));
	AddChild(BGroupLayoutBuilder(B_VERTICAL, 0.0)
		.Add(fMenuBar, 1.0)
		.Add(BSplitLayoutBuilder(B_VERTICAL, 0.0)
			.Add(BSplitLayoutBuilder(B_HORIZONTAL, 0.0)
				.Add(fProjectTabView, 1.0)
				.Add(BSplitLayoutBuilder(B_VERTICAL, 0.0)
					.Add(fDebugTabView, 1.0)
					.Add(BSplitLayoutBuilder(B_HORIZONTAL, 0.0)
						.Add(fLeftTabView, 1.0) // BTabView (left editor view)
						.Add(fRightTabView, 1.0) // BTabView (right editor view)
					)
				)
			)
			.Add(fOutputTabView, 1.0)
		)
	);

	ProjectController pc;
	pc.Load("/boot/home/Desktop/mockup of CodePal project file");
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case NEWPROJECT_MSG:
		case OPENPROJECT_MSG:
		{
			break;
		}
		case EXIT_MSG:
		{
			QuitRequested();
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


bool
MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
