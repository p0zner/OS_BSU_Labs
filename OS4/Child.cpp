#include <windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

volatile bool flag = true;

void getMessage(int num) {
	switch (num) {
	case 1:
		cout << "Получено сообщение 1 в процессе Parent\n";
		break;
	case 2:
		cout << "Получено сообщение 2 в процессе Parent\n";
		break;
	case 3:
		cout << "Получено сообщение 3 в процессе Parent\n";
		break;
	case 4:
		cout << "Получено сообщение 4 в процессе Parent\n";
		break;

	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	cout << "Child process\n";

	HANDLE* handleArray = new HANDLE[7];
	handleArray[0] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event1");
	handleArray[1] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event2");
	handleArray[2] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event3");
	handleArray[3] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event4");
	HANDLE semaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, "Semaphore");

	cout << "Не активен\n";
	WaitForSingleObject(semaphore, INFINITE);
	system("cls");
	cout << "Child process\n";
	cout << "Активен\n";

	int pos = WaitForMultipleObjects(4, handleArray, FALSE, INFINITE);

	getMessage(pos + 1);

	ReleaseSemaphore(semaphore, 1, NULL);
	system("pause");
	return 1;
}