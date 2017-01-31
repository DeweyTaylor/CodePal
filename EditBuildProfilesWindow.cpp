// (c) 2017 Dewey Taylor

#include "EditBuildProfilesWindow.h"
#include "MainWindow.h"

EditBuildProfilesWindow::EditBuildProfilesWindow(MainWindow* parent, vector<BuildProfile*> profiles)
	:
	BWindow(BRect(100,100,600,700), "Edit Build Profiles", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	fProfiles = profiles;
	fOpenFilePanel = NULL;
	fParent = parent;

	fProfileList = new BColumnListView("profile view", B_ALLOW_COLUMN_NONE);
	BStringColumn *column = new BStringColumn("profilename", 200, 50, 2000, B_TRUNCATE_MIDDLE);
	fProfileList->AddColumn(column, 0);
	fProfileList->SetSelectionMessage(new BMessage(PROFILECHANGED_MSG));

	fLibsListView = new BColumnListView("libs list view", B_ALLOW_COLUMN_NONE);
	fLocalIncludesListView = new BColumnListView("local includes list view", B_ALLOW_COLUMN_NONE);
	fSysIncludesListView = new BColumnListView("sys includes list view", B_ALLOW_COLUMN_NONE);

	column = new BStringColumn("lib path", 500,50,2000, B_TRUNCATE_MIDDLE);
	fLibsListView->AddColumn(column, 0);

	column = new BStringColumn("local path", 500, 50, 2000, B_TRUNCATE_MIDDLE);
	fLocalIncludesListView->AddColumn(column, 0);

	column = new BStringColumn("sys path", 500, 50, 2000, B_TRUNCATE_MIDDLE);
	fSysIncludesListView->AddColumn(column, 0);

	fOSMenu = new BPopUpMenu("os menu");
	fArchMenu = new BPopUpMenu("architecture menu");
	fCompilerMenu = new BPopUpMenu("compiler menu");

	fOSSelector = new BMenuField("os selector", "Target OS:", fOSMenu);
	fArchSelector = new BMenuField("architecture selector", "Target Architecture:", fArchMenu);
	fCompilerSelector = new BMenuField("compiler selector", "Compiler:", fCompilerMenu);

	fBuildForSize = new BCheckBox("size check", "Small binary size", new BMessage(SMALLSIZE_MSG));
	fDebugSymbols = new BCheckBox("debug check", "Debug symbols", new BMessage(DEBUG_MSG));
	fProfileInfo = new BCheckBox("profile info check", "Profiling info", new BMessage(PROFILING_MSG));

	fRunArgs = new BTextControl("runargs text", "Run Arguments:", "", new BMessage(RUNARGS_MSG));
	fPreScript = new BTextControl("prescript text", "Pre-build Script:", "", new BMessage(PRESCRIPT_MSG));
	fPostScript = new BTextControl("postscript text", "Post-build Script:", "", new BMessage(POSTSCRIPT_MSG));

	fFilesTabView = new BTabView("files tab view", B_WIDTH_FROM_LABEL);

	fLocalTab = new BTab();
	fSysTab = new BTab();
	fLibsTab = new BTab();

	fNewProfileBtn = new BButton("new profile button", "New", new BMessage(NEWPROFILE_MSG));
	fDeleteProfileBtn = new BButton("delete profile button", "Delete", new BMessage(DELETEPROFILE_MSG));
	fAddLibBtn = new BButton("add lib btn", "Add", new BMessage(ADDLIB_MSG));
	fDeleteLibBtn = new BButton("del lib btn", "Remove", new BMessage(DELLIB_MSG));
	fAddLocalBtn = new BButton("add loc btn", "Add", new BMessage(ADDLOC_MSG));
	fDeleteLocalBtn = new BButton("del loc btn", "Remove", new BMessage(DELLOC_MSG));
	fAddSysBtn = new BButton("add sys btn", "Add", new BMessage(ADDSYS_MSG));
	fDeleteSysBtn = new BButton("del sys btn", "Remove", new BMessage(DELSYS_MSG));

	BView *libview = new BView("libview", B_WILL_DRAW);
	BView *sysview = new BView("sysview", B_WILL_DRAW);
	BView *locview = new BView("locview", B_WILL_DRAW);

	libview->SetLayout(new BGroupLayout(B_HORIZONTAL));
	sysview->SetLayout(new BGroupLayout(B_HORIZONTAL));
	locview->SetLayout(new BGroupLayout(B_HORIZONTAL));

	sysview->AddChild(BGroupLayoutBuilder(B_VERTICAL, 5.0)
		.AddStrut(2.0)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5.0)
			.AddStrut(5.0)
			.Add(fAddSysBtn, 1.0)
			.Add(fDeleteSysBtn, 1.0)
			.AddGlue()
		)
		.Add(fSysIncludesListView, 1.0)
	);
	locview->AddChild(BGroupLayoutBuilder(B_VERTICAL, 5.0)
		.AddStrut(2.0)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5.0)
			.AddStrut(5.0)
			.Add(fAddLocalBtn, 1.0)
			.Add(fDeleteLocalBtn, 1.0)
			.AddGlue()
		)
		.Add(fLocalIncludesListView, 1.0)
	);
	libview->AddChild(BGroupLayoutBuilder(B_VERTICAL, 5.0)
		.AddStrut(2.0)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5.0)
			.AddStrut(5.0)
			.Add(fAddLibBtn, 1.0)
			.Add(fDeleteLibBtn, 1.0)
			.AddGlue()
		)
		.Add(fLibsListView, 1.0)
	);

	fFilesTabView->AddTab(locview, fLocalTab);
	fFilesTabView->AddTab(sysview, fSysTab);
	fFilesTabView->AddTab(libview, fLibsTab);

	fLocalTab->SetLabel("Local Include Directories");
	fSysTab->SetLabel("System Include Directories");
	fLibsTab->SetLabel("Libraries");

	SetLayout(new BGroupLayout(B_HORIZONTAL));
	AddChild(BGroupLayoutBuilder(B_VERTICAL, 5.0)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5.0)
			.AddStrut(5.0)
			.Add(BGroupLayoutBuilder(B_VERTICAL, 5.0)
				.AddStrut(5.0)
				.Add(fProfileList, 1.0)
				.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5.0)
					.AddGlue()
					.Add(fNewProfileBtn, 1.0)
					.Add(fDeleteProfileBtn, 1.0)
					.AddGlue()
				)
				.AddStrut(5.0)
			)
			.AddStrut(5.0)
			.Add(BGroupLayoutBuilder(B_VERTICAL, 5.0)
				.AddStrut(5.0)
				.Add(fOSSelector, 1.0)
				.Add(fArchSelector, 1.0)
				.Add(fCompilerSelector, 1.0)
				.Add(fBuildForSize, 1.0)
				.Add(fDebugSymbols, 1.0)
				.Add(fProfileInfo, 1.0)
				.Add(fRunArgs, 1.0)
				.Add(fPreScript, 1.0)
				.Add(fPostScript, 1.0)
				.AddGlue()
			)
			.AddStrut(5.0)
		)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5.0)
			.AddStrut(5.0)
			.Add(fFilesTabView, 1.0)
			.AddStrut(5.0)
		)
		.AddStrut(5.0)
	);

	// now that everything is laid out, this is where we populate the controls
	for (uint a = 0; a < profiles.size(); a++)
	{
		OneStringRow* row = new OneStringRow(profiles[a]->Name.c_str());
		fProfileList->AddRow(row);
	}
	for (uint a = 0; a < AVAILABLEOSCOUNT; a++)
	{
		BMessage* m = new BMessage(OSCHANGED_MSG);
		m->AddString("value", fAvailableOSes[a].c_str());
		fOSMenu->AddItem(new BMenuItem(fAvailableOSes[a].c_str(), m));
	}
	for (uint a = 0; a < AVAILABLECOMPILERCOUNT; a++)
	{
		BMessage* m = new BMessage(COMPILERCHANGED_MSG);
		m->AddString("value", fAvailableCompilers[a].c_str());
		fCompilerMenu->AddItem(new BMenuItem(fAvailableCompilers[a].c_str(), m));
	}
	for (uint a = 0; a < AVAILABLEARCHCOUNT; a++)
	{
		BMessage* m = new BMessage(ARCHCHANGED_MSG);
		m->AddString("value", fAvailableArchitectures[a].c_str());
		fArchMenu->AddItem(new BMenuItem(fAvailableArchitectures[a].c_str(), m));
	}
	// do everything else here (before setting the selected row)
	fProfileList->SetFocusRow(0, true);
}

EditBuildProfilesWindow::~EditBuildProfilesWindow()
{
	if (fOpenFilePanel != NULL)
	{
		delete fOpenFilePanel;
	}
}

void
EditBuildProfilesWindow::MessageReceived(BMessage *msg)
{
	int profidx = fProfileList->IndexOf(fProfileList->CurrentSelection());
	BuildProfile *prof = fProfiles[profidx];

	switch(msg->what)
	{
		case PROFILECHANGED_MSG:
		{
			_ProfileChanged();
			break;
		}
		case SMALLSIZE_MSG:
		{
			prof->Size = (fBuildForSize->Value() == 1) ? true : false;
			break;
		}
		case DEBUG_MSG:
		{
			prof->Debug = (fDebugSymbols->Value() == 1) ? true : false;
			break;
		}
		case PROFILING_MSG:
		{
			prof->Profile = (fProfileInfo->Value() == 1) ? true : false;
			break;
		}
		case RUNARGS_MSG:
		{
			prof->RunArgs = fRunArgs->Text();
			break;
		}
		case PRESCRIPT_MSG:
		{
			prof->PreScript = fPreScript->Text();
			break;
		}
		case POSTSCRIPT_MSG:
		{
			prof->PostScript = fPostScript->Text();
			break;
		}
		case NEWPROFILE_MSG:
		{
			InputPanel* i = new InputPanel(this, "New Profile", "New Profile Name", "Cancel", "Okay", new BMessage(NEWPROFILE2_MSG));
			i->Show();
			break;
		}
		case NEWPROFILE2_MSG:
		{
			BString str;
			msg->FindString("button", &str);
			string s1 = str.String();
			if (s1 == "Okay")
			{
				msg->FindString("value", &str);
				BuildProfile* newbp = new BuildProfile();
				newbp->Name = str.String();
				newbp->fCompiler = "(Default)";
				newbp->fTargetArch = "(Default)";
				newbp->fTargetOS = "(Default)";
				newbp->PreScript = "";
				newbp->PostScript = "";
				newbp->RunArgs = "";
				newbp->Debug = false;
				newbp->Profile = false;
				newbp->Size = false;
				fProfiles.push_back(newbp);
				OneStringRow* row = new OneStringRow(newbp->Name.c_str());
				fProfileList->AddRow(row);
			}
			break;
		}
		case DELETEPROFILE_MSG:
		{
			BRow* r = fProfileList->CurrentSelection();
			fProfileList->RemoveRow(r);
			string s = dynamic_cast<BStringField*>(r->GetField(0))->String();
			uint idx = 0;
			for (uint a = 0; a < fProfiles.size(); a++)
			{
				if (fProfiles[a]->Name == s)
				{
					idx = a;
					break;
				}
			}
			fProfiles.erase(fProfiles.begin() + idx);
			delete r;
			break;
		}
		case ADDLIB_MSG:
		{
			if (fOpenFilePanel != NULL)
			{
				delete fOpenFilePanel;
				fOpenFilePanel = NULL;
			}
			BMessenger* msgr = new BMessenger(this);
			fOpenFilePanel = new BFilePanel(B_OPEN_PANEL, msgr, NULL, B_FILE_NODE, false, new BMessage(ADDLIB2_MSG));
			fOpenFilePanel->Show();
			break;
		}
		case ADDLIB2_MSG:
		{
			entry_ref libref;
			msg->FindRef("refs", &libref);
			BEntry entry(&libref, true);
			BPath path;
			entry.GetPath(&path);
			prof->Libs.push_back(path.Path());
			OneStringRow* row = new OneStringRow(path.Path());
			fLibsListView->AddRow(row);
			break;
		}
		case DELLIB_MSG:
		{
			BRow* r = fLibsListView->CurrentSelection();
			fLibsListView->RemoveRow(r);
			string s = dynamic_cast<BStringField*>(r->GetField(0))->String();
			uint idx = 0;
			for (uint a = 0; a < prof->Libs.size(); a++)
			{
				if (prof->Libs[a] == s)
				{
					idx = a;
					break;
				}
			}
			prof->Libs.erase(prof->Libs.begin() + idx);
			delete r;
			break;
		}
		case ADDLOC_MSG:
		{
			if (fOpenFilePanel != NULL)
			{
				delete fOpenFilePanel;
				fOpenFilePanel = NULL;
			}
			BMessenger* msgr = new BMessenger(this);
			fOpenFilePanel = new BFilePanel(B_OPEN_PANEL, msgr, NULL, B_DIRECTORY_NODE, false, new BMessage(ADDLOC2_MSG));
			fOpenFilePanel->Show();
			break;
		}
		case ADDLOC2_MSG:
		{
			entry_ref libref;
			msg->FindRef("refs", &libref);
			BEntry entry(&libref, true);
			BPath path;
			entry.GetPath(&path);
			prof->LocalIncludes.push_back(path.Path());
			OneStringRow* row = new OneStringRow(path.Path());
			fLocalIncludesListView->AddRow(row);
			break;
		}
		case DELLOC_MSG:
		{
			BRow* r = fLocalIncludesListView->CurrentSelection();
			fLocalIncludesListView->RemoveRow(r);
			string s = dynamic_cast<BStringField*>(r->GetField(0))->String();
			uint idx = 0;
			for (uint a = 0; a < prof->LocalIncludes.size(); a++)
			{
				if (prof->LocalIncludes[a] == s)
				{
					idx = a;
					break;
				}
			}
			prof->LocalIncludes.erase(prof->LocalIncludes.begin() + idx);
			delete r;
			break;
		}
		case ADDSYS_MSG:
		{
			if (fOpenFilePanel != NULL)
			{
				delete fOpenFilePanel;
				fOpenFilePanel = NULL;
			}
			BMessenger* msgr = new BMessenger(this);
			fOpenFilePanel = new BFilePanel(B_OPEN_PANEL, msgr, NULL, B_DIRECTORY_NODE, false, new BMessage(ADDSYS2_MSG));
			fOpenFilePanel->Show();
			break;
		}
		case ADDSYS2_MSG:
		{
			entry_ref libref;
			msg->FindRef("refs", &libref);
			BEntry entry(&libref, true);
			BPath path;
			entry.GetPath(&path);
			prof->SysIncludes.push_back(path.Path());
			OneStringRow* row = new OneStringRow(path.Path());
			fSysIncludesListView->AddRow(row);
			break;
		}
		case DELSYS_MSG:
		{
			BRow* r = fSysIncludesListView->CurrentSelection();
			fSysIncludesListView->RemoveRow(r);
			string s = dynamic_cast<BStringField*>(r->GetField(0))->String();
			uint idx = 0;
			for (uint a = 0; a < prof->SysIncludes.size(); a++)
			{
				if (prof->SysIncludes[a] == s)
				{
					idx = a;
					break;
				}
			}
			prof->SysIncludes.erase(prof->SysIncludes.begin() + idx);
			delete r;
			break;
		}
		case OSCHANGED_MSG:
		{
			BString v;
			msg->FindString("value", &v);
			prof->fTargetOS = v.String();
			break;
		}
		case COMPILERCHANGED_MSG:
		{
			BString v;
			msg->FindString("value", &v);
			prof->fCompiler = v.String();
			break;
		}
		case ARCHCHANGED_MSG:
		{
			BString v;
			msg->FindString("value", &v);
			prof->fTargetArch = v.String();
			break;
		}
		default:
		{
			break;
		}
	}
}

bool
EditBuildProfilesWindow::QuitRequested()
{
	BMessage* m = new BMessage(NEWPROFILEDATA_MSG);
	fParent->_fHiddenNewBuildProfiles = fProfiles;
	fParent->PostMessage(m);
	return BWindow::QuitRequested();
}

void
EditBuildProfilesWindow::_ProfileChanged()
{
	int profidx = fProfileList->IndexOf(fProfileList->CurrentSelection());
	BuildProfile *prof = fProfiles[profidx];
	for (int a = 0; a < fOSMenu->CountItems(); a++)
	{
		BMenuItem* item = fOSMenu->ItemAt(a);
		if (item->Label() == prof->fTargetOS)
		{
			item->SetMarked(true);
		}
	}
	for (int a = 0; a < fArchMenu->CountItems(); a++)
	{
		BMenuItem* item = fArchMenu->ItemAt(a);
		if (item->Label() == prof->fTargetArch)
		{
			item->SetMarked(true);
		}
	}
	for (int a = 0; a < fCompilerMenu->CountItems(); a++)
	{
		BMenuItem* item = fCompilerMenu->ItemAt(a);
		if (item->Label() == prof->fCompiler)
		{
			item->SetMarked(true);
		}
	}
	fBuildForSize->SetValue(prof->Size ? 1 : 0);
	fDebugSymbols->SetValue(prof->Debug ? 1 : 0);
	fProfileInfo->SetValue(prof->Profile ? 1 : 0);
	fRunArgs->SetText(prof->RunArgs.c_str());
	fPreScript->SetText(prof->PreScript.c_str());
	fPostScript->SetText(prof->PostScript.c_str());
	fLibsListView->Clear();
	fLocalIncludesListView->Clear();
	fSysIncludesListView->Clear();
	OneStringRow *row;
	for (uint a = 0; a < prof->LocalIncludes.size(); a++)
	{
		row = new OneStringRow(prof->LocalIncludes[a].c_str());
		fLocalIncludesListView->AddRow(row);
	}
	for (uint a = 0; a < prof->SysIncludes.size(); a++)
	{
		row = new OneStringRow(prof->SysIncludes[a].c_str());
		fSysIncludesListView->AddRow(row);
	}
	for (uint a = 0; a < prof->Libs.size(); a++)
	{
		row = new OneStringRow(prof->Libs[a].c_str());
		fLibsListView->AddRow(row);
	}
}
