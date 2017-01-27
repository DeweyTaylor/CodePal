// (c) 2017 Dewey Taylor

#ifndef PROJECTCONTROLLER_H
#define PROJECTCONTROLLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <StorageKit.h>

using namespace std;

class SourceFile
{
	public:
	private:
		string	fLanguage;			// "C++", etc
		string	fLanguageVersion;	// "C++11", etc
};

class SourceTarget
{
	public:
	private:
		list<SourceFile>	fSourceList;
		string				fCompiler;
		string				fTargetArch;	// "(Default)" (current), "gcc2", "x86_64" etc
		string				fTargetOS;		// "(Default)" (current), "Haiku", etc
};

class ProjectController
{
	public:
							ProjectController();
							~ProjectController();

		int					Load(string);
		int					Save();
		int					SaveAs(string);
		int 				Build();
		int 				AddFile(string);
		int 				RemoveFile(string);
		int					Import(string, string);	// project type, project path
		int					Export(string, string); // project type, project path
	private:
		// settings
		list<SourceTarget>	fTargetList;
		BPath				fPath;
		char				fName[B_FILE_NAME_LENGTH];
};

#endif
