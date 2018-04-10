#ifndef _GetInfo_H
#define _GetInfo_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SEPERATOR "---------------------------------------------------------------------\r\n"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern const char *sOptions[];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool PopInfoList(bool bWriteToFile, HWND hWnd, char *sOption);
void WriteOsVersion(bool bWriteToFile, HWND hWnd);
void WriteProcessorInformation(bool bWriteToFile, HWND hWnd);
void WriteMemoryStatus(bool bWriteToFile, HWND hWnd);
void WriteNetWorkInformation(bool bWriteToFile, HWND hWnd);
void WriteBIOSInfo(bool bWriteToFile, HWND hWnd);
bool PrintWSAData(bool bWriteToFile, HWND hWnd);
void WriteHostInformation(bool bWriteToFile, HWND hWnd, struct hostent *lpHost);
bool WriteBootType(bool bWriteToFile, HWND hWnd);
bool WriteEnvironmentInfo(bool bWriteToFile, HWND hWnd);
bool Write_DiskInformation(bool bWriteToFile, HWND hWnd);
bool WriteDriveInfo(bool bWriteToFile, HWND hWnd, char *sDrive);
bool WriteDisplayInformation(bool bWriteToFile, HWND hWnd);
bool WriteMouseInformation(bool bWriteToFile, HWND hWnd);
bool SaveAllInfoToFile(char *sTarget);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

