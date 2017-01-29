// (c) 2017 Dewey Taylor

#include "ProjectController.h"

// Source File

// Target Group
TargetGroup::~TargetGroup()
{
	while (Sources.size() > 0)
	{
		SourceFile* temp = Sources.back();
		Sources.pop_back();
		delete temp;
	}
}

// Compile Target
CompileTarget::~CompileTarget()
{
	while (Groups.size() > 0)
	{
		TargetGroup* temp = Groups.back();
		Groups.pop_back();
		delete temp;
	}
}

// Project Controller

ProjectController::ProjectController()
{
}

ProjectController::~ProjectController()
{
	while(fTargetList.size() > 0)
	{
		CompileTarget* temp = fTargetList.back();
		fTargetList.pop_back();
		delete temp;
	}

	while (fBuildProfileList.size() > 0)
	{
		BuildProfile* temp = fBuildProfileList.back();
		fBuildProfileList.pop_back();
		delete temp;
	}
}

int
ProjectController::Load(string project_path)
{
	BEntry entry(project_path.c_str(), true);
	if (entry.InitCheck() != B_OK)
	{
		return B_NO_INIT;
	}

	if (entry.Exists() != true)
	{
		return B_ENTRY_NOT_FOUND;
	}

	entry.GetPath(&fPath);
	{
		char temp[B_FILE_NAME_LENGTH];
		entry.GetName(temp);
		fName = temp;
	}

	string line;
	ifstream file(fPath.Path());
	if (!file.is_open())
	{
		// error opening file
		return B_ERROR;
	}

	// load project
	if (!getline(file, line))
	{
		file.close();
		return B_ERROR;
	}
	// first line is version number
	if (line != "CodePal Proj v.1")
	{
		// invalid file type or file version number
		file.close();
		return B_ERROR;
	}

	CompileTarget* CurrentTarget = NULL;
	BuildProfile* CurrentProfile = NULL;
	string CurrentLanguage = "C++";
	string CurrentLanguageVersion = "C++11";
	TargetGroup*	CurrentGroup = NULL;

	while (getline(file,line))
	{
		if (line == "" || line.c_str()[0] == '#')
		{
			// blank line or comment
			continue;
		}

		if (line.find("NAME=") == 0)
		{
			ProjectName = line.substr(5);
		}
		else if (line.find("PDIR=") == 0)
		{
			ProjectDirectory = line.substr(5);
		}
		else if (line.find("TARGET=") == 0)
		{
			if (CurrentTarget != NULL)
			{
				fTargetList.push_back(CurrentTarget);
			}
			CurrentTarget = new CompileTarget();
			CurrentTarget->Name = line.substr(7);
		}
		else if (line.find("TDIR=") == 0)
		{
			if (CurrentTarget == NULL)
			{
				return B_ERROR;
			}
		}
		else if (line.find("EXEC=") == 0)
		{
			if (CurrentTarget == NULL)
			{
				return B_ERROR;
			}
		}
		else if (line.find("PROFILE=") == 0)
		{
			if (CurrentProfile != NULL)
			{
				fBuildProfileList.push_back(CurrentProfile);
			}
			CurrentProfile = new BuildProfile();
			CurrentProfile->Name = line.substr(8);
		}
		else if (line.find("ARCH=") == 0)
		{
			if (CurrentProfile == NULL)
			{
				return B_ERROR;
			}
			CurrentProfile->fTargetArch = line.substr(5);
		}
		else if (line.find("OS=") == 0)
		{
			if (CurrentProfile == NULL)
			{
				return B_ERROR;
			}
			CurrentProfile->fTargetOS = line.substr(3);
		}
		else if (line.find("COMPILER=") == 0)
		{
			if (CurrentProfile == NULL)
			{
				return B_ERROR;
			}
			CurrentProfile->fCompiler = line.substr(9);
		}
		else if (line.find("PRESCRIPT=") == 0)
		{
			if (CurrentProfile == NULL)
			{
				return B_ERROR;
			}
			CurrentProfile->PreScript = line.substr(10);
		}
		else if (line.find("POSTSCRIPT=") == 0)
		{
			if (CurrentProfile == NULL)
			{
				return B_ERROR;
			}
			CurrentProfile->PostScript = line.substr(11);
		}
		else if (line.find("RUNARGS=") == 0)
		{
			if (CurrentProfile == NULL)
			{
				return B_ERROR;
			}
			CurrentProfile->RunArgs = line.substr(8);
		}
		else if (line.find("DEBUG=") == 0)
		{
			if (CurrentProfile == NULL)
			{
				return B_ERROR;
			}
			if (line.substr(6) == "yes" || line.substr(6) == "YES")
			{
				CurrentProfile->Debug = true;
			}
			else
			{
				CurrentProfile->Debug = false;
			}
		}
		else if (line.find("PROFILER=") == 0)
		{
			if (CurrentProfile == NULL)
			{
				return B_ERROR;
			}
			if (line.substr(9) == "yes" || line.substr(9) == "YES")
			{
				CurrentProfile->Profile = true;
			}
			else
			{
				CurrentProfile->Profile = false;
			}
		}
		else if (line.find("OPSIZE=") == 0)
		{
			if (CurrentProfile == NULL)
			{
				return B_ERROR;
			}
			if (line.substr(7) == "yes" || line.substr(7) == "YES")
			{
				CurrentProfile->Size = true;
			}
			else
			{
				CurrentProfile->Size = false;
			}
		}
		else if (line.find("LOCALINCLUDE=") == 0)
		{
			if (CurrentProfile == NULL)
			{
				return B_ERROR;
			}
			CurrentProfile->LocalIncludes.push_back(line.substr(13));
		}
		else if (line.find("SYSINCLUDE=") == 0)
		{
			if (CurrentProfile == NULL)
			{
				return B_ERROR;
			}
			CurrentProfile->SysIncludes.push_back(line.substr(11));
		}
		else if (line.find("LIB=") == 0)
		{
			if (CurrentProfile == NULL)
			{
				return B_ERROR;
			}
			CurrentProfile->Libs.push_back(line.substr(4));
		}
		else if (line.find("GROUP=") == 0)
		{
			if (line.substr(6) == "" || CurrentTarget == NULL)
			{
				return B_ERROR;
			}
			CurrentGroup = new TargetGroup();
			CurrentTarget->Groups.push_back(CurrentGroup);
			CurrentGroup->Name = line.substr(6);
		}
		else if (line.find("EXPANDGROUP=") == 0)
		{
			if (CurrentGroup == NULL)
			{
				return B_ERROR;
			}
			if (line.substr(12) == "yes" || line.substr(12) == "YES")
			{
				CurrentGroup->Expanded = true;
			}
			else
			{
				CurrentGroup->Expanded = false;
			}
		}
		else if (line.find("COMPILE=") == 0)
		{
			if (CurrentGroup == NULL)
			{
				return B_ERROR;
			}
			SourceFile* newfile = new SourceFile();
			newfile->Compile = true;
			newfile->Language = CurrentLanguage;
			newfile->LanguageVersion = CurrentLanguageVersion;
			newfile->Path = line.substr(8);
			CurrentGroup->Sources.push_back(newfile);
		}
		else if (line.find("NOCOMPILE=") == 0)
		{
			if (CurrentGroup == NULL)
			{
				return B_ERROR;
			}
			SourceFile* newfile = new SourceFile();
			newfile->Compile = false;
			newfile->Language = CurrentLanguage;
			newfile->LanguageVersion = CurrentLanguageVersion;
			newfile->Path = line.substr(10);
			CurrentGroup->Sources.push_back(newfile);
		}
		else if (line.find("LANGUAGE=") == 0)
		{
			if (line.substr(9) == "")
			{
				return B_ERROR;
			}
			CurrentLanguage = line.substr(9);
		}
		else if (line.find("LANGVER=") == 0)
		{
			if (line.substr(8) == "")
			{
				return B_ERROR;
			}
			CurrentLanguageVersion = line.substr(8);
		}
		else
		{
			return B_ERROR;
		}
	}

	if (CurrentTarget == NULL)
	{
		return B_ERROR;
	}
	if (CurrentTarget != NULL)
	{
		fTargetList.push_back(CurrentTarget);
	}
	if (CurrentProfile != NULL)
	{
		fBuildProfileList.push_back(CurrentProfile);
	}
	file.close();
	fChanged = false;
	return B_OK;
}

int ProjectController::Load(BMessage *msg)
{
	if (!msg)
	{
		return B_ERROR;
	}

	entry_ref dirRef;
	if(msg->FindRef("refs", &dirRef) != B_OK)
	{
		return B_BAD_VALUE;
	}

	BEntry entry(&dirRef, true);

	BPath path;
	entry.GetPath(&path);
	return Load(path.Path());
}

int
ProjectController::Save()
{
	// TODO: save
	fChanged = false;
	return B_OK;
}

int
ProjectController::Close()
{
	if (fChanged)
	{
		// TODO: save
	}
	// TODO: destruct
	return B_OK;
}

int
ProjectController::SaveAs(string new_project_path)
{
	return B_OK;
}

int
ProjectController::Build()
{
	return B_OK;
}

int
ProjectController::AddFile(string file_path)
{
	return B_OK;
}

int
ProjectController::RemoveFile(string file_path)
{
	return B_OK;
}

int
ProjectController::Import(string project_type, string project_path)
{
	return B_OK;
}

int
ProjectController::Export(string project_type, string project_path)
{
	return B_OK;
}

vector<string>
ProjectController::GetBuildProfileList()
{
	vector<string> profilenames;
	for (uint a = 0; a < fBuildProfileList.size(); a++)
	{
		profilenames.push_back(fBuildProfileList[a]->Name);
	}
	return profilenames;
}

vector<CompileTarget*>
ProjectController::GetTargetTree()
{
	return fTargetList;
}

/*vector<string>
ProjectController::GetTargetList()
{
	vector<string> targets;
	for (uint a = 0; a < fTargetList.size(); a++)
	{
		targets.push_back(fTargetList[a]->Name);
	}
	return targets;
}

vector<string>
ProjectController::GetGroupsForTarget(string target)
{
	vector<string> groups;
	for (uint a = 0; a < fTargetList.size(); a++)
	{
		if (fTargetList[a]->Name == target)
		{
			for (uint b = 0; b < fTargetList[a]->Groups.size(); b++)
			{
				groups.push_back(fTargetList[a]->Groups[b]->Name);
			}
		}
	}
	return groups;
}*/
