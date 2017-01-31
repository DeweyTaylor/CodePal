// (c) 2017 Dewey Taylor

#ifndef INPUTPANEL_H
#define INPUTPANEL_H

#include <InterfaceKit.h>
#include <GroupLayoutBuilder.h>

#define BUTTON1_MSG		'btn1'
#define BUTTON2_MSG		'btn2'
#define INPUTPANEL_MSG	'inpm'

class InputPanel : public BWindow
{
	public:
						InputPanel(BWindow*, const char*, const char*, const char* = 0, const char* = 0, BMessage* = NULL);
						~InputPanel();
		void			MessageReceived(BMessage*);
	private:
		const char*		fTitle;
		const char*		fText;
		const char*		fButton1Text;
		const char*		fButton2Text;
		BTextControl*	fControl;
		BButton*		fButton1;
		BButton*		fButton2;
		BWindow*		fTarget;
		BMessage*		fMessage;
};

#endif
