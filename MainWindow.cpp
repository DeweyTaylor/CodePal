// (c) 2017 Dewey Taylor

#include "MainWindow.h"

#include <Application.h>
#include <Menu.h>
#include <MenuItem.h>
#include <View.h>
#include <GroupLayoutBuilder.h>

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,400,700),"CodePal",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	fProject = NULL;
	fBuildProfileMenu = new BPopUpMenu("build profile menu");
	fBuildProfileSelector = new BMenuField("build profile selector", "Build Profile:", fBuildProfileMenu);
	fProjectContextMenu = new BPopUpMenu("project context menu", false, false);

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
	fPropertiesTabView = new BTabView("properties view");
	fProjectTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fDebugTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fLeftTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fRightTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fOutputTabView->SetExplicitMinSize(BSize(1.0, 1.0));
	fPropertiesTabView->SetExplicitMinSize(BSize(1.0, 1.0));

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
				.Add(BSplitLayoutBuilder(B_VERTICAL, 0.0)
					.Add(fProjectTabView, 1.0)
					.Add(fPropertiesTabView, 1.0)
				)
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

	((BSplitView*)fProjectTabView->Parent()->Parent())->SetItemCollapsed(1, true);
	((BSplitView*)fOutputTabView->Parent())->SetItemCollapsed(1, true);
	fSettingsMenu->SetEnabled(false);
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case NEWPROJECT_MSG:
		{
			BMessenger target(this);
			BFilePanel *newpanel = new BFilePanel(B_SAVE_PANEL, &target, NULL, B_FILE_NODE, false, new BMessage(NEWPROJECT2_MSG), NULL, true, true);
			newpanel->Show();
			break;
		}
		case NEWPROJECT2_MSG:
		{
			BMessage* msg2 = new BMessage(*msg);
			msg2->what = NEWPROJECT3_MSG;
			InputPanel* panel = new InputPanel(this, "Project Name", "Enter the name of the new project here:", "Okay", "Cancel", msg2);
			panel->Show();
			break;
		}
		case NEWPROJECT3_MSG:
		{
			// make sure "okay" was pressed
			BString btn;
			msg->FindString("button", &btn);
			if (btn != "Okay")
			{
				break;
			}

			if (fProject != NULL)
			{
				fProject->Close();
				delete fProject;
			}

			fProject = new ProjectController();

			// set name
			BString name;
			msg->FindString("value", &name);
			fProject->ProjectName = name;
			// set directory
			entry_ref dir;
			if (msg->FindRef("directory", &dir) != B_OK)
			{
				// TODO: show error
				cout <<"no directory found"<<endl;
				break;
			}
			BEntry e(&dir, true);
			BPath p;
			e.GetPath(&p);
			fProject->ProjectDirectory = p.Path();

			// TODO: check extension
			BString fname;
			if (msg->FindString("name", &fname) != B_OK)
			{
				// TODO: show error
				cout <<"no name found" << endl;
				break;
			}
			if (fname.FindLast(".proj") != fname.Length() - 5)
			{
				fname << ".proj";
			}

			BString fullpath;
			fullpath << p.Path() << "/" << fname;

			// save and open the project
			if (fProject->SaveAs(fullpath.String()) == B_OK)
			{
				if (fProject->Load(fullpath.String()) == B_OK)
				{
					fSettingsMenu->SetEnabled(true);
					_PopulateProjectTab();
					_PopulateBuildProfileMenu();
				}
				else
				{
					cout<<"error loading :("<<endl;
					BAlert *alert = new BAlert("Error", "Error loading project file.", "Okay", NULL, NULL, B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_STOP_ALERT);
					alert->Go();
					fProject->Close();
					delete fProject;
				}
			}
			else
			{
				cout <<"error saving :("<<endl;
			}
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
			EditBuildProfilesWindow *editwindow = new EditBuildProfilesWindow(this, profiles);
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
				fSettingsMenu->SetEnabled(true);
				_PopulateProjectTab();
				_PopulateBuildProfileMenu();
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
		case NEWPROFILEDATA_MSG:
		{
			fProject->SetBuildProfiles(_fHiddenNewBuildProfiles);
			_PopulateBuildProfileMenu();
			break;
		}
		case PROJECTLISTSELECTION_MSG:
		{
			BPoint where;
			uint32 buttons;
			fProjectItemsView->GetMouse(&where, &buttons);
			//where.x += 2; // to prevent occasional select
			if (buttons & B_SECONDARY_MOUSE_BUTTON)
			{
				OneStringRow* r = (OneStringRow*)fProjectItemsView->CurrentSelection();
				while (fProjectContextMenu->CountItems() > 0)
				{
					fProjectContextMenu->RemoveItem(0);
				}
				string type = r->GetTypeID();
				if (type == "target")
				{
					fProjectContextMenu->AddItem(fNewTargetMenuItem);
					//fProjectContextMenu->AddItem(fRenameTargetMenuItem);
					fProjectContextMenu->AddItem(fRemoveTargetMenuItem);
					fProjectContextMenu->AddItem(fAddGroupMenuItem);
				}
				else if (type == "group")
				{
					//fProjectContextMenu->AddItem(fRenameGroupMenuItem);
					fProjectContextMenu->AddItem(fRemoveGroupMenuItem);
					fProjectContextMenu->AddItem(fNewFileMenuItem);
					fProjectContextMenu->AddItem(fAddFileMenuItem);
				}
				else if (type == "source")
				{
					//fProjectContextMenu->AddItem(fRenameFileMenuItem);
					fProjectContextMenu->AddItem(fRemoveFileMenuItem);
				}
				fProjectContextMenu->SetTargetForItems(this);
				fProjectContextMenu->Go(fProjectItemsView->ConvertToScreen(where), true, false, true);
			}
			break;
		}
		case NEWTARGET_MSG:
		{
			InputPanel* ip = new InputPanel(this, "New Target", "Name: ", "Cancel", "Okay", new BMessage(NEWTARGET2_MSG));
			ip->Show();
			// use InputPanel
			break;
		}
		case NEWTARGET2_MSG:
		{
			BString str;
			msg->FindString("button", &str);
			string s1 = str.String();
			if (s1 != "Okay")
			{
				return;
			}
			msg->FindString("value", &str);
			s1 = str.String();
			if (s1 == "")
			{
				// TODO: show error
				return;
			}
			// find the target that was selected
			OneStringRow* r = (OneStringRow*)fProjectItemsView->CurrentSelection();
			string value = r->GetValue();
			vector<CompileTarget*> targets = fProject->GetTargetTree();
			uint idx = -1;
			string targdir;
			string exec;
			for (uint a = 0; a < targets.size(); a++)
			{
				if (targets[a]->Name == value)
				{
					idx = a;
					targdir = targets[a]->TargetDir;
					exec = targets[a]->Executable;
					break;
				}
			}
			if (idx < 0)
			{
				// TODO: show error
				return;
			}
			vector<CompileTarget*>::iterator it = targets.begin() + idx + 1;
			CompileTarget* t = new CompileTarget();
			t->TargetDir = targdir;
			t->Executable = exec;
			t->Name = s1;
			targets.insert(it, 1, t);
			fProject->SetTargetTree(targets);
			fProject->SetChanged(true);
			_PopulateProjectTab();
			// TODO: select the new target
			cout<<"tab count (in newtarget): " << fProjectTabView->CountTabs() << endl;
			break;
		}
		//case RENAMETARGET_MSG: // unneeded (properties)
		//{
			// TODO: implement
			// use InputPanel
		//	break;
		//}
		case REMOVETARGET_MSG:
		{
			// TODO: implement
			break;
		}
		case ADDGROUP_MSG:
		{
			// TODO: implement
			// use InputPanel
			break;
		}
		//case RENAMEGROUP_MSG: // unneeded (properties)
		//{
			// TODO: implement
			// use InputPanel
		//	break;
		//}
		case REMOVEGROUP_MSG:
		{
			// TODO: implement
			break;
		}
		case NEWFILE_MSG:
		{
			// TODO: implement
			// use FilePanel
			break;
		}
		case ADDFILE_MSG:
		{
			// TODO: implement
			// use FilePanel
			break;
		}
		//case RENAMEFILE_MSG: // unneeded (properties)
		//{
			// TODO: implement
			// use InputPanel
		//	break;
		//}
		case REMOVEFILE_MSG:
		{
			// TODO: implement
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
	if (fProject != NULL)
	{
		fProject->Close();
	}
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

BPath
MainWindow::GetProjectPath()
{
	if (fProject)
	{
		return fProject->GetPath();
	}
	return BPath();
}

void
MainWindow::_PopulateProjectTab()
{
	string wintitle = "CodePal (" + fProject->GetName() + ")";
	SetTitle(wintitle.c_str());
	bool addtab = false;

	// due to bugs in BTabView, we need to delete the whole tab view and recreate it
	// (working with *just* the tab or the view in it results in the new view not displaying)
	BSplitView* parent = (BSplitView*)fProjectTabView->Parent();
	fProjectTabView->RemoveSelf();
	delete fProjectTabView;
	fProjectTabView = new BTabView("project view");
	parent->AddChild(0, fProjectTabView, 1.0);
	BTab* newtab = new BTab();

	BColumnListView *newoutline = new BColumnListView("project view", B_ALLOW_COLUMN_NONE);
	BStringColumn *newcolumn = new BStringColumn("projectdata", 200, 50, 2000, B_TRUNCATE_MIDDLE);
	newoutline->AddColumn(newcolumn, 0);

	newtab->SetLabel(fProject->ProjectName.c_str());
	fProjectTabView->AddTab(newoutline, newtab);

	// populate the project tab
	vector<CompileTarget*> targettree = fProject->GetTargetTree();
	for (uint a = 0; a < targettree.size(); a++)
	{
		OneStringRow* target = new OneStringRow(targettree[a]->Name.c_str(), "target");
		newoutline->AddRow(target);
		for (uint b = 0; b < targettree[a]->Groups.size(); b++)
		{
			OneStringRow* group = new OneStringRow(targettree[a]->Groups[b]->Name.c_str(), "group");
			newoutline->AddRow(group, target);
			for (uint c = 0; c < targettree[a]->Groups[b]->Sources.size(); c++)
			{
				OneStringRow* file = new OneStringRow(targettree[a]->Groups[b]->Sources[c]->Path.c_str(), "source");
				newoutline->AddRow(file, group);
			}
			newoutline->ExpandOrCollapse(group, targettree[a]->Groups[b]->Expanded);
		}
		newoutline->ExpandOrCollapse(target, true);
	}
	newoutline->SetSelectionMessage(new BMessage(PROJECTLISTSELECTION_MSG));
	fProjectItemsView = newoutline;
}

void
MainWindow::_PopulateBuildProfileMenu()
{
	// clear the old menu items
	while (fBuildProfileMenu->CountItems() > 0)
	{
		BMenuItem* item = fBuildProfileMenu->RemoveItem(0);
		delete item;
	}
	// populate the build project menu
	vector<string> buildproflist = fProject->GetBuildProfileList();
	for (uint a = 0; a < buildproflist.size(); a++)
	{
		fBuildProfileMenu->AddItem(new BMenuItem(buildproflist[a].c_str(), new BMessage(UPDATEBUILDPROFILE_MSG)));
	}
	if (fBuildProfileMenu->CountItems() > 0)
	{
		fBuildProfileMenu->ItemAt(0)->SetMarked(true);
	}
}
