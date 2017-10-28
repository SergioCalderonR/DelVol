#include <Windows.h>
#include <wchar.h>


#pragma comment(lib, "Kernel32.lib")

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

	wprintf(L"\n%s", errMsg);
	LocalFree(errMsg);
}

//Main function. All magic happens here! 
int wmain(int argc, WCHAR * argv[])
{
	DWORD drivesOn;
	BOOL delVolume;
	LPCWSTR volumeLetter=NULL;

	//Get all available logical drives
	drivesOn = GetLogicalDrives();

	if (drivesOn == 0)
	{
		ShowError(GetLastError());
	}
	else if (drivesOn & 8)	//D drive is on! 
	{
		volumeLetter = L"D:\\";
		delVolume = DeleteVolumeMountPointW(volumeLetter);	//Let's remove the letter

		if (delVolume)	//It worked!
		{
			wprintf(L"Drive letter was deleted.\n");
		}
		else
		{
			//This error message could be useful for troubleshooting
			fwprintf(stderr, L"Drive letter wasn't deleted, error: ");
			ShowError(GetLastError());
		}

	}


	return 0;
}