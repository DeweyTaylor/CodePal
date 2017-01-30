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
	fBuildProfileSelector = new BMenuField("build profile selector", "Build Profile:", fBuildProfileMenu);

	fMenuBar = new BMenuBar("menubar");

	fProjectMenu = new BMenu("Project");

	fProjectMenu->AddItem(new BMenuItem("New Project...", new BMessage(NEWPROJECT_MSG)));
	fProjectMenu->AddItem(new BMenuItem("Open Project...", new BMessage(OPENPROJECT_MSG)));
	fProjectMenu->AddSeparatorItem();
	fProjectMenu->AddItem(new BMenuItem("Exit", new BMessage(EXIT_MSG)));

	fSettingsMenu = new BMenu("Settings");

	fSettingsMenu->AddItem(new BMenuItem("Build Profiles...", new BMessage(EDITBUILDPROFILES_MSG)));

	fMenuBar->AddItem(fProjectMenu);
	fMenuBar->AddItem(fSettingsMenu);

	fProjectTabView = new BTabView("project view");
	fDebugTabView = new BTabView("debug view");
	fLeftTabView = new BTabView("left view");
	fRightTabView = new BTabView("right view");
	fOutputTabView = new BTabView("output view");
	fProjectTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fDebugTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fLeftTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fRightTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fOutputTabView->SetExplicitMinSize(BSize(1.0, 1.0));

	SetLayout(new BGroupLayout(B_HORIZONTAL));
	AddChild(BGroupLayoutBuilder(B_VERTICAL, 0.0)
		.Add(fMenuBar, 1.0)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5.0)
			.AddStrut(5)
			.Add(fBuildProfileSelector, 1.0)
			.AddGlue().AddGlue().AddGlue()
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
		case EDITBUILDPROFILES_MSG:
		{
			if (fProject == NULL)
			{
				return;
			}
			vector<BuildProfile*> profiles = fProject->GetBuildProfiles();
			EditBuildProfilesWindow *editwindow = new EditBuildProfilesWindow(profiles);
			editwindow->Show();
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

	BColumnListView *newoutline = new BColumnListView("project view", B_ALLOW_COLUMN_NONE);
	BStringColumn *newcolumn = new BStringColumn("projectdata", 200, 50, 2000, B_TRUNCATE_MIDDLE);
	//newcolumn->SetShowHeading(false);
	newoutline->AddColumn(newcolumn, 0);

	fProjectTabView->AddTab(newoutline, newtab);
	newtab->SetLabel(fProject->ProjectName.c_str());

	// populate the build project menu
	vector<string> buildproflist = fProject->GetBuildProfileList();
	for (uint a = 0; a < buildproflist.size(); a++)
	{
		fBuildProfileMenu->AddItem(new BMenuItem(buildproflist[a].c_str(), new BMessage(UPDATEBUILDPROFILE_MSG)));
	}
	fBuildProfileMenu->ItemAt(0)->SetMarked(true);

	// populate the project tab
	vector<CompileTarget*> targettree = fProject->GetTargetTree();
	for (uint a = 0; a < targettree.size(); a++)
	{
		OneStringRow* target = new OneStringRow(targettree[a]->Name.c_str());
		newoutline->AddRow(target);
		for (uint b = 0; b < targettree[a]->Groups.size(); b++)
		{
			OneStringRow* group = new OneStringRow(targettree[a]->Groups[b]->Name.c_str());
			newoutline->AddRow(group, target);
			for (uint c = 0; c < targettree[a]->Groups[b]->Sources.size(); c++)
			{
				OneStringRow* file = new OneStringRow(targettree[a]->Groups[b]->Sources[c]->Path.c_str());
				newoutline->AddRow(file, group);
			}
		}
	}
}
