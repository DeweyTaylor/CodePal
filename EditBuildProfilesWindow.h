// (c) 2017 Dewey Taylor

#ifndef EDITBUILDPROFILESWINDOW_H
#define EDITBUILDPROFILESWINDOW_H

#include <string>
#include <InterfaceKit.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <ColumnListView.h>
#include <vector>

#include "CLVHelpers.h"
#include "ProjectController.h"

using namespace std;

#define SMALLSIZE_MSG		'smsz'
#define DEBUG_MSG			'dbug'
#define PROFILING_MSG		'prof'
#define RUNARGS_MSG			'rnrg'
#define PRESCRIPT_MSG		'pres'
#define POSTSCRIPT_MSG		'psts'
#define NEWPROFILE_MSG		'newp'
#define DELETEPROFILE_MSG	'delp'
#define ADDLIB_MSG			'alib'
#define DELLIB_MSG			'dlib'
#define ADDLOC_MSG			'aloc'
#define DELLOC_MSG			'dloc'
#define ADDSYS_MSG			'asys'
#define DELSYS_MSG			'dsys'
#define OSCHANGED_MSG		'osch'
#define COMPILERCHANGED_MSG	'cmch'
#define ARCHCHANGED_MSG		'arch'
#define PROFILECHANGED_MSG	'prch'

#define AVAILABLECOMPILERCOUNT	1
#define AVAILABLEOSCOUNT		1
#define AVAILABLEARCHCOUNT		3

class EditBuildProfilesWindow : public BWindow
{
	public:
				EditBuildProfilesWindow(vector<BuildProfile*>);
		void	MessageReceived(BMessage*);
	private:
		BColumnListView	*fProfileList;
		BMenuField		*fOSSelector;
		BMenuField		*fArchSelector;
		BMenuField		*fCompilerSelector;
		BPopUpMenu		*fOSMenu;
		BPopUpMenu		*fArchMenu;
		BPopUpMenu		*fCompilerMenu;
		BCheckBox		*fBuildForSize;
		BCheckBox		*fDebugSymbols;
		BCheckBox		*fProfileInfo;
		BTextControl	*fRunArgs;
		BTextControl	*fPreScript;
		BTextControl	*fPostScript;
		BTabView		*fFilesTabView;
		BTab			*fLocalTab;
		BTab			*fSysTab;
		BTab			*fLibsTab;
		BColumnListView *fLibsListView;
		BColumnListView	*fLocalIncludesListView;
		BColumnListView	*fSysIncludesListView;
		BButton			*fNewProfileBtn;
		BButton			*fDeleteProfileBtn;
		BButton			*fAddLibBtn;
		BButton			*fDeleteLibBtn;
		BButton			*fAddLocalBtn;
		BButton			*fDeleteLocalBtn;
		BButton			*fAddSysBtn;
		BButton			*fDeleteSysBtn;

		string	fAvailableCompilers[AVAILABLECOMPILERCOUNT] = {"gcc"};
		string	fAvailableArchitectures[AVAILABLEARCHCOUNT] = {"gcc2", "gcc5", "x86_64"};
		string	fAvailableOSes[AVAILABLEOSCOUNT] = {"Haiku"};

		vector<BuildProfile*>	fProfiles;

		void			_ProfileChanged();
};

#endif
