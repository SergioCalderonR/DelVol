#include <Windows.h>
#include <wchar.h>

#pragma comment(lib, "Kernel32.lib")

VOID ShowHelp(VOID)
{
	wprintf(L"\nDelVol - Delete a volume letter and assign it a new one\n"
			L"Copyrigth (C) 2017 Sergio Calderon\n"
			L"\nUsage: DelVol.exe [VolumeLetter]\n" L"\nExample: DelVol.exe D:\\ \n");
}

BOOL ValidateLetter(LPWSTR driveLetter)
{
	BOOL isValid = TRUE;
	if (_wcsicmp(driveLetter, L"A:\\") == 0 || _wcsicmp(driveLetter, L"B:\\")==0 ||
		_wcsicmp(driveLetter, L"C:\\")==0)
	{
		isValid = FALSE;
	}

	return isValid;

}

LPWSTR GetVolumeLetter(LPWSTR letter, DWORD * pLetterIndex)
{
	
	WCHAR *driveLetters[26] = {
		L"A:\\", L"B:\\", L"C:\\", L"D:\\", L"E:\\", L"F:\\",
		L"G:\\", L"H:\\", L"I:\\", L"J:\\", L"K:\\", L"L:\\",
		L"M:\\", L"N:\\", L"O:\\", L"P:\\", L"Q:\\", L"R:\\",
		L"S:\\", L"T:\\", L"U:\\", L"V:\\", L"W:\\", L"X:\\",
		L"Y:\\", L"Z:\\" };
	

	for (int i = 0; i < 26; i++)
	{
		if (_wcsicmp(letter, driveLetters[i]) == 0)
		{
			*pLetterIndex = i;
		}
	}

	return letter;

}

LPWSTR AssignDriveLetter(DWORD logicalDrives, DWORD * pLetterIndex)
{
	WCHAR *driveLetters[23] = {L"D:\\",
		L"E:\\", L"F:\\", L"G:\\", L"H:\\", L"I:\\",
		L"J:\\", L"K:\\", L"L:\\", L"M:\\", L"N:\\",
		L"O:\\", L"P:\\", L"Q:\\", L"R:\\", L"S:\\", L"T:\\",
		L"U:\\", L"V:\\", L"W:\\", L"X:\\", L"Y:\\", L"Z:\\" };

	int i = 0;

	//If E:\ volume is passed, go back to D:\ to start over
	if (*pLetterIndex == 4)
	{
		
		logicalDrives <<= 1;
	}

	while (logicalDrives & 1)
	{
		i++;
		logicalDrives >>= 1;
		
	}

	return driveLetters[i];

}

VOID ShowError(DWORD errId)
{
	DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS;

	LPWSTR errMsg;

	if (!FormatMessageW(flags, 0, errId, 0, (LPWSTR)&errMsg, 0, NULL))
	{
		fwprintf(stderr, L"\nCould not show error message, code: %lu", GetLastError());

	}

	wprintf(L"%s", errMsg);
	LocalFree(errMsg);
}

//Main function. All magic happens here! 
int wmain(int argc, WCHAR * argv[])
{
	DWORD driveIndex=0;
	PDWORD pDriveIndex = &driveIndex;

	DWORD drivesOn;	
	LPCWSTR volumeLetter=NULL;
	

	//GetVolumeNameForVolumeMountPoint
	BOOL getVolume;
	LPCWSTR oldDriveLetter=NULL;
	WCHAR volumeGUID[50];
	DWORD guidSize = 50;

	//SetVolumeMountPoint
	BOOL setVolume;

	if (argc != 2)
	{
		ShowHelp();
		return FALSE;
	}

	if (!ValidateLetter(argv[1]))
	{
		fwprintf(stderr, L"\nWe cannot use drive letter %s\n", argv[1]);
		return FALSE;
	}

	oldDriveLetter = GetVolumeLetter(argv[1], pDriveIndex);	

	//Get all available logical drives
	drivesOn = GetLogicalDrives();

	drivesOn >>= *pDriveIndex;

	if (drivesOn & 1)	//Drive is on! 
	{
		/*	We know the drive is busy, so me need to
			get its volume GUID path, delete the 
			volume mount point and set it with another
			letter. I will use next available letter.	*/		

		//Here we get the volume GUID path
		getVolume = GetVolumeNameForVolumeMountPointW(oldDriveLetter, volumeGUID, guidSize);

		if (!getVolume)	//Function failed
		{
			fwprintf(stderr, L"\nCould not get volume GUID path, error: ");
			ShowError(GetLastError());
			return FALSE;
		}

		//Now we have to release the letter by deleting
		//the volume mount point

		if (!DeleteVolumeMountPointW(oldDriveLetter))
		{
			fwprintf(stderr, L"\nCould not delete the volume mount point, error: ");
			ShowError(GetLastError());
			return FALSE;
		}

		//Finally, we just have to assign a new
		//letter to the volume and mount it

		//AssignDriveLetter looks for the next available letter

		setVolume = SetVolumeMountPointW(AssignDriveLetter(drivesOn, pDriveIndex), volumeGUID);

		if (!setVolume)	//Function failed
		{
			fwprintf(stderr, L"\nCould not set a new volume mount point, error: ");
			ShowError(GetLastError());
			return FALSE;
		}

		//Let's the user know we have finish
		wprintf(L"\nDrive letter has been successfully changed.\n");			

	}
	else
	{
		fwprintf(stderr, L"\nThere is not volume mounted with this letter.\n");
		return FALSE;
	}

	return 0;
}