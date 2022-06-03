#include <windows.h>
#include <iostream>

using namespace std;

struct Data
{
	int* arr;
	int size;

	Data(int s, int* a)
	{
		size = s;
		arr = new int[size];
		for (int i = 0; i < size; i++)
		{
			arr[i] = a[i];
		}
	}

	~Data()
	{
		delete arr;
	}
};

DWORD WINAPI worker(LPVOID v)
{
	Data* data = (Data*)v;
	int sum = 0;

	for (int i = 0; i < data->size; i++)
	{
		sum += data->arr[i] * data->arr[i];
	}
	cout << sum << endl;
	return 0;
}

DWORD WINAPI fib(LPVOID v)
{
	int num1 = 1;
	int num2 = 1;

	do
	{
		cout << num2 << " ";

		int tmp = num2;
		num2 += num1;
		num1 = tmp;

		Sleep(200);
	} while (true);

	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	cout << "Введите размер массива:\n";
	int size;
	cin >> size;

	cout << "Введите " << size << " чисел:\n";
	int* arr = new int[size];
	for (int i = 0; i < size; i++)
	{
		cin >> arr[i];
	}

	cout << "Введите задержку:\n";
	int time;
	cin >> time;

	HANDLE taskThread;
	DWORD IDThread1;

	taskThread = CreateThread(NULL, 0, worker, (void*)new Data(size, arr), 0, &IDThread1);

	SuspendThread(taskThread);
	Sleep(time);
	ResumeThread(taskThread);

	WaitForSingleObject(taskThread, INFINITE);

	HANDLE thread;
	DWORD IDThread2;

	thread = CreateThread(NULL, 0, fib, NULL, 0, &IDThread2);

	WaitForSingleObject(thread, INFINITE);
	SuspendThread(thread);

	CloseHandle(taskThread);
	delete arr;

	system("pause");

	return 0;
}