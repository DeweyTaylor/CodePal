// (c) 2017 Dewey Taylor

#ifndef PROJECTCONTROLLER_H
#define PROJECTCONTROLLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <StorageKit.h>
#include <String.h>
#include <InterfaceKit.h>

using namespace std;

struct SourceFile
{
	string	Path;
	bool	Compile;
	string	Language;			// "C++", etc
	string	LanguageVersion;	// "C++11", etc
};

struct BuildProfile
{
	string			Name;
	string			fCompiler;		// "(Default)" (current), "gcc", "llvm", etc
	string			fTargetArch;	// "(Default)" (current), "gcc2", "x86_64" etc
	string			fTargetOS;		// "(Default)" (current), "Haiku", etc
	string			PreScript;		// script to run before build
	string			PostScript;		// script to run after build
	string			RunArgs;		// arguments to pass to the program
	bool			Debug;			// compile with debug symbols
	bool			Profile;		// compile with profiling data
	bool			Size;			// compile for size
	vector<string>	LocalIncludes;
	vector<string>	SysIncludes;
	vector<string>	Libs;
};

struct TargetGroup
{
						~TargetGroup();
	string				Name;
	bool				Expanded;
	vector<SourceFile*>	Sources;
};

struct CompileTarget
{
							~CompileTarget();
	string					TargetDir;
	string					Executable;
	string					Name;
	vector<TargetGroup*>	Groups;
	bool					fFileChanged; // file was edited, target is invalid
};

class ProjectController
{
	public:
								ProjectController();
								~ProjectController();

		int						Load(string);
		int						Load(BMessage*);
		int						Save();
		int						Close();
		int						SaveAs(string);
		int 					Build();
		int 					AddFile(string);
		int 					RemoveFile(string);
		int						Import(string, string);	// project type, project path
		int						Export(string, string); // project type, project path
		string					GetName() { return fName; }
		vector<string>			GetBuildProfileList();
		vector<BuildProfile*>	GetBuildProfiles();
		void					SetBuildProfiles(vector<BuildProfile*>);
		vector<CompileTarget*>	GetTargetTree();
		string					SelectedProfile();
		void					SetSelectedProfile(string);

		string					ProjectName = "";
		string					ProjectDirectory;
	private:
		// settings
		vector<CompileTarget*>	fTargetList;
		vector<BuildProfile*>	fBuildProfileList;
		BPath					fPath;
		string					fName = "";
		bool					fChanged; // projects needs saving
		string					fSelectedProfile;
};

#endif
