// (c) 2017 Dewey Taylor

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <MenuBar.h>

#include "MainView.h"
#include "ProjectController.h"

#define NEWPROJECT_MSG		'nprj'
#define OPENPROJECT_MSG		'oprj'
#define EXIT_MSG			'exit'


class MainWindow : public BWindow
{
	public:
					MainWindow(void);
		void		MessageReceived(BMessage *msg);
		bool		QuitRequested(void);
			
	private:
		BMenuBar	*fMenuBar;
		BMenu		*fProjectMenu;
		BMenu		*fSettingsMenu;

		BTabView	*fProjectTabView;
		BTabView	*fDebugTabView;
		BTabView	*fOutputTabView;
		BTabView	*fLeftTabView;
		BTabView	*fRightTabView;

		MainView	*fMainView;
};

#endif
