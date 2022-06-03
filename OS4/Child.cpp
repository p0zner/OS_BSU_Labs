#include <windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

volatile bool flag = true;

void getMessage(int num) {
	switch (num) {
	case 1:
		cout << "�������� ��������� 1 � �������� Parent\n";
		break;
	case 2:
		cout << "�������� ��������� 2 � �������� Parent\n";
		break;
	case 3:
		cout << "�������� ��������� 3 � �������� Parent\n";
		break;
	case 4:
		cout << "�������� ��������� 4 � �������� Parent\n";
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

	cout << "�� �������\n";
	WaitForSingleObject(semaphore, INFINITE);
	system("cls");
	cout << "Child process\n";
	cout << "�������\n";

	int pos = WaitForMultipleObjects(4, handleArray, FALSE, INFINITE);

	getMessage(pos + 1);

	ReleaseSemaphore(semaphore, 1, NULL);
	system("pause");
	return 1;
}