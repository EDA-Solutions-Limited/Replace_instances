/*******************************************************************************
 * Macro Name: Replace_inst
 *
 * Revision History:
 * 25-Sept-2020: Added Dialog item boxes for input
 *				 Added chrono function to measure execution time vs the same
				 script written in tcl
 *******************************************************************************/
#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cmath>
#include <vector>
#include <chrono>

#include <string>

#define EXCLUDE_LEDIT_LEGACY_UPI

#include "ldata.h"
/* Begin -- Uncomment this block if you are using L-Comp. */
#include "lcomp.h"
/* End */

extern "C"
{
	void replace_instances(void);
	int UPI_Entry_Point(void);
	void Macro2(void);
}
/********************************************************************************************
function that searches all instances of the visible cell and finds the required instances that
match the specified cells to be replaced
********************************************************************************************/
std::vector<LInstance> phLEditFindInstances(LCell pCell, char library[MAX_CELL_NAME],
											char cellname[MAX_CELL_NAME], char viewname[MAX_CELL_NAME])
{
	std::vector<LInstance> instances;
	LCell mCell,gCell = NULL;
	for (LInstance pInstance = LInstance_GetList(pCell); Assigned(pInstance); pInstance = LInstance_GetNext(pInstance))
	{
		//get cell generator to cover instances with generator parent cells i.e. precompiled T-Cells
		gCell = LCell_GetGenerator(LInstance_GetCell(pInstance)); 
		if (gCell != NULL){
			mCell = gCell;
		}
		//Regular instances with non-generator parent cells
		else{
			mCell = LInstance_GetCell(pInstance);
		}		
		char lib[MAX_CELL_NAME] = "";
		char view[MAX_CELL_NAME] = "";
		char cell[MAX_CELL_NAME] = "";
		//get cell details and sieve out the required cells
		LCell_GetLibName(mCell, lib, MAX_CELL_NAME);
		LCell_GetViewName(mCell, view, MAX_CELL_NAME);
		LCell_GetName(mCell, cell, MAX_CELL_NAME);
		if (strcmp(lib, library) == 0 && strcmp(cellname, cell) == 0 && strcmp(viewname, view) == 0)
		{
			instances.push_back(pInstance);
		}
	}
	return instances;
}

void phLEditReplaceInstWithNewCell(LCell pCell, LInstance inst, char *newlib, char *newcell, char *newview)
{
	LFile pFile = LCell_GetFile(pCell);
	char name[MAX_CELL_NAME] = "";
	LTransform_Ex99 transform = LInstance_GetTransform_Ex99(inst);
	LCell new_cell = LCell_FindEx2(pFile, newcell, newview, newlib);
	// if new cell is found and created
	if (new_cell != NULL)
	{
		LInstance_GetName(inst, name, MAX_CELL_NAME);
		LInstance_Delete(pCell, inst); // delete old instance 
		LInstance inst_new = LInstance_New_Ex99(pCell, new_cell, transform, {1, 1}, {0, 0}); // place new instance in the place of the old instance
		LInstance_SetName(pCell, inst_new, name);
	}
	else
	{
		LUpi_LogMessage(LFormat("Unable to find %s\\%s\\%s  \n", newlib, newview, newcell));
	}
}
void phLEditReplaceAllInstOfCell(char oldlib[MAX_CELL_NAME], char oldcell[MAX_CELL_NAME],
								 char oldview[MAX_CELL_NAME], char newlib[MAX_CELL_NAME],
								 char newcell[MAX_CELL_NAME], char newview[MAX_CELL_NAME])
{
	LCell pCell = LCell_GetVisible();
	std::vector<LInstance> inst_list = phLEditFindInstances(pCell, oldlib, oldcell, oldview);
	LUpi_LogMessage(LFormat("instances found:%d \n", inst_list.size()));
	if (inst_list.size() > 0)
	{
		int count{0}, completion{0}, completion_old{0};
		for (auto inst : inst_list)
		{
			phLEditReplaceInstWithNewCell(pCell, inst, newlib, newcell, newview);
			++count;
			// counter to keep track of the completion percentages, useful when replacing a huge amount of instances
			if (inst_list.size() > 10)
			{
				completion = 100 * (float)count / inst_list.size();
				if (completion > completion_old)
				{
					LUpi_LogMessage(LFormat("%d%%  replaced \n", completion));
					completion_old = completion + 5;
				}
			}
		}
		LDisplay_Refresh();
		LUpi_LogMessage("Finished \n");
	}
	else
	{
		LUpi_LogMessage(LFormat("Unable to find %s\\%s\\%s  \n", oldlib, oldview, oldcell));
	}
}

void replace_instances(void)
{
	char oldlib[MAX_CELL_NAME] = "";
	char oldcell[MAX_CELL_NAME] = "";
	char oldview[MAX_CELL_NAME] = "";
	char newlib[MAX_CELL_NAME] = "";
	char newcell[MAX_CELL_NAME] = "";
	char newview[MAX_CELL_NAME] = "";

	LDialogItem Dialog_Items[6] =
		{
			{"oldlib", "RingVCO"},
			{"oldcell", "DiffCell"},
			{"oldview", "layout"},
			{"newlib", "RingVCO"},
			{"newcell", "Control"},
			{"newview", "layout"},
		}; // display dialog to allow user input 

	if (LDialog_MultiLineInputBox("Replace instances", Dialog_Items, 6))
	{
		strcpy(oldlib, Dialog_Items[0].value);
		strcpy(oldcell, Dialog_Items[1].value);
		strcpy(oldview, Dialog_Items[2].value);
		strcpy(newlib, Dialog_Items[3].value);
		strcpy(newcell, Dialog_Items[4].value);
		strcpy(newview, Dialog_Items[5].value);

		auto t1 = std::chrono::high_resolution_clock::now();
		phLEditReplaceAllInstOfCell(oldlib, oldcell, oldview, newlib, newcell, newview);
		auto t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
		LUpi_LogMessage(LFormat("Execution time:%d seconds \n", duration));
		LDisplay_Refresh();
	}
	else
	{
		LDialog_MsgBox("No input, exiting  \n");
	}
}

void Macro2 (void){
	LCell pCell = LCell_GetVisible();
	LFile pFile = LCell_GetFile(pCell);

	LDialog_MsgBox("Hello, from Replace instances!");
}

int UPI_Entry_Point(void)
{
	LMacro_BindToMenuAndHotKey_v9_30("Tools", NULL /*hotkey*/,
									 "replace_instances", "replace_instances", NULL /*hotkey category*/);
	return 1;
}
