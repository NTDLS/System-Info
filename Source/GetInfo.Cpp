#ifndef _GetInfo_CPP
#define _GetInfo_CPP
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <WindowsX.H>
#include <WinSock2.H>
#include <WinSock.H>
#include <ShellAPI.H>
#include <Stdio.H>
#include <Stdlib.H>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../Resources/Resource.H"

#include "../../@Libraries/NSWFL/NSWFL.H"

#include "Entry.H"
#include "Init.H"
#include "GetInfo.H"

#include "../Dialogs/MainDlg.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace NSWFL::Windows;
using namespace NSWFL::ListBox;
using namespace NSWFL::File;
using namespace NSWFL::Registry;
using namespace NSWFL::System;
using namespace NSWFL::String;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MMX_SUPPORTED			0x00800000
#define SSE_SUPPORTED			0x02000000
#define SSE2_SUPPORTED			0x04000000
#define AMD_3DNOW_SUPPORTED		0x80000000

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FILE *hTarget = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char *sOptions[] = {
	"General",
	"BIOS",
	"Socket",
	"Environment",
	"Disk",
	"Miscellaneous",
	NULL
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WriteText(HWND hWnd, char *sText, bool bWriteToFile)
{
	if(bWriteToFile)
	{
		fprintf(hTarget, "%s\r\n", sText);
	}
	else{
		InsertListBoxItem(hWnd, sText, -1);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SaveAllInfoToFile(char *sTarget)
{
	if(fopen_s(&hTarget, sTarget, "wb") != 0)
	{
		return false;
	}

	strcpy_s(MDI.sLastOption, sizeof(MDI.sLastOption), "");

	fprintf(hTarget, SEPERATOR);

	for(int iOpt = 0; sOptions[iOpt]; iOpt++)
	{
		PopInfoList(true, MDI.InfoList_hWnd, (char *)sOptions[iOpt]);
		fprintf(hTarget, SEPERATOR);
	}
	
	fclose(hTarget);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool PopInfoList(bool bWriteToFile, HWND hWnd, char *sOption)
{
	char sText[255];

	if(!bWriteToFile)
	{
		ClearListBox(hWnd);
	}
	else{
		sprintf_s(sText, sizeof(sText), "[%s Information]", sOption);
		WriteText(hWnd, sText, bWriteToFile);
		WriteText(hWnd, "", bWriteToFile);
	}

	if(_strcmpi(sOption, "General") == 0)
	{
		WriteOsVersion(bWriteToFile, hWnd);
		WriteText(hWnd, "", bWriteToFile);

		WriteProcessorInformation(bWriteToFile, hWnd);
		WriteText(hWnd, "", bWriteToFile);

		WriteNetWorkInformation(bWriteToFile, hWnd);
		WriteText(hWnd, "", bWriteToFile);

		WriteMemoryStatus(bWriteToFile, hWnd);
		WriteText(hWnd, "", bWriteToFile);
	}
	else if(_strcmpi(sOption, "BIOS") == 0)
	{
		WriteBIOSInfo(bWriteToFile, hWnd);
		WriteText(hWnd, "", bWriteToFile);
	}
	else if(_strcmpi(sOption, "Socket") == 0)
	{
		PrintWSAData(bWriteToFile, hWnd);
		WriteText(hWnd, "", bWriteToFile);
	}
	else if(_strcmpi(sOption, "Environment") == 0)
	{
		WriteBootType(bWriteToFile, hWnd);
		WriteText(hWnd, "", bWriteToFile);
		WriteEnvironmentInfo(bWriteToFile, hWnd);
		WriteText(hWnd, "", bWriteToFile);
	}
	else if(_strcmpi(sOption, "Disk") == 0)
	{
		Write_DiskInformation(bWriteToFile, hWnd);
		WriteText(hWnd, "", bWriteToFile);
	}
	else if(_strcmpi(sOption, "Miscellaneous") == 0)
	{
		WriteDisplayInformation(bWriteToFile, hWnd);
		WriteText(hWnd, "", bWriteToFile);
		WriteMouseInformation(bWriteToFile, hWnd);
		WriteText(hWnd, "", bWriteToFile);
	}

	if(!bWriteToFile)
	{
		SendMessage(hWnd, (UINT) LB_SETANCHORINDEX , 0, TRUE);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool WriteMouseInformation(bool bWriteToFile, HWND hWnd)
{
    char sTemp[255];
    char *sOption = NULL;

    //--------------------------------------------------------------------

	WriteText(hWnd, "- Mouse Information : ", bWriteToFile);
    if(GetSystemMetrics(SM_MOUSEPRESENT) == 0)
        sOption = "False";
    else sOption = "True";

	sprintf_s(sTemp, sizeof(sTemp), "\t Mouse Present : %s",sOption);
    WriteText(hWnd, sTemp, bWriteToFile);

    //--------------------------------------------------------------------

    sprintf_s(sTemp, sizeof(sTemp), "\t Number Of Mouse Buttons: %d ",GetSystemMetrics(SM_CMOUSEBUTTONS));
    WriteText(hWnd, sTemp, bWriteToFile);

    //--------------------------------------------------------------------

    if(GetSystemMetrics(SM_SWAPBUTTON) == 0)
        sOption = "False";
    else sOption = "True";

	sprintf_s(sTemp, sizeof(sTemp), "\t Mouse Buttons Swapped : %s", sOption);
    WriteText(hWnd, sTemp, bWriteToFile);

    //--------------------------------------------------------------------

    if(GetSystemMetrics(SM_MOUSEWHEELPRESENT) == 0)
        sOption = "False";
    else sOption = "True";

	sprintf_s(sTemp, sizeof(sTemp), "\t Mouse Wheel Present :  %s", sOption);
    WriteText(hWnd, sTemp, bWriteToFile);

	WriteText(hWnd, "", bWriteToFile);
    
	WriteText(hWnd, "- Mouse Metrics : ", bWriteToFile);
    //--------------------------------------------------------------------
    sprintf_s(sTemp, sizeof(sTemp), "\t Double Click Square : %d X %d ",
		GetSystemMetrics(SM_CXDOUBLECLK), GetSystemMetrics(SM_CYDOUBLECLK));

    WriteText(hWnd, sTemp, bWriteToFile);

    //--------------------------------------------------------------------
    sprintf_s(sTemp, sizeof(sTemp), "\t Begin Drag Square : %d X %d ",
		GetSystemMetrics(SM_CXDRAG), GetSystemMetrics(SM_CYDRAG));

    WriteText(hWnd, sTemp, bWriteToFile);
    //--------------------------------------------------------------------
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool WriteDisplayInformation(bool bWriteToFile, HWND hWnd)
{
    char sTemp[255];

    //--------------------------------------------------------------------

	WriteText(hWnd, "- Display Information : ", bWriteToFile);

    //--------------------------------------------------------------------

	sprintf_s(sTemp, sizeof(sTemp),"\t Full Screen Size : %d X %d ",
		GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    WriteText(hWnd, sTemp, bWriteToFile);

    //--------------------------------------------------------------------

    sprintf_s(sTemp, sizeof(sTemp),"\t Viewable Desktop Size : %d X %d ",
		GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));
    WriteText(hWnd, sTemp, bWriteToFile);

    //--------------------------------------------------------------------

    sprintf_s(sTemp, sizeof(sTemp),"\t Minimum Window Size : %d X %d ",
		GetSystemMetrics(SM_CXMIN), GetSystemMetrics(SM_CYMIN));
    WriteText(hWnd, sTemp, bWriteToFile);

    //--------------------------------------------------------------------

    sprintf_s(sTemp, sizeof(sTemp),"\t Icon Spacing Grid : %d X %d ",
		GetSystemMetrics(SM_CXICONSPACING),GetSystemMetrics(SM_CYICONSPACING));
    WriteText(hWnd, sTemp, bWriteToFile);

	//--------------------------------------------------------------------

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Write_DiskInformation(bool bWriteToFile, HWND hWnd)
{
    int iPos = 65; //Drive S
	int iDriveType = 0;
    char sDrive[5];
    char sText[1024];

	while(iPos <= 90)
    {
        sprintf_s(sDrive, sizeof(sDrive), "%c:\\", iPos);

		if((iDriveType = GetDriveType(sDrive)) != DRIVE_NO_ROOT_DIR)
		{
			sprintf_s(sText, sizeof(sText), "-Drive: %s", sDrive);
			
			if(iDriveType == DRIVE_UNKNOWN) {
				strcat_s(sText, sizeof(sText), " (The drive type cannot be determined)");
			}
			else if(iDriveType == DRIVE_REMOVABLE) {
				strcat_s(sText, sizeof(sText), " (Removable Media)");
			}
			else if(iDriveType == DRIVE_FIXED) {
				strcat_s(sText, sizeof(sText), " (Fixed Disk)");
			}
			else if(iDriveType == DRIVE_REMOTE) {
				strcat_s(sText, sizeof(sText), " (Network Drive)");
			}
			else if(iDriveType == DRIVE_CDROM) {
				strcat_s(sText, sizeof(sText), " (CD-ROM Drive)");
			}
			else if(iDriveType == DRIVE_RAMDISK) {
				strcat_s(sText, sizeof(sText), " (RAM disk)");
			}

			WriteText(hWnd, sText, bWriteToFile);

			if(iDriveType != DRIVE_UNKNOWN && iDriveType != DRIVE_REMOVABLE && iDriveType != DRIVE_CDROM)
			{
				WriteDriveInfo(bWriteToFile, hWnd, sDrive);
			}
		}

		iPos++;
    }

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool WriteDriveInfo(bool bWriteToFile, HWND hWnd, char *sDrive)
{
    char sTemp[1024];
	char sSize[64];
    char sSerial[240];
    LARGE_INTEGER a, b, c;

    char sVolumeNameBuffer[1024];
    DWORD dwVolumeNameSize = sizeof(sVolumeNameBuffer);
    DWORD dwVolumeSerialNumber;
    DWORD dwMaximumComponentLength;
    DWORD dwFileSystemFlags;
    char sFileSystemNameBuffer[1024];
    DWORD dwFileSystemNameSize = sizeof(sFileSystemNameBuffer);

    if(GetVolumeInformation(sDrive,
		sVolumeNameBuffer,
		dwVolumeNameSize,
		&dwVolumeSerialNumber,
		&dwMaximumComponentLength,
		&dwFileSystemFlags,
		sFileSystemNameBuffer,
		dwFileSystemNameSize))
    {
        WriteText(hWnd, "\t Misc Volume Info:", bWriteToFile);
        sprintf_s(sTemp, sizeof(sTemp),"\t\t Volume Name: %s",sVolumeNameBuffer);
        WriteText(hWnd, sTemp, bWriteToFile);
        sprintf_s(sTemp, sizeof(sTemp),"\t\t File System: %s",sFileSystemNameBuffer);
        WriteText(hWnd, sTemp, bWriteToFile);

        sprintf_s(sSerial, sizeof(sSerial), "%X",dwVolumeSerialNumber);

		sprintf_s(sTemp, sizeof(sTemp),"\t\t Serial Number : %c%c%c%c-%c%c%c%c ",
			sSerial[0],sSerial[1],sSerial[2],sSerial[3],
			sSerial[4],sSerial[5],sSerial[6],sSerial[7]);

        WriteText(hWnd, sTemp, bWriteToFile);

        sprintf_s(sTemp, sizeof(sTemp),"\t\t Max File Name Length: %d",dwMaximumComponentLength);
        WriteText(hWnd, sTemp, bWriteToFile);

        WriteText(hWnd, "", bWriteToFile);

        WriteText(hWnd, "\t Volume Flags:", bWriteToFile);
        if(dwFileSystemFlags & FS_CASE_IS_PRESERVED)
            WriteText(hWnd, "\t\t File Case Is Preserved.", bWriteToFile);
        if(dwFileSystemFlags & FS_CASE_SENSITIVE)
            WriteText(hWnd, "\t\t Supports Case Sensitive Names.", bWriteToFile);
        if(dwFileSystemFlags & FS_UNICODE_STORED_ON_DISK)
            WriteText(hWnd, "\t\t Supports Unicode In Filenames.", bWriteToFile);
        if(dwFileSystemFlags & FS_PERSISTENT_ACLS)
            WriteText(hWnd, "\t\t Security Is Preserved And Enforced.", bWriteToFile);
        if(dwFileSystemFlags & FS_FILE_COMPRESSION)
            WriteText(hWnd, "\t\t Supports File-Based Compression.", bWriteToFile);
        if(dwFileSystemFlags & FS_VOL_IS_COMPRESSED)
            WriteText(hWnd, "\t\t Volume Is Compressed.", bWriteToFile);
    }
    else WriteText(hWnd, "Volume Information Error", bWriteToFile);

    WriteText(hWnd, "", bWriteToFile);

    if(GetDiskFreeSpaceEx(sDrive,(ULARGE_INTEGER *)&a,(ULARGE_INTEGER *)&b,(ULARGE_INTEGER *)&c))
    {
        WriteText(hWnd, "\t Volume Size Info:", bWriteToFile);

		FileSizeFriendly(b.QuadPart, sSize, sizeof(sSize));
        sprintf_s(sTemp, sizeof(sTemp),"\t\t Bytes Total %s", sSize);
        WriteText(hWnd, sTemp, bWriteToFile);

		FileSizeFriendly(b.QuadPart - c.QuadPart, sSize, sizeof(sSize));
        sprintf_s(sTemp, sizeof(sTemp),"\t\t Bytes Used %s", sSize);
        WriteText(hWnd, sTemp, bWriteToFile);

		FileSizeFriendly(c.QuadPart, sSize, sizeof(sSize));
        sprintf_s(sTemp, sizeof(sTemp),"\t\t Bytes Free %s", sSize);
        WriteText(hWnd, sTemp, bWriteToFile);

		FileSizeFriendly(a.QuadPart, sSize, sizeof(sSize));
        sprintf_s(sTemp, sizeof(sTemp),"\t\t Bytes Quota %s", sSize);
        WriteText(hWnd, sTemp, bWriteToFile);
    }
    else WriteText(hWnd, "Disk Free Space Error", bWriteToFile);

    WriteText(hWnd, "", bWriteToFile);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool WriteEnvironmentInfo(bool bWriteToFile, HWND hWnd)
{
	char sVar[1024];
	char sTemp[1024];

    DWORD dwVersion;
    dwVersion = GetVersion();

	WriteText(hWnd, "- Command :", bWriteToFile);    

	GetEnvironmentVariable("COMSPEC", sVar, sizeof(sVar));
    if(strlen(sVar) > 3)
    {
        sprintf_s(sTemp, sizeof(sTemp), "\t Interpreter: %s ", sVar);
        WriteText(hWnd, sTemp, bWriteToFile);
    }

    if(dwVersion != 0)
    {
        sprintf_s(sTemp, sizeof(sTemp), "\t Version: %d.%d ", HIBYTE(HIWORD(dwVersion)), LOBYTE(HIWORD(dwVersion)));
        WriteText(hWnd, sTemp, bWriteToFile);
    }

	WriteText(hWnd, "", bWriteToFile);

	WriteText(hWnd, "- Directory :", bWriteToFile);    

	if(Get_WindowsDirectory(sVar, sizeof(sVar)))
    {
        sprintf_s(sTemp, sizeof(sTemp), "\t Windows: %s ", sVar);
        WriteText(hWnd, sTemp, bWriteToFile);
    }

    if(Get_SystemDirectory(sVar, sizeof(sVar)))
    {
        sprintf_s(sTemp, sizeof(sTemp), "\t System: %s ", sVar);
        WriteText(hWnd, sTemp, bWriteToFile);
    }

	if(Get_TempDirectory(sVar, sizeof(sVar)))
    {
        sprintf_s(sTemp, sizeof(sTemp), "\t Temporary: %s", sVar);
        WriteText(hWnd, sTemp, bWriteToFile);
    }

	if(Get_CurrentDirectory(sVar, sizeof(sVar)))
    {
        sprintf_s(sTemp, sizeof(sTemp), "\t Current: %s", sVar);
        WriteText(hWnd, sTemp, bWriteToFile);
    }

	GetEnvironmentVariable("path", sVar, sizeof(sVar));
    if(strlen(sVar) > 3)
    {
        sprintf_s(sTemp, sizeof(sTemp), "\t Path: %s ", sVar);
        WriteText(hWnd, sTemp, bWriteToFile);
    }

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool WriteBootType(bool bWriteToFile, HWND hWnd)
{
    int iType = GetSystemMetrics(SM_CLEANBOOT);
    WriteText(hWnd, "- Boot Type : ", bWriteToFile);
    if(iType == 0) WriteText(hWnd, "\t Normal", bWriteToFile);
    if(iType == 1) WriteText(hWnd, "\t Fail-Safe", bWriteToFile);
    if(iType == 2) WriteText(hWnd, "\t Fail-Safe With Network", bWriteToFile);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WriteHostInformation(bool bWriteToFile, HWND hWnd, struct hostent *lpHost)
{
    char sTemp[1024];
    struct in_addr MyAddr;
    int iPos = 0;

    WriteText(hWnd, "\t Address List", bWriteToFile);

    while((*lpHost).h_addr_list[iPos])
    {
        memcpy(&MyAddr.s_addr, (*lpHost).h_addr_list[iPos], (*lpHost).h_length );

		sprintf_s(sTemp, sizeof(sTemp), "\t\t (# %d) %s", iPos, inet_ntoa(MyAddr));
        
		WriteText(hWnd, sTemp, bWriteToFile);
        iPos++;
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool PrintWSAData(bool bWriteToFile, HWND hWnd)
{
    int iLO = 0;
    int iHI = 0;

	char sTemp[1024];

    int iResult = 0;
    char sHostName[1024];
    struct hostent *MyHost;

	WORD WSARequiredVersion; // Windows Socket Required Version
	WSADATA WSAData;		 // Windows Socket Required Version
	WSADATA *WSAPointerData; // Windows Socket Required Version

	//---------------------------------------------------------

    WriteText(hWnd, "- Windows Socket Information : ", bWriteToFile);

	// -------- WINSOCK STARTUP --------
	WSARequiredVersion = 0x0101; // Version 1.1
	WSAPointerData = &WSAData;
	if(WSAStartup(WSARequiredVersion, WSAPointerData)!= 0)
	{
		WriteText(hWnd, "\t Winsock is out of date or not installed.", bWriteToFile);
		return false;
	}

	//---------------------------------------------------------

	iLO = LOBYTE(WSAData.wVersion);
    iHI = HIBYTE(WSAData.wVersion);
    sprintf_s(sTemp, sizeof(sTemp), "\t Winsock Version : %d.%d ", iLO, iHI);
    WriteText(hWnd, sTemp, bWriteToFile);
    
	//---------------------------------------------------------

	iLO = LOBYTE(WSAData.wHighVersion);
    iHI = HIBYTE(WSAData.wHighVersion);
    sprintf_s(sTemp, sizeof(sTemp), "\t Winsock HIGH Version : %d.%d ", iLO, iHI);
    WriteText(hWnd, sTemp, bWriteToFile);

	//---------------------------------------------------------

	sprintf_s(sTemp, sizeof(sTemp), "\t Winsock Description : %s ", WSAData.szDescription);
    WriteText(hWnd, sTemp, bWriteToFile);

	//---------------------------------------------------------

	sprintf_s(sTemp, sizeof(sTemp), "\t Max Sockets : %d ", WSAData.iMaxSockets);
    WriteText(hWnd, sTemp, bWriteToFile);

	//---------------------------------------------------------

	sprintf_s(sTemp, sizeof(sTemp), "\t Max UDP Datagram Size : %d ", WSAData.iMaxUdpDg);
    WriteText(hWnd, sTemp, bWriteToFile);

	//---------------------------------------------------------

    if((MyHost = gethostbyname("localhost")) == NULL)
    {
        WriteText(hWnd, "GetHostByName() failed.", bWriteToFile);
        return false;
    }

	WriteText(hWnd, "", bWriteToFile);

	sprintf_s(sTemp, sizeof(sTemp), "-Local Host %s Info :", (*MyHost).h_name);
    WriteText(hWnd, sTemp, bWriteToFile);
    WriteHostInformation(bWriteToFile, hWnd, MyHost);

    if( gethostname(sHostName, sizeof(sHostName)) == SOCKET_ERROR )
    {
        WriteText(hWnd, "GetHostName() failed.", bWriteToFile);
        return false;
    }

    if((MyHost = gethostbyname(sHostName)) == NULL)
    {
        WriteText(hWnd, "- GetHostByName() failed.", bWriteToFile);
        return false;
    }

	WriteText(hWnd, "", bWriteToFile);

	sprintf_s(sTemp, sizeof(sTemp), "-Local Machine %s Info :", (*MyHost).h_name);
    WriteText(hWnd, sTemp, bWriteToFile);

    WriteHostInformation(bWriteToFile, hWnd, MyHost);

	//---------------------------------------------------------

	if(WSACleanup() == SOCKET_ERROR)
	{
		return false;
	}

	//---------------------------------------------------------

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WriteBIOSInfo(bool bWriteToFile, HWND hWnd)
{
    char sTemp[1024];

    char sBuf[1024];
    DWORD dwBufSz = sizeof(sBuf);

    WriteText(hWnd, "- System BIOS : ", bWriteToFile);

    memset(sBuf, 0, sizeof(sBuf));
	dwBufSz = sizeof(sBuf);
	if(Get_StringRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System", "SystemBIOSVersion", sBuf, dwBufSz))
    {
        sprintf_s(sTemp, sizeof(sTemp), "\t BIOS Version : %s", sBuf);
        WriteText(hWnd, sTemp, bWriteToFile);
    }else{
        sprintf_s(sTemp, sizeof(sTemp), "\t BIOS Version : %s", "N/A");
        WriteText(hWnd, sTemp, bWriteToFile);
    }

    memset(sBuf, 0, sizeof(sBuf));
	dwBufSz = sizeof(sBuf);
    if(Get_StringRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System", "SystemBIOSDate", sBuf, dwBufSz))
    {
        sprintf_s(sTemp, sizeof(sTemp), "\t BIOS Date : %s", sBuf);
        WriteText(hWnd, sTemp, bWriteToFile);
    }else{
        sprintf_s(sTemp, sizeof(sTemp), "\t BIOS Date : %s", "N/A");
        WriteText(hWnd, sTemp, bWriteToFile);
    }

    memset(sBuf, 0, sizeof(sBuf));
	dwBufSz = sizeof(sBuf);
    if(Get_StringRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System", "Identifier", sBuf, dwBufSz))
    {
        sprintf_s(sTemp, sizeof(sTemp), "\t Identifier : %s ", sBuf);
        WriteText(hWnd, sTemp, bWriteToFile);
    }else{
        sprintf_s(sTemp, sizeof(sTemp), "\t Identifier : %s", "N/A");
        WriteText(hWnd, sTemp, bWriteToFile);
    }

    WriteText(hWnd, "", bWriteToFile);

	WriteText(hWnd, "- Video BIOS :", bWriteToFile);

    memset(sBuf, 0, sizeof(sBuf));
	dwBufSz = sizeof(sBuf);
    if(Get_StringRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System", "VideoBIOSVersion", sBuf, dwBufSz))
    {
        sprintf_s(sTemp, sizeof(sTemp), "\t BIOS Version : %s", sBuf);
        WriteText(hWnd, sTemp, bWriteToFile);
    }else{
        sprintf_s(sTemp, sizeof(sTemp), "\t BIOS Version : %s", "N/A");
        WriteText(hWnd, sTemp, bWriteToFile);
    }

    memset(sBuf, 0, sizeof(sBuf));
	dwBufSz = sizeof(sBuf);
    if(Get_StringRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System", "VideoBIOSDate", sBuf, dwBufSz))
    {
        sprintf_s(sTemp, sizeof(sTemp), "\t BIOS Date : %s", sBuf);
        WriteText(hWnd, sTemp, bWriteToFile);
    }else{
        sprintf_s(sTemp, sizeof(sTemp), "\t BIOS Date : %s", "N/A");
        WriteText(hWnd, sTemp, bWriteToFile);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WriteOsVersion(bool bWriteToFile, HWND hWnd)
{
    char sTemp[255];
    char sOSInfo[255];

	char *sOption = NULL;

    //--------------------------------------------------------------------
    WriteText(hWnd, "- OS Version :", bWriteToFile);
    //--------------------------------------------------------------------

	Get_OsVersion(sOSInfo, sizeof(sOSInfo));
    sprintf_s(sTemp, sizeof(sTemp), "\t %s", sOSInfo);
	WriteText(hWnd, sTemp, bWriteToFile);
	
	//--------------------------------------------------------------------

	if(IsWinNT())
        sOption = "True";
    else sOption = "False";
    sprintf_s(sTemp, sizeof(sTemp), "\t Is System NT based? : %s", sOption);
    WriteText(hWnd, sTemp, bWriteToFile);

	//--------------------------------------------------------------------

	if(GetSystemMetrics(SM_DBCSENABLED) == 0)
        sOption = "False";
    else sOption = "True";
    sprintf_s(sTemp, sizeof(sTemp), "\t (DBCS) User.Exe Version : %s", sOption);
    WriteText(hWnd, sTemp, bWriteToFile);

    //--------------------------------------------------------------------

    if(GetSystemMetrics(SM_DEBUG) == 0)
        sOption = "False";
    else sOption = "True";
    sprintf_s(sTemp, sizeof(sTemp), "\t Debugging User.Exe Version : %s", sOption);
    WriteText(hWnd, sTemp, bWriteToFile);

	//--------------------------------------------------------------------
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int GetLastCharPosv(char *sInBuf, int iInBufSz)
{
    int iRPos = iInBufSz;
    while(iRPos != 0)
    {
        if(!IsWhiteSpace(sInBuf[iRPos]))
            return iRPos + 1;
		iRPos--;
	}
    return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GetCycleCount(unsigned __int64 *i64Cycles)
{
    __asm {
		push edx
		rdtsc
		push edi
		mov edi, dword ptr i64Cycles
		mov dword ptr [edi], eax
		mov dword ptr [edi+4], edx
		pop edi
		pop edx
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD GetCPUSpeedAlt(void)
{
	unsigned __int64 iBefore = 0;
	unsigned __int64 iAfter = 0;

	GetCycleCount(&iBefore);
	Sleep(1000);
	GetCycleCount(&iAfter);

	return((DWORD)(iAfter - iBefore) / 1000000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsCPUID()
{
	__try 
	{
		_asm 
		{
			xor eax, eax
			cpuid
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) 
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool WriteProcessorFeatures(bool bWriteToFile, HWND hWnd)
{
	if (!IsCPUID())
	{
		return false;
	}

	DWORD dwStandard = 0;
	DWORD dwFeature = 0;
	DWORD dwMax = 0;
	DWORD dwExt = 0;
	int feature = 0;
	int os_support = 0;
	
	union {
		char cBuffer[12+1];
		struct {
			DWORD dw0;
			DWORD dw1;
			DWORD dw2;
		} stc;
	} Vendor;

	memset(&Vendor, 0, sizeof(Vendor));

    _asm {
		push ebx
	    push ecx
	    push edx
	    
    	// get the vendor string
	    xor eax, eax
	    cpuid
	    mov dwMax, eax
	    mov Vendor.stc.dw0, ebx
	    mov Vendor.stc.dw1, edx
	    mov Vendor.stc.dw2, ecx
	    
		// get the Standard bits
	    mov eax, 1
	    cpuid
	    mov dwStandard, eax
    	mov dwFeature, edx
    
    	// get AMD-specials
	    mov eax, 80000000h
	    cpuid
	    cmp eax, 80000000h
	    jc notamd
	    mov eax, 80000001h
	    cpuid
	    mov dwExt, edx
    
    	notamd:
	    pop ecx
	    pop ebx
	    pop edx
	};

	char sFeatures[255];
	strcpy_s(sFeatures, sizeof(sFeatures), "");

	if (dwFeature & MMX_SUPPORTED)
		strcat_s(sFeatures, sizeof(sFeatures), "MMX ");
	if (dwExt & AMD_3DNOW_SUPPORTED) 
		strcat_s(sFeatures, sizeof(sFeatures), "3DNow! ");
	if (dwFeature & SSE_SUPPORTED) 
		strcat_s(sFeatures, sizeof(sFeatures), "SSE ");
	if (dwFeature & SSE2_SUPPORTED) 
		strcat_s(sFeatures, sizeof(sFeatures), "SSE2 ");

	if(strlen(sFeatures) > 0)
	{
		char sTemp[1024];
		sprintf_s(sTemp, sizeof(sTemp), "\t Features : %s", sFeatures);
		WriteText(hWnd, sTemp, bWriteToFile);
	}

	/*
	int iFamily = (short int)((dwStandard >> 8) & 0xF); // retrieve family
	int iFamilyEx = 0;
	if (iFamily == 15) // retrieve extended family
		iFamilyEx = (dwStandard >> 16) & 0xFF0;

	int iModel = (short int)((dwStandard >> 4) & 0xF);  // retrieve model
	int iModelEx = 0;
	if (iModel == 15)	// retrieve extended model
		iModelEx = (dwStandard >> 12) & 0xF;

	int iStepping = (short int)((dwStandard) & 0xF);    // retrieve stepping
	*/

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WriteProcessorInformation(bool bWriteToFile, HWND hWnd)
{
	char *sOption = NULL;
	char sBuf[1024];
	char sTemp[1024];

	DWORD dwBuf = 0;
	DWORD dwBufSz = sizeof(sBuf);

	SYSTEM_INFO SI;

	memset(&SI, 0, sizeof(SI));

    GetSystemInfo(&SI);

    //--------------------------------------------------------------------

    WriteText(hWnd, "- Processor :", bWriteToFile);

    //--------------------------------------------------------------------

    sprintf_s(sTemp, sizeof(sTemp), "\t Number of Processors : %d", SI.dwNumberOfProcessors);
    WriteText(hWnd, sTemp, bWriteToFile);

    //--------------------------------------------------------------------

    memset(sBuf, 0, sizeof(sBuf));
	dwBufSz = sizeof(sBuf);
	if(Get_StringRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "VendorIdentifier", sBuf, dwBufSz))
        sprintf_s(sTemp, sizeof(sTemp), "\t Processor Type : %s", sBuf);
    else sprintf_s(sTemp, sizeof(sTemp), "\t Processor Type : %s", "N/A");
    WriteText(hWnd, sTemp, bWriteToFile);

	//--------------------------------------------------------------------

    memset(sBuf, 0, sizeof(sBuf));
	dwBufSz = sizeof(sBuf);
    if(Get_StringRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString", sBuf, dwBufSz))
	{
		LTrim(sBuf);
		sprintf_s(sTemp, sizeof(sTemp), "\t Processor Name : %s", sBuf);
	}
    else sprintf_s(sTemp, sizeof(sTemp), "\t Processor Name : %s", "N/A");
    WriteText(hWnd, sTemp, bWriteToFile);

	//--------------------------------------------------------------------

	WriteProcessorFeatures(bWriteToFile, hWnd);

	//--------------------------------------------------------------------

    memset(sBuf, 0, sizeof(sBuf));
	dwBufSz = sizeof(sBuf);
    if(Get_StringRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "Identifier", sBuf, dwBufSz))
        sprintf_s(sTemp, sizeof(sTemp), "\t Identifier : %s", sBuf);
    else sprintf_s(sTemp, sizeof(sTemp), "\t Identifier : %s", "N/A");
    WriteText(hWnd, sTemp, bWriteToFile);

	//--------------------------------------------------------------------

    if(!Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "~MHz", dwBuf))
	{
		dwBuf = GetCPUSpeedAlt();
	}

	float fHz = (float)dwBuf;
	char *sDivisor = "MHz";

	if(fHz >= 1000)
	{
		fHz /= 1000.0f;
		sDivisor = "GHz";
	}

    sprintf_s(sTemp, sizeof(sTemp), "\t Speed : %.2f%s", fHz, sDivisor);
    WriteText(hWnd, sTemp, bWriteToFile);

	//--------------------------------------------------------------------

    sprintf_s(sTemp, sizeof(sTemp), "\t Level : %d ", SI.dwProcessorType);
    WriteText(hWnd, sTemp, bWriteToFile);

	//--------------------------------------------------------------------

    sprintf_s(sTemp, sizeof(sTemp), "\t Granularity : %dK", SI.dwAllocationGranularity/1024);
    WriteText(hWnd, sTemp, bWriteToFile);

    //--------------------------------------------------------------------

	sprintf_s(sTemp, sizeof(sTemp), "\t Revision : %d", SI.wProcessorRevision);
    WriteText(hWnd, sTemp, bWriteToFile);

    //--------------------------------------------------------------------

	if(GetSystemMetrics(SM_SLOWMACHINE) == 1)
        sOption = "False";
    else sOption = "True";
    sprintf_s(sTemp, sizeof(sTemp), "\t High-End Processor : %s", sOption);
    WriteText(hWnd, sTemp, bWriteToFile);

    //--------------------------------------------------------------------
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WriteMemoryStatus(bool bWriteToFile, HWND hWnd)
{
    char sTemp[255];
	char sSize[64];

	MEMORYSTATUSEX MS;
	memset(&MS, 0, sizeof(MS));
	MS.dwLength = sizeof(MS);
	GlobalMemoryStatusEx(&MS);

	//--------------------------------------------------------------------

	WriteText(hWnd, "- Memory Information :", bWriteToFile);

	//--------------------------------------------------------------------

	FileSizeFriendly(MS.ullTotalPhys, 2, sSize, sizeof(sSize));
	sprintf_s(sTemp, sizeof(sTemp), "\t Total Physical Memory: %s", sSize);
	WriteText(hWnd, sTemp, bWriteToFile);

	//--------------------------------------------------------------------

	FileSizeFriendly(MS.ullTotalPhys - MS.ullAvailPhys, sSize, sizeof(sSize));
	sprintf_s(sTemp, sizeof(sTemp), "\t Used Memory: %s", sSize);
	WriteText(hWnd, sTemp, bWriteToFile);

	//--------------------------------------------------------------------

	FileSizeFriendly(MS.ullAvailPhys, sSize, sizeof(sSize));
	sprintf_s(sTemp, sizeof(sTemp), "\t Free Memory: %s", sSize);
	WriteText(hWnd, sTemp, bWriteToFile);

	//--------------------------------------------------------------------
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WriteNetWorkInformation(bool bWriteToFile, HWND hWnd)
{
    char *sOption = NULL;
    char sBuf[255];
	char sTemp[255];

	WriteText(hWnd, "- Windows Network :", bWriteToFile);

	if(Get_MachineName(sBuf, sizeof(sBuf)))
	{
	    sprintf_s(sTemp, sizeof(sTemp), "\t Machine Name : %s", sBuf);
	    WriteText(hWnd, sTemp, bWriteToFile);
	}

    if(Get_UserName(sBuf, sizeof(sBuf)))
	{
		sprintf_s(sTemp, sizeof(sTemp), "\t User Name : %s", sBuf);
		WriteText(hWnd, sTemp, bWriteToFile);
	}

    if(GetSystemMetrics(SM_SECURE) == 0)
        sOption = "False";
    else sOption = "True";
    sprintf_s(sTemp, sizeof(sTemp), "\t Security Present : %s", sOption);
    WriteText(hWnd, sTemp, bWriteToFile);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

