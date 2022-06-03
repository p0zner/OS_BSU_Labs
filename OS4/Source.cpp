#include <windows.h>
#include <iostream>
#include <conio.h>
#include <list>

using namespace std;

HANDLE event1;
HANDLE event2;
HANDLE event3;
HANDLE event4;
HANDLE parentEvent1;
HANDLE parentEvent2;
HANDLE parentEndEvent;
HANDLE semaphore;
bool flag1 = true;
bool flag2 = true;
bool flag3 = true;
volatile int messageParentEventCount = 0;
volatile int messageParentEndCount = 0;

struct ProcessInformation {
	STARTUPINFO info;
	PROCESS_INFORMATION processInformation;

	ProcessInformation() {
		ZeroMemory(&info, sizeof(STARTUPINFO));
		info.cb = sizeof(STARTUPINFO);
	}
};

void sendMessage(int type) {
	switch (type) {
	case 1:
		SetEvent(event1);
		break;
	case 2:
		SetEvent(event2);
		break;
	case 3:
		SetEvent(event3);
		break;
	case 4:
		SetEvent(event4);
	}
}

DWORD WINAPI waitEvent1(LPVOID v) {
	while (flag1) {
		WaitForSingleObject(parentEvent1, INFINITE);
		cout << "Получено сообение 1 от процесса Parent\n";
		messageParentEventCount++;
	}
	return 0;
}

DWORD WINAPI waitEvent2(LPVOID v) {
	while (flag2) {
		WaitForSingleObject(parentEvent2, INFINITE);
		cout << "Получено сообение 2 от процесса Parent\n";
		messageParentEventCount++;
	}
	return 0;
}

DWORD WINAPI waitParentEndEvent(LPVOID v) {
	while (flag3) {
		WaitForSingleObject(parentEndEvent, INFINITE);
		cout << "Процесс Parent завершил работу.\n";
		messageParentEndCount++;
	}
	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	event1 = CreateEvent(NULL, FALSE, FALSE, "Event1");
	event2 = CreateEvent(NULL, FALSE, FALSE, "Event2");
	event3 = CreateEvent(NULL, FALSE, FALSE, "Event3");
	event4 = CreateEvent(NULL, FALSE, FALSE, "Event4");
	parentEvent1 = CreateEvent(NULL, FALSE, FALSE, "ParentEvent1");
	parentEvent2 = CreateEvent(NULL, FALSE, FALSE, "ParentEvent2");
	parentEndEvent = CreateEvent(NULL, FALSE, FALSE, "ParentEndEvent");
	semaphore = CreateSemaphore(NULL, 3, 3, "Semaphore");

	cout << "Введите кол-во процессов \"Parent\" и \"Child\":\n";
	int processCount;
	cin >> processCount;
	system("cls");

	ProcessInformation** processInformation;
	processInformation = new ProcessInformation * [processCount];

	for (int i = 0; i < processCount * 2; i += 2) {
		processInformation[i] = new ProcessInformation();
		if (!CreateProcess(NULL, "Lab_04(Child).exe", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &processInformation[i]->info, &processInformation[i]->processInformation)) {
			cout << "Child not created\n";
		}

		processInformation[i + 1] = new ProcessInformation();
		if (!CreateProcess(NULL, "Lab_04(Parent).exe", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &processInformation[i + 1]->info, &processInformation[i + 1]->processInformation)) {
			cout << "Parent not created\n";
		}
	}

	HANDLE parentThread1;
	DWORD IDThread1;

	HANDLE parentThread2;
	DWORD IDThread2;

	HANDLE parentEndThread;
	DWORD IDThread3;

	parentThread1 = CreateThread(NULL, 0, waitEvent1, (void*)parentEvent1, 0, &IDThread1);
	parentThread2 = CreateThread(NULL, 0, waitEvent2, (void*)parentEvent2, 0, &IDThread2);
	parentEndThread = CreateThread(NULL, 0, waitParentEndEvent, (void*)parentEndEvent, 0, &IDThread3);

	for (int i = 0; i < processCount * 2; i++) {
		cout << "Введите сообщение (1-4):\n";
		int message;
		cin >> message;

		sendMessage(message);
	}

	while ((messageParentEventCount < processCount) || (messageParentEndCount < processCount));

	TerminateThread(parentThread1, 0);
	TerminateThread(parentThread2, 0);
	TerminateThread(parentEndThread, 0);

	CloseHandle(parentThread1);
	CloseHandle(parentThread2);
	CloseHandle(parentEndThread);

	system("pause");

	return 0;
}