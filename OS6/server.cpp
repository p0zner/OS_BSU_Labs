#include <windows.h>
#include <iostream>

using namespace std;

int main() {
	SECURITY_ATTRIBUTES sa; // атрибуты защиты
	SECURITY_DESCRIPTOR sd; // дескриптор защиты
	HANDLE hNamedPipe;
	DWORD dwBytesRead; // для числа прочитанных байтов
	DWORD dwBytesWrite; // для числа записанных байтов

	// инициализация атрибутов защиты
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE; // дескриптор канала ненаследуемый

	// инициализируем дескриптор защиты
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);

	// устанавливаем атрибуты защиты, разрешая доступ всем пользователям
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;

	// создаем именованный канал для чтения
	hNamedPipe = CreateNamedPipe("\\\\.\\pipe\\demo_pipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, &sa);

	// проверяем на успешное создание
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cout << "Invalid handle value.\n";
		system("pause");
		return 0;
	}

	// ждем, пока клиент свяжется с каналом
	cout << "The server is waiting for connection with a client." << endl;
	if (!ConnectNamedPipe(hNamedPipe, (LPOVERLAPPED)NULL)) {
		cout << "Connection failed.\n";
		system("pause");
		return 0;
	}

	double N;
	cout << "Input number N\n";
	cin >> N;

	cout << "Input array size:\n";
	int size;
	cin >> size;

	char* arr = new char[size];
	char eng[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
	cout << "Input " << size << " elemets, type char.\n";
	for (int i = 0; i < size; i++) {
		cin >> arr[i];
	}

	if (!WriteFile(hNamedPipe, &N, sizeof(N), &dwBytesWrite, (LPOVERLAPPED)NULL)) {
		// ошибка записи
		cout << "Write N failed.\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return 0;
	}

	if (!WriteFile(hNamedPipe, &size, sizeof(size), &dwBytesWrite, (LPOVERLAPPED)NULL)) {
		// ошибка записи
		cout << "Write size failed.\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return 0;
	}

	for (int i = 0; i < size; i++) {
		char num = arr[i];
		if (!WriteFile(hNamedPipe, &num, sizeof(num), &dwBytesWrite, (LPOVERLAPPED)NULL)) {
			// ошибка записи
			cout << "Write number failed.\n";
			system("pause");
			CloseHandle(hNamedPipe);
			return 0;
		}
	}

	int rSize;

	if (!ReadFile(hNamedPipe, &rSize, sizeof(rSize), &dwBytesRead, (LPOVERLAPPED)NULL)) {
		cout << "Read size failed\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return GetLastError();
	}

	cout << "Result array: \n";
	for (int i = 0; i < rSize; i++)
	{
		char data;
		if (!ReadFile(hNamedPipe, &data, sizeof(data), &dwBytesRead, (LPOVERLAPPED)NULL)) {
			cout << "Read number failed.\n";
			system("pause");
			CloseHandle(hNamedPipe);
			return GetLastError();
		}
		cout << data << " ";
	}
	cout << endl;

	// закрываем дескриптор канала
	CloseHandle(hNamedPipe);

	system("pause");

	return 0;
}
