// (c) 2017 Dewey Taylor

#include "EditBuildProfilesWindow.h"

EditBuildProfilesWindow::EditBuildProfilesWindow(vector<BuildProfile*> profiles)
	:
	BWindow(BRect(100,100,600,700), "Edit Build Profiles", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	fProfiles = profiles;

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
	// fProfileList
	for (uint a = 0; a < profiles.size(); a++)
	{
		OneStringRow* row = new OneStringRow(profiles[a]->Name.c_str());
		fProfileList->AddRow(row);
	}
	for (uint a = 0; a < AVAILABLEOSCOUNT; a++)
	{
		fOSMenu->AddItem(new BMenuItem(fAvailableOSes[a].c_str(), new BMessage(OSCHANGED_MSG)));
	}
	for (uint a = 0; a < AVAILABLECOMPILERCOUNT; a++)
	{
		fCompilerMenu->AddItem(new BMenuItem(fAvailableCompilers[a].c_str(), new BMessage(COMPILERCHANGED_MSG)));
	}
	for (uint a = 0; a < AVAILABLEARCHCOUNT; a++)
	{
		fArchMenu->AddItem(new BMenuItem(fAvailableArchitectures[a].c_str(), new BMessage(ARCHCHANGED_MSG)));
	}
	// do everything else here (before setting the selected row)
	fProfileList->SetFocusRow(0, true);
}

void
EditBuildProfilesWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case PROFILECHANGED_MSG:
		{
			_ProfileChanged();
			break;
		}
		default:
		{
			break;
		}
	}
}

void
EditBuildProfilesWindow::_ProfileChanged()
{
	// get the profile index
	int profidx = fProfileList->IndexOf(fProfileList->CurrentSelection());
	BuildProfile *prof = fProfiles[profidx];
}
