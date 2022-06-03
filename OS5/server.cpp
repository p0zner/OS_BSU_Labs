#include<iostream>
#include<windows.h>

using namespace std;

int main() {
	int sizeOfArray;
	cout << "Input size of array: ";
	cin >> sizeOfArray;

	char* array = new char[sizeOfArray];
	cout << "Input elements of the array: " << endl;
	for (int i = 0; i < sizeOfArray; i++) {
		cin >> array[i];
	}
	
	int N;
	cout << "Input N: ";
	cin >> N;

	HANDLE hChildEnableRead, hServerEnableRead;
	
	hChildEnableRead = CreateEvent(NULL, FALSE, FALSE, "ChildRead");
	hServerEnableRead = CreateEvent(NULL, FALSE, FALSE, "ServerRead");

	HANDLE hWritePipe, hReadPipe;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);

	STARTUPINFO stp;
	PROCESS_INFORMATION pi;

	ZeroMemory(&stp, sizeof(STARTUPINFO));
	stp.cb = sizeof(STARTUPINFO);

	wchar_t commandLine[80];
	wsprintf((LPSTR)commandLine, "child.exe %d %d", (int)hWritePipe, (int)hReadPipe);
	CreateProcess(NULL, (LPSTR)commandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &stp, &pi);

	DWORD dwBytesWritten, dwBytesRead;

	WriteFile(hWritePipe, &sizeOfArray, sizeof(int), &dwBytesWritten, NULL);
	WriteFile(hWritePipe, &N, sizeof(int), &dwBytesWritten, NULL);

	for (int i = 0; i < sizeOfArray; ++i)
		WriteFile(hWritePipe, &array[i], sizeof(int), &dwBytesWritten, NULL);
	SetEvent(hChildEnableRead);

	WaitForSingleObject(hServerEnableRead, INFINITE);

	int sizeOfNewArray;
	ReadFile(hReadPipe, &sizeOfNewArray, sizeof(int), &dwBytesRead, NULL);

	char* resultArr = new char[sizeOfNewArray];
	for (int i = 0; i < sizeOfNewArray; ++i)
		ReadFile(hReadPipe, &resultArr[i], sizeof(int), &dwBytesRead, NULL);

	cout << "Number of elements is " << sizeOfNewArray << endl;

	cout << "Result : ";
	for (int i = 0; i < sizeOfNewArray; ++i)
		cout << resultArr[i] << " ";

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(hServerEnableRead);
	CloseHandle(hChildEnableRead);

	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}
