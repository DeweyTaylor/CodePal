// (c) 2017 Dewey Taylor

#include "MainView.h"

MainView::MainView (BRect frame, char *name, BWindow* mw)
	: BView(frame, name, B_FOLLOW_ALL, B_WILL_DRAW)
{
	fProjectView = new ProjectView(BRect(0,0,1,1), "project view");

	SetLayout(new BGroupLayout(B_HORIZONTAL));
	AddChild(BSplitLayoutBuilder(B_VERTICAL, 10)
		.Add(fProjectView, 1.0)
		.Add(new BView(BRect(0,0,1,1), "test", B_FOLLOW_ALL, B_WILL_DRAW), 1.0)
	);
}

MainView::~MainView()
{
}
