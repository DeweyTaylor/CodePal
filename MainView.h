// (c) 2017 Dewey Taylor

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <InterfaceKit.h>
#include <GroupLayout.h>
#include <SplitLayoutBuilder.h>

#include "ProjectView.h"

class MainView : public BView
{
	public:
		MainView(BRect frame, char *name, BWindow* mw);
		~MainView();
	private:
		ProjectView	*fProjectView;
};

#endif
