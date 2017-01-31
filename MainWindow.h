// (c) 2017 Dewey Taylor

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <MenuBar.h>
#include <InterfaceKit.h>
#include <GroupLayout.h>
#include <SplitLayoutBuilder.h>
#include <vector>
#include <ColumnListView.h>
#include <ColumnTypes.h>

#include "ProjectController.h"
#include "CLVHelpers.h"
#include "EditBuildProfilesWindow.h"

#define NEWPROJECT_MSG			'nprj'
#define OPENPROJECT_MSG			'oprj'
#define EXIT_MSG				'exit'
#define UPDATEBUILDPROFILE_MSG	'ubpr'
#define EDITBUILDPROFILES_MSG	'ebpr'


class MainWindow : public BWindow
{
	public:
							MainWindow(void);
		void				MessageReceived(BMessage *msg);
		bool				QuitRequested(void);

		vector<BuildProfile*>	_fHiddenNewBuildProfiles;

	private:
		BMenuBar			*fMenuBar;
		BMenu				*fProjectMenu;
		BMenu				*fSettingsMenu;

		BTabView			*fProjectTabView;
		BTabView			*fDebugTabView;
		BTabView			*fOutputTabView;
		BTabView			*fLeftTabView;
		BTabView			*fRightTabView;
		BView				*fToolBar;
		BMenuField			*fBuildProfileSelector;
		BPopUpMenu			*fBuildProfileMenu;
		BColumnListView		*fProjectItemsView;

		ProjectController	*fProject;

		void				_PopulateProjectTab(void);
		void				_PopulateBuildProfileMenu(void);
};

#endif
