#include <windows.h>
#include <iostream>

using namespace std;

int main() {
	HANDLE hNamedPipe;
	char machineName[80];
	char pipeName[80];
	DWORD dwBytesWritten; // для числа записанных байтов
	DWORD dwBytesRead; // для числа прочитанных байтов

	// вводим имя машины в сети, на которой работает сервер
	cout << "Enter a name of the server machine (. - default): ";
	cin >> machineName;

	// подставляем имя машины в имя канала
	wsprintf(pipeName, "\\\\%s\\pipe\\demo_pipe", machineName);

	// связываемся с именованным каналом
	hNamedPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	// проверяем связь с каналом
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cout << "Invalid handle value.\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return 0;
	}

	double N;

	if (!ReadFile(hNamedPipe, &N, sizeof(N), &dwBytesRead, (LPOVERLAPPED)NULL)) {
		cout << "Read N failed.\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return GetLastError();
	}

	int size;

	if (!ReadFile(hNamedPipe, &size, sizeof(size), &dwBytesRead, (LPOVERLAPPED)NULL)) {
		cout << "Read size failed\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return GetLastError();
	}

	int counter = 0;
	char eng[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
	
	char* result = new char[counter];
	for (int i = 0; i < size; i++)
	{
		char data;

		if (!ReadFile(hNamedPipe, &data, sizeof(data), &dwBytesRead, (LPOVERLAPPED)NULL)) {
			cout << "Read number failed.\n";
			system("pause");
			CloseHandle(hNamedPipe);
			return GetLastError();
		}

		for (int j = 0; j < 26; j++) {
			if (data == eng[j]) {
				result[counter++] = data;
			}
		}
	}
	cout << endl;

	if (!WriteFile(hNamedPipe, &counter, sizeof(counter), &dwBytesWritten, (LPOVERLAPPED)NULL)) {
		// ошибка записи
		cout << "Write size failed.\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return 0;
	}

	for (int i = 0; i < counter; i++) {
		char num = result[i];

		if (!WriteFile(hNamedPipe, &num, sizeof(num), &dwBytesWritten, (LPOVERLAPPED)NULL)) {
			cout << "Write number failed.\n";
			system("pause");
			return GetLastError();
		}
	}

	cout << "Result array:\n";
	for (int i = 0; i < counter; i++) {
		cout << result[i] << " ";
	}
	cout << endl;

	// закрываем дескриптор канала
	CloseHandle(hNamedPipe);

	system("pause");

	return 0;
}