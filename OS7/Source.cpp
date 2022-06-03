#include <windows.h> 
#include <ctime> 
#include <stdlib.h> 
#include <iostream> 
using namespace std;

__int16* stack;
HANDLE SemaphoreAdd;
HANDLE SemaphoreRemove;
CRITICAL_SECTION cs;

class MonitorStack {
private:
	int maxSize, currentSize;
public:
	MonitorStack() {
		maxSize = 0;
		currentSize = 0;
	}

	MonitorStack(int nSize) {
		stack = new __int16[nSize];
		maxSize = nSize;
		currentSize = 0;
	};

	void Push(__int16& nElement) {
		WaitForSingleObject(SemaphoreAdd, INFINITE);
		stack[currentSize] = nElement;
		currentSize++;
		ReleaseSemaphore(SemaphoreRemove, 1, NULL);
	};

	__int16 Pop() {
		WaitForSingleObject(SemaphoreRemove, INFINITE);
		__int16 elem = stack[currentSize - 1];
		currentSize--;
		ReleaseSemaphore(SemaphoreAdd, 1, NULL);
		return elem;
	};
};

MonitorStack monitor;

DWORD WINAPI consume(LPVOID count) {
	int cnt = (int)count;
	for (int i = 0; i < cnt; i++) {
		unsigned int obj = monitor.Pop();
		EnterCriticalSection(&cs);
		cout << "Consumed object " << obj << endl;
		LeaveCriticalSection(&cs);
		Sleep(500);
	}
	return 0;
}

DWORD WINAPI produce(LPVOID count) {
	int cnt = (int)count;
	for (int i = 0; i < cnt; i++) {
		__int16 obj = (__int16)rand() % 100;
		EnterCriticalSection(&cs);
		cout << "Produced object " << obj << endl;
		LeaveCriticalSection(&cs);
		monitor.Push(obj);
		Sleep(500);
	}
	return 0;
}

int main() {
	int consumers, producers, size;
	
	int* consumed;
	int* produced;

	HANDLE* handles;
	DWORD* consumersID;
	DWORD* producersID;
	
	cout << "Input number of consumers: " << endl;
	cin >> consumers;
	
	consumed = new int[consumers];
	
	cout << "Input number of producers: " << endl;
	cin >> producers;
	
	produced = new int[producers];
	
	cout << "Input size of stack: " << endl;
	cin >> size;
	
	monitor = MonitorStack(size);
	
	SemaphoreAdd = CreateSemaphore(NULL, size, size, NULL);
	SemaphoreRemove = CreateSemaphore(NULL, 0, size, NULL);
	
	InitializeCriticalSection(&cs);
	
	handles = new HANDLE[consumers + producers];
	consumersID = new DWORD[consumers];
	producersID = new DWORD[producers];
	
	int pos = 0;
	for (int i = 0; i < consumers; i++) {
		cout << "Input number of consumed objects " << endl;
		cin >> consumed[i];
	}
	
	for (int i = 0; i < producers; i++) {
		cout << "Input number of produced objects " << endl;
		cin >> produced[i];
	}
	
	srand(time(0));
	
	for (int i = 0; i < producers; i++, pos++)
		handles[i] = CreateThread(NULL, 0, produce, (int*)produced[i], 0, &producersID[i]);
	
	for (int i = 0; i < consumers; i++, pos++)
		handles[pos] = CreateThread(NULL, 0, consume, (int*)consumed[i], 0, &consumersID[i]);
	
	WaitForMultipleObjects(consumers + producers, handles, TRUE, INFINITE);
	
	cout << "Main finished" << endl;

	return 0;
};