#pragma once
#include <Windows.h>
#include <wchar.h>

#pragma comment(lib, "Kernel32.lib")

BOOL ValidateLetter(LPWSTR driveLetter)
{
	BOOL isValid = TRUE;
	if (_wcsicmp(driveLetter, L"A:\\") == 0 || _wcsicmp(driveLetter, L"B:\\") == 0 ||
		_wcsicmp(driveLetter, L"C:\\") == 0)
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
	WCHAR *driveLetters[23] = { L"D:\\",
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