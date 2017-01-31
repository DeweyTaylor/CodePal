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
			CurrentTarget = new CompileTarget();
			fTargetList.push_back(CurrentTarget);
			CurrentGroup = NULL;
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
			CurrentProfile = new BuildProfile();
			fBuildProfileList.push_back(CurrentProfile);
			CurrentProfile->Name = line.substr(8);
		}
		else if (line.find("DEFAULTPROFILE=") == 0)
		{
			fSelectedProfile = line.substr(15);
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
	file.close();
	// TEMPORARY FOR DEBUGGING
	for (uint a = 0; a < fTargetList.size(); a++)
	{
		cout<<"TARGET -> '" << fTargetList[a]->Name << "'" << endl;
		for (uint b = 0; b < fTargetList[a]->Groups.size(); b++)
		{
			cout << "  GROUP -> '" << fTargetList[a]->Groups[b]->Name << "'" << endl;
			for (uint c = 0; c < fTargetList[a]->Groups[b]->Sources.size(); c++)
			{
				cout <<"    FILE -> '" << fTargetList[a]->Groups[b]->Sources[c]->Path << "'" << endl;
			}
		}
	}
	// END TEMPORARY
	fChanged = false;
	// update path and name
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
	ofstream file;
	//cout << "path: " << fPath.Path() + fName << endl;
	//return 0;
	//file.open (ProjectDirectory + ProjectName);
	file.open(fPath.Path());
	file << "CodePal Proj v.1" << endl;
	file << "NAME=" << ProjectName << endl;
	file << "PDIR=" << ProjectDirectory << endl;
	for (uint a = 0; a < fBuildProfileList.size(); a++)
	{
		BuildProfile *bp = fBuildProfileList[a];
		file << "PROFILE=" << bp->Name << endl;
		file << "ARCH=" << bp->fTargetArch << endl;
		file << "OS=" << bp->fTargetOS << endl;
		file << "COMPILER=" << bp->fCompiler << endl;
		file << "PRESCRIPT=" << bp->PreScript << endl;
		file << "POSTSCRIPT=" << bp->PostScript << endl;
		file << "RUNARGS=" << bp->RunArgs << endl;
		file << "DEBUG=" << (bp->Debug ? "yes" : "no") << endl;
		file << "PROFILER=" << (bp->Profile ? "yes" : "no") << endl;
		file << "OPSIZE=" << (bp->Size ? "yes" : "no") << endl;
		for (uint b = 0; b < bp->LocalIncludes.size(); b++)
		{
			file << "LOCALINCLUDE=" << bp->LocalIncludes[b] << endl;
		}
		for (uint b = 0; b < bp->SysIncludes.size(); b++)
		{
			file << "SYSINCLUDE=" << bp->SysIncludes[b] << endl;
		}
		for (uint b = 0; b < bp->Libs.size(); b++)
		{
			file << "LIB=" << bp->Libs[b] << endl;
		}
	}
	for (uint a = 0; a < fTargetList.size(); a++)
	{
		CompileTarget *ct = fTargetList[a];
		file << "TARGET=" << ct->Name << endl;
		file << "TDIR=" << ct->TargetDir << endl;
		file << "EXEC=" << ct->Executable << endl;
		for (uint b = 0; b < ct->Groups.size(); b++)
		{
			TargetGroup *tg = ct->Groups[b];
			file << "GROUP=" << tg->Name;
			file << "EXPANDGROUP=" << (tg->Expanded ? "yes" : "no") << endl;
			string lang = "";
			string langver = "";
			for (uint c = 0; c < tg->Sources.size(); c++)
			{
				SourceFile *s = tg->Sources[c];
				if (s->Language != lang)
				{
					lang = s->Language;
					file << "LANGUAGE=" << lang << endl;
				}
				if (s->LanguageVersion != langver)
				{
					langver = s->LanguageVersion;
					file << "LANGVER=" << langver << endl;
				}
				file << (s->Compile ? "COMPILE=" : "NOCOMPILE=") << s->Path << endl;
			}
		}
	}
	file.close();
	fChanged = false;
	return B_OK;
}

int
ProjectController::Close()
{
	cout << "changed: " << fChanged << endl;
	if (fChanged)
	{
		// could ask first
		Save();
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

vector<BuildProfile*>
ProjectController::GetBuildProfiles()
{
	return fBuildProfileList;
}

void
ProjectController::SetBuildProfiles(vector<BuildProfile*> profiles)
{
	fChanged = true;
	fBuildProfileList = profiles;
}

vector<CompileTarget*>
ProjectController::GetTargetTree()
{
	return fTargetList;
}

string
ProjectController::SelectedProfile()
{
	return fSelectedProfile;
}

void
ProjectController::SetSelectedProfile(string prof)
{
	// TODO: validate that the profile is valid
	fSelectedProfile = prof;
}
