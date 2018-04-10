#ifndef _MAINDLG_CPP
#define _MAINDLG_CPP
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <Stdio.H>
#include <Stdlib.H>

#include "../Resources/Resource.H"

#include "../../@Libraries/NSWFL/NSWFL.H"

#include "../Source/Entry.H"
#include "../Source/GetInfo.H"

#include "../Dialogs/MainDlg.H"
#include "../Dialogs/NetLogoDlg.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace NSWFL::Windows;
using namespace NSWFL::ListBox;
using namespace NSWFL::File;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------(Variable Declarations)
MAINDIALOGINFO MDI;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool PopOptionList(HWND OptionList_hWnd)
{
	for(int iOpt = 0; sOptions[iOpt]; iOpt++)
	{
		InsertListBoxItem(OptionList_hWnd, (char *)sOptions[iOpt], -1);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK OptionListProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = CallWindowProc(MDI.gpOldOptionListProc, hWnd, uMsg, wParam, lParam);
	
	if(uMsg == WM_LBUTTONDOWN)
    {
		char sOption[255];
		memset(sOption, 0, sizeof(sOption));

		if(GetListBoxItemText(sOption, sizeof(sOption), hWnd) > 0)
		{
			if(_strcmpi(sOption, MDI.sLastOption) != 0)
			{
				PopInfoList(false, MDI.InfoList_hWnd, sOption);
				
				strcpy_s(MDI.sLastOption, sizeof(MDI.sLastOption), sOption);
			}
		}
    }

    return lResult;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK MainDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int iLastSelection = -1;

	//--------------------------------------------------------------------------

    if(uMsg == WM_INITDIALOG) // Received A Initilize Dialog Message
    {
        memset(&MDI, 0, sizeof(MDI));
		
		MDI.hWnd = hWnd;

        MDI.SystemMenu_hMenu = GetSystemMenu(hWnd, FALSE);
        AppendMenu(MDI.SystemMenu_hMenu, MF_SEPARATOR, 0, 0);
        AppendMenu(MDI.SystemMenu_hMenu, MF_STRING, MAINDIALOG_MENU_ABOUT, "About");

        SendMessage(hWnd, (UINT)WM_SETTEXT, (WPARAM)0, (LPARAM)gsTitleCaption);
        SendMessage(hWnd, WM_SETICON, TRUE, (LPARAM) LoadIcon(ghAppInstance, MAKEINTRESOURCE(IDI_MAIN)));

        MDI.OptionList_hWnd = GetDlgItem(hWnd, IDC_OPTLIST);
        MDI.InfoList_hWnd   = GetDlgItem(hWnd, IDC_INFOLIST);

		PopOptionList(MDI.OptionList_hWnd);
		
        MDI.gpOldOptionListProc = (WNDPROC) SetWindowLong(MDI.OptionList_hWnd, GWL_WNDPROC, (long) &OptionListProc);

		SelectListBoxItem(MDI.OptionList_hWnd, 0);
		
		SendMessage(MDI.OptionList_hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
        SendMessage(MDI.InfoList_hWnd, LB_SETHORIZONTALEXTENT, (WPARAM)2500, (LPARAM)0);

		CenterWindow(hWnd);

        return TRUE; // Return TRUE to set the keyboard focus, Otherwise return FALSE.
    }

	//--------------------------------------------------------------------------

	else if(uMsg == WM_COMMAND)
    {
		if(wParam == IDC_REFRESH)
        {
			PopInfoList(false, MDI.InfoList_hWnd , MDI.sLastOption);
			return TRUE;
		}

		if(wParam == ID_FILE_SAVE_INFO)
        {
			char sTarget[MAX_PATH];
			char sStartPath[MAX_PATH];

			char *sFilters = "Text-Files\0*.txt\0All-Files\0\0";

			DWORD dwFlags = OFN_EXPLORER + OFN_LONGNAMES + OFN_PATHMUSTEXIST;

			if(Get_CurrentDirectory(sStartPath, sizeof(sStartPath)))
			{
				strcpy_s(sTarget, sizeof(sTarget), "");

				if(OSFileDialog(hWnd, sTarget, sizeof(sTarget),
					"txt", "Save file as?", sFilters, sStartPath, SFD, dwFlags))
				{
					if(FileAccess(sTarget, FExist))
					{
						if(MessageBox(hWnd, "File already exist. Overwrite?", gsTitleCaption, MB_YESNO | MB_ICONQUESTION) != IDYES)
						{
							return TRUE;
						}
					}
					if(SaveAllInfoToFile(sTarget))
					{
						MessageBox(hWnd, "Information saved!", gsTitleCaption, MB_ICONINFORMATION);
					}
					else{
						MessageBox(hWnd, "Failed to open file for write access!", gsTitleCaption, MB_ICONERROR);
					}
				}
			}
			return TRUE;
		}

		if(wParam == ID_FILE_PRINT_INFO)
        {
            return TRUE;
		}

		if(wParam == IDC_CLOSE || wParam == ID_FILE_CLOSE_APP)
        {
            EndDialog(hWnd, 0);
            DestroyWindow(hWnd);

			memset(&MDI, 0, sizeof(MDI));

            return TRUE;
        }

        if(wParam == ID_HELP_ABOUT)
        {
            _AboutDialogInfo ADI;
    
            ADI.DisplayIcon  = LoadIcon(ghAppInstance, MAKEINTRESOURCE(IDI_MAIN));
            ADI.TitleCaption = gsTitleCaption;
            ADI.FileVersion  = gsFileVersion;
            ADI.BuildDate    = __DATE__;
            ADI.BuildTime    = __TIME__;
            ADI.CopyRight    = gsAppCopyRight;
            ADI.OwnerHandle  = hWnd;

            NetLogo(&ADI);
            return TRUE;
        }
		return FALSE;
    }

    //--------------------------------------------------------------------------

	else if(uMsg == WM_PAINT)
    {
        HDC ThisHDC;
        PAINTSTRUCT ThisPS;

        ThisHDC = BeginPaint(hWnd, &ThisPS);

        //- Any painting should be done here.

        EndPaint(hWnd, &ThisPS);
        return TRUE;
    }

    //--------------------------------------------------------------------------

    else if(uMsg == WM_SYSCOMMAND) //- Received a system menu message.
    {
        if(LOWORD(wParam) == MAINDIALOG_MENU_ABOUT) //- About.
        {
            _AboutDialogInfo ADI;
    
            ADI.DisplayIcon  = LoadIcon(ghAppInstance, MAKEINTRESOURCE(IDI_MAIN));
            ADI.TitleCaption = gsTitleCaption;
            ADI.FileVersion  = gsFileVersion;
            ADI.BuildDate    = __DATE__;
            ADI.BuildTime    = __TIME__;
            ADI.CopyRight    = gsAppCopyRight;
            ADI.OwnerHandle  = hWnd;

            NetLogo(&ADI);
            return TRUE;
        }

        return FALSE;
    }

    //--------------------------------------------------------------------------

    else if(uMsg == WM_CLOSE) //- Received close message.
    {
        EndDialog(hWnd,0);
        DestroyWindow(hWnd);
		
		memset(&MDI, 0, sizeof(MDI));

        return TRUE;
    }

    //--------------------------------------------------------------------------

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

