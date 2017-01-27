// (c) 2017 Dewey Taylor

#include "ProjectController.h"

// Source File


// Source Target


// Project Controller

ProjectController::ProjectController()
{
}

ProjectController::~ProjectController()
{
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
	entry.GetName(fName);

	//printf("Path: %s\n", fPath.Path());
	//printf("Leaf: %s\n", fPath.Leaf());
	// TODO: load project
	return B_OK;
}

int
ProjectController::Save()
{
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
