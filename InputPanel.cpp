#include "InputPanel.h"

InputPanel::InputPanel(BWindow* target, const char* Title, const char* Text, const char* Button1, const char* Button2, BMessage* message)
	:
	BWindow(BRect(200,200,600,300), "<Default>", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	fTitle = Title;
	fText = Text;
	fButton1Text = Button1;
	fButton2Text = Button2;
	fTarget = target;
	fMessage = message;

	SetTitle(Title);
	fControl = new BTextControl("textcontrol", fText, "", NULL);
	fButton1 = new BButton("button1", fButton1Text, new BMessage(BUTTON1_MSG));
	fButton2 = new BButton("button2", fButton2Text, new BMessage(BUTTON2_MSG));

	SetLayout(new BGroupLayout(B_HORIZONTAL));
	AddChild(BGroupLayoutBuilder(B_VERTICAL, 5.0)
		.AddStrut(5.0)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5.0)
			.AddStrut(5.0)
			.Add(fControl, 1.0)
			.AddStrut(5.0)
		)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5.0)
			.AddGlue()
			.Add(fButton1, 1.0)
			.Add(fButton2, 1.0)
			.AddStrut(5.0)
		)
		.AddStrut(5.0)
	);
}

InputPanel::~InputPanel()
{
}

void
InputPanel::MessageReceived(BMessage* msg)
{
	switch (msg->what)
	{
		case BUTTON1_MSG:
		{
			fMessage->AddString("button", fButton1Text);
			fMessage->AddString("value", fControl->Text());
			fMessage->AddPointer("window", this);
			fTarget->PostMessage(fMessage);
			PostMessage(new BMessage(B_QUIT_REQUESTED));
			break;
		}
		case BUTTON2_MSG:
		{
			fMessage->AddString("button", fButton2Text);
			fMessage->AddString("value", fControl->Text());
			fMessage->AddPointer("window", this);
			fTarget->PostMessage(fMessage);
			PostMessage(new BMessage(B_QUIT_REQUESTED));
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}
