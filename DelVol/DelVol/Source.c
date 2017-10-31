#include <Windows.h>
#include <wchar.h>

#pragma comment(lib, "Kernel32.lib")

LPWSTR AssignDriveLetter(DWORD logicalDrives)
{
	WCHAR *driveletters[22] = {
		L"E:\\", L"F:\\", L"G:\\", L"H:\\", L"I:\\",
		L"J:\\", L"K:\\", L"L:\\", L"M:\\", L"N:\\",
		L"O:\\", L"P:\\", L"Q:\\", L"R:\\", L"S:\\", L"T:\\",
		L"U:\\", L"V:\\", L"W:\\", L"X:\\", L"Y:\\", L"Z:\\" };

	int i = 0;	

	//Shift to letter "E:\"
	logicalDrives >>= 4;

	while (logicalDrives & 1)
	{
		i++;
		logicalDrives >>= 1;
		
	}

	return driveletters[i];

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
	DWORD drivesOn;	
	LPCWSTR volumeLetter=NULL;

	//GetVolumeNameForVolumeMountPoint
	BOOL getVolume;
	LPCWSTR oldDriveLetter = L"D:\\";
	WCHAR volumeGUID[50];
	DWORD guidSize = 50;

	//SetVolumeMountPoint
	BOOL setVolume;
	/*LPCWSTR newDriveLetter =NULL*/;

	//Get all available logical drives
	drivesOn = GetLogicalDrives();

	if (drivesOn == 0)
	{
		ShowError(GetLastError());
	}
	else if (drivesOn & 8)	//D drive is on! 
	{
		/*	We know the D is busy, so me need to
			get its volume GUID path, delete the 
			volume mount point and set it with another
			letter. I will use G.	*/

		

		//Here we get the volume GUID path
		getVolume = GetVolumeNameForVolumeMountPointW(oldDriveLetter, volumeGUID, guidSize);

		if (!getVolume)	//Function failed
		{
			fwprintf(stderr, L"Could not get volume GUID path, error: ");
			ShowError(GetLastError());
			return FALSE;
		}

		//Now we have to release the letter by deleting
		//the volume mount point

		if (!DeleteVolumeMountPointW(oldDriveLetter))
		{
			fwprintf(stderr, L"Could not delete the volume mount point, error: ");
			ShowError(GetLastError());
			return FALSE;
		}

		//Finally, we just have to assign a new
		//letter to the volume and mount it

		setVolume = SetVolumeMountPointW(AssignDriveLetter(drivesOn), volumeGUID);

		if (!setVolume)	//Function failed
		{
			fwprintf(stderr, L"Could not set a new volume mount point, error: ");
			ShowError(GetLastError());
			return FALSE;
		}

		//Let's the user know we have finish
		wprintf(L"\nDrive letter has been successfully changed.\n");	
		

	}
	else
	{
		fwprintf(stderr, L"\nThere is not available drive.\n");
		return FALSE;
	}

	return 0;
}