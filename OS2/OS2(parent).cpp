#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	char lpszAppName[200] = "OS2(child).exe ";

	cout << "Введите позицию K, до которой необходимо найти произведение элементов:\n";
	char* k = new char[10];
	cin >> k;

	cout << "Введите размер массива:\n";
	int size;
	cin >> size;

	char* tmp = new char[10];

	_itoa(size, tmp, 10);
	strcat(lpszAppName, tmp);
	strcat(lpszAppName, " ");

	strcat(lpszAppName, k);
	strcat(lpszAppName, " ");

	cout << "Введите " << size << " символов:\n";
	for (int i = 0; i < size; i++)
	{
		cin >> tmp;
		strcat(lpszAppName, tmp);
		strcat(lpszAppName, " ");
	}

	STARTUPINFO si;
	PROCESS_INFORMATION piApp;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO); 
	si.dwFlags = STARTF_USEFILLATTRIBUTE;
	si.dwFillAttribute = FOREGROUND_RED;

	if (!CreateProcess((LPCWSTR)"OS2(child).exe", (LPWSTR)lpszAppName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
	{
		_cputs("The new process is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}

	_cputs("The new process is created.\n");

	WaitForSingleObject(piApp.hProcess, INFINITE);
	system("pause");
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	TerminateProcess(piApp.hProcess, 1);
	ExitProcess(1);
	return 0;
}