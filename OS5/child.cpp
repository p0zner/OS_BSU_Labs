#include<iostream>
#include<windows.h>

using namespace std;

int main(int argc, char* argv[]) {
	HANDLE hWritePipe = (HANDLE)atoi(argv[1]);
	HANDLE hReadPipe = (HANDLE)atoi(argv[2]);

	HANDLE hChildEnableRead, hServerEnableRead;
	hChildEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, "ChildRead");
	hServerEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, "ServerRead");

	WaitForSingleObject(hChildEnableRead, INFINITE);

	int size, number;
	DWORD dwBytesRead, dwBytesWrite;
	
	ReadFile(hReadPipe, &size, sizeof(int), &dwBytesRead, NULL);
	ReadFile(hReadPipe, &number, sizeof(int), &dwBytesRead, NULL);
	
	char* arr = new char[size];
	char eng[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
	for (int i = 0; i < size; i++)
		ReadFile(hReadPipe, &arr[i], sizeof(int), &dwBytesRead, NULL);

	int counter = 0;
	cout << "Array with english letters: ";
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 26; j++) {
			if (arr[i] == eng[j]) {
				cout << arr[i] << " ";
				counter++;
			}
		}
	}
	cout << endl;
	
	char* resultArr = new char[counter];
	int index = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 26; j++) {
			if (arr[i] == eng[j]) {
				resultArr[index] = arr[i];
				index++;
			}
		}
	}

	WriteFile(hWritePipe, &counter, sizeof(int), &dwBytesWrite, NULL);
	for (int i = 0; i < counter; ++i)
		WriteFile(hWritePipe, &resultArr[i], sizeof(int), &dwBytesWrite, NULL);
	SetEvent(hServerEnableRead);

	delete[]arr;
	delete[]resultArr;
	system("pause");
	return 0;
}
