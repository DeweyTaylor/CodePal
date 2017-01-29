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
	fProject = NULL;
	fBuildProfileMenu = new BPopUpMenu("build profile menu");
	fBuildProfileSelector = new BMenuField(BRect(0,0,1,1), "build profile selector", "Build Profile", fBuildProfileMenu);

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
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5.0)
			//.Add(fToolBar, 1.0)
			.AddStrut(5)
			.Add(new BStringView(BRect(0,0,1,1), "build profile menu label", "Build Profile:"))
			.Add(fBuildProfileSelector, 1.0)
		)
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

	((BSplitView*)fProjectTabView->Parent())->SetItemCollapsed(1, true);
	((BSplitView*)fOutputTabView->Parent())->SetItemCollapsed(1, true);
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case NEWPROJECT_MSG:
		{
			break;
		}
		case OPENPROJECT_MSG:
		{
			BMessenger target(this);
			BFilePanel *OpenPanel = new BFilePanel(B_OPEN_PANEL, &target, NULL, B_FILE_NODE, false, NULL, NULL, true, true);
			OpenPanel->Show();
			break;
		}
		case EXIT_MSG:
		{
			QuitRequested();
			break;
		}
		case B_REFS_RECEIVED:
		{
			if (fProject != NULL)
			{
				fProject->Close();
				delete fProject;
			}
			fProject = new ProjectController();
			if (fProject->Load(msg) == B_OK)
			{
				_PopulateProjectTab();
			}
			else
			{
				BAlert *alert = new BAlert("Error", "Error loading project file.", "Okay", NULL, NULL, B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_STOP_ALERT);
				alert->Go();
				fProject->Close();
				delete fProject;
			}
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

void
MainWindow::_PopulateProjectTab()
{
	string wintitle = "CodePal (" + fProject->GetName() + ")";
	SetTitle(wintitle.c_str());

	BTab *newtab = new BTab();
	newtab->SetLabel(fProject->ProjectName.c_str());
	BOutlineListView *newoutline = new BOutlineListView(BRect(0,0,1,1),"project view", B_MULTIPLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW);
	BScrollView *bsv = new BScrollView("project scroll view", newoutline, B_FOLLOW_ALL, 0, true, true);
	newoutline->SetLayout(new BGroupLayout(B_VERTICAL));
	//bsv->SetLayout(new BGroupLayout(B_VERTICAL));
	fProjectTabView->AddTab(bsv, newtab);

	//BTab *newtab = new  BTab(); //BTab(newoutline);
	newtab->SetLabel(fProject->ProjectName.c_str());
	//BScrollView* bsv = new BScrollView("project scroll view", newoutline, B_FOLLOW_ALL, 0, true, true);
	//bsv->SetLayout(new BGroupLayout(B_VERTICAL));
//newoutline->SetLayout(new BGroupLayout(B_VERTICAL));
	//fProjectTabView->AddTab(newoutline, newtab);
	//fProjectTabView->AddTab(bsv, newtab);
	//fProjectTabView->SetLayout(new BGroupLayout(B_VERTICAL));

	vector<string> buildproflist = fProject->GetBuildProfileList();
	for (int a = 0; a < buildproflist.size(); a++)
	{
		fBuildProfileMenu->AddItem(new BMenuItem(buildproflist[a].c_str(), new BMessage(UPDATEBUILDPROFILE_MSG)));
	}
	fBuildProfileMenu->ItemAt(0)->SetMarked(true);
	//fProjectItemsView = new BOutlineListView(BRect(0,0,1,1), "project items view");
	//newtab->SetView(fProjectItemsView);
	//fProjectItemsView->SetResizingMode(B_FOLLOW_ALL);
	vector<string> targetlist = fProject->GetTargetList();
	for (int a = 0; a < targetlist.size(); a++)
	{
		BStringItem* newitem = new BStringItem(targetlist[a].c_str());
		newoutline->AddItem(newitem);
	}
	//newoutline->ResizeToPreferred();
}
