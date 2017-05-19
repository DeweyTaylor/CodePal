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
#include "InputPanel.h"

#define NEWPROJECT_MSG				'nprj'
#define NEWPROJECT2_MSG				'npr2'
#define NEWPROJECT3_MSG				'npr3'
#define OPENPROJECT_MSG				'oprj'
#define EXIT_MSG					'exit'
#define UPDATEBUILDPROFILE_MSG		'ubpr'
#define EDITBUILDPROFILES_MSG		'ebpr'
#define PROJECTLISTSELECTION_MSG	'plsm'

#define NEWTARGET_MSG				'nwtg'
#define RENAMETARGET_MSG			'rntg'
#define REMOVETARGET_MSG			'rmtg'
#define ADDGROUP_MSG				'adgr'
#define RENAMEGROUP_MSG				'rngr'
#define REMOVEGROUP_MSG				'rmgr'
#define NEWFILE_MSG					'nwfl'
#define ADDFILE_MSG					'adfl'
#define RENAMEFILE_MSG				'rnfl'
#define REMOVEFILE_MSG				'rmfl'
#define PROPERTIES_MSG				'prop'
#define NEWTARGET2_MSG				'ntg2'

struct WindowRef
{
	MainWindow* Window;
	WindowRef*	Next;
};

class MainWindow : public BWindow
{
	public:
							MainWindow(void);
		void				MessageReceived(BMessage *msg);
		bool				QuitRequested(void);
		BPath				GetProjectPath();

		vector<BuildProfile*>	_fHiddenNewBuildProfiles;

	private:
		BMenuBar*			fMenuBar;
		BMenu*				fProjectMenu;
		BMenu*				fSettingsMenu;

		BTabView*			fProjectTabView;
		BTabView*			fDebugTabView;
		BTabView*			fOutputTabView;
		BTabView*			fLeftTabView;
		BTabView*			fRightTabView;
		BTabView*			fPropertiesTabView;
		BView*				fToolBar;
		BMenuField*			fBuildProfileSelector;
		BPopUpMenu*			fBuildProfileMenu;
		BColumnListView*	fProjectItemsView;
		BPopUpMenu*			fProjectContextMenu;

		ProjectController*	fProject;

		void				_PopulateProjectTab(void);
		void				_PopulateBuildProfileMenu(void);

		BMenuItem*			fNewTargetMenuItem = new BMenuItem("New Target...", new BMessage(NEWTARGET_MSG));
		//BMenuItem*			fRenameTargetMenuItem = new BMenuItem("Rename Target...", new BMessage(RENAMETARGET_MSG));
		BMenuItem*			fRemoveTargetMenuItem = new BMenuItem("Remove Target", new BMessage(REMOVETARGET_MSG));
		BMenuItem*			fAddGroupMenuItem = new BMenuItem("Add Group...", new BMessage(ADDGROUP_MSG));
		//BMenuItem*			fRenameGroupMenuItem = new BMenuItem("Rename Group...", new BMessage(RENAMEGROUP_MSG));
		BMenuItem*			fRemoveGroupMenuItem = new BMenuItem("Remove Group", new BMessage(REMOVEGROUP_MSG));
		BMenuItem*			fNewFileMenuItem = new BMenuItem("New File...", new BMessage(NEWFILE_MSG));
		BMenuItem*			fAddFileMenuItem = new BMenuItem("Add File...", new BMessage(ADDFILE_MSG));
		//BMenuItem*			fRenameFileMenuItem = new BMenuItem("Rename File...", new BMessage(RENAMEFILE_MSG));
		BMenuItem*			fRemoveFileMenuItem = new BMenuItem("Remove File", new BMessage(REMOVEFILE_MSG));
		//BMenuItem*			fPropertiesMenuItem = new BMenuItem("Properties...", new BMessage(PROPERTIES_MSG));
};

#endif
