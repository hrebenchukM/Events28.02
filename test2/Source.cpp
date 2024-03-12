#pragma once
#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <fstream>
#include <string>
using namespace std;

#include <ctime>


BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

DWORD WINAPI Write(LPVOID lp)
{
	HWND hWnd = HWND(lp);
	// получим дескриптор существующего события
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
	if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
	{
		MessageBox(0, L"Write", NULL, MB_OK);

		wofstream myfile("Test.txt", ios::out);
		myfile << "Hello world\n";

		myfile.close();

	}
	ResetEvent(hEvent);

	return 0;
}
DWORD WINAPI Read(LPVOID lp)
{
	HWND hWnd = HWND(lp);
	// получим дескриптор существующего события
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
	TCHAR buff[100];

	if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
	{
		wifstream myfile("Test.txt", ios::in);
		wstring res;
		while (!myfile.eof())
		{
			myfile >> buff;
			res += buff;
			res += L" ";
		}
		myfile.close();
		MessageBox(0, res.c_str(), NULL, MB_OK);

	}
	ResetEvent(hEvent);

	return 0;
}



DWORD WINAPI DisplayFileContent(LPVOID lp)
{
	HWND hWnd = HWND(lp);

	// получим дескриптор существующего события
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
	TCHAR buff[100];

	if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
	{
		wifstream myfile("Test.txt", ios::in);
		wstring res;
		while (!myfile.eof())
		{
			myfile >> buff;
			res += buff;
			res += L" ";
		}
		myfile.close();
		HWND hEdit = GetDlgItem(hWnd, IDC_EDIT1);
		SetWindowText(hEdit, res.c_str());

	}
	ResetEvent(hEvent);

	return 0;



	
}


DWORD WINAPI DateTime(LPVOID lp)
{
	HWND hWnd = HWND(lp);
	SYSTEMTIME st;
	GetLocalTime(&st);

	wchar_t buffer[80];
	swprintf(buffer, sizeof(buffer), L"%04d-%02d-%02d %02d:%02d:%02d",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	HWND hEdit = GetDlgItem(hWnd, IDC_EDIT2);
	SetWindowText(hEdit, buffer);



	return 0;

}




BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
	switch (mes)
	{
	case WM_INITDIALOG:
	{
		HANDLE hEvent = CreateEvent(NULL, TRUE /* ручной сброс события */, FALSE /* несигнальное состояние */, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
		HANDLE h = CreateThread(NULL, 0, Write, hWnd, 0, NULL);
		CloseHandle(h);

		h = CreateThread(NULL, 0, Read, hWnd, 0, NULL);
		CloseHandle(h);

		h = CreateThread(NULL, 0, DisplayFileContent, hWnd, 0, NULL);
		CloseHandle(h);

		h = CreateThread(NULL, 0, DateTime, hWnd, 0, NULL);
		CloseHandle(h);

		
	}
	break;
	case WM_COMMAND:
	{
		HANDLE h = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2BA7C99B-D9F7-4485-BB3F-E4735FFEF139}"));
		SetEvent(h); // перевод события в сигнальное состояние

		
	}
	break;
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		break;

	}
	return FALSE;
}