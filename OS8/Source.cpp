#include <iostream>
#include <windows.h>
#include <time.h>

using namespace std;

class MonitorStack {
private:
    int nSize;
    int pos;
    __int16* arr;
public:

    MonitorStack(int nSize) {
        this->nSize = nSize;
        this->arr = new __int16[nSize];
        pos = 0;
    }
    MonitorStack() {
        this->nSize = 0;
        this->arr = nullptr;
        pos = 0;
    }
    ~MonitorStack() {
        delete[] arr;
    }

    void Push(__int16& nElement) {
        if (pos < nSize) {
            arr[pos++] = nElement;
        }
    }
    __int16 Pop() {
        if (pos > 0) {
            return arr[--pos];
        }
    }
};

struct TransmitedParametrs : public MonitorStack {
public:
    int count;

    HANDLE hAccess;
    HANDLE hFull;
    HANDLE hEmpty;

    TransmitedParametrs(int countProducer, int size, HANDLE hAccess, HANDLE hFull, HANDLE hEmpty) : MonitorStack(size)
    {
        this->hAccess = hAccess;
        this->hFull = hFull;
        this->hEmpty = hEmpty;

    }
};

void producer(LPVOID lpParam) {
    setlocale(LC_ALL, "Rus");
    
    TransmitedParametrs* param = (TransmitedParametrs*)lpParam;
    
    __int16 element = rand() % 100;

    WaitForSingleObject(param->hFull, INFINITE);
    WaitForSingleObject(param->hAccess, INFINITE);
    
    cout << "\nПроизвели элемент: " << element;
    param->Push(element);
    
    ReleaseSemaphore(param->hAccess, 1, NULL);
    ReleaseSemaphore(param->hEmpty, 1, NULL);
}

void consumer(LPVOID lpParam) {
    TransmitedParametrs* param = (TransmitedParametrs*)lpParam;

    WaitForSingleObject(param->hEmpty, INFINITE);
    WaitForSingleObject(param->hAccess, INFINITE);
    
    cout << "\nУпотребили элемент: " << param->Pop();
    
    ReleaseSemaphore(param->hAccess, 1, NULL);
    ReleaseSemaphore(param->hFull, 1, NULL);
}

int main()
{
    setlocale(LC_ALL, "rus");
    srand(time(NULL));

    cout << "Введите размер стека: ";
    int sizeStack;
    cin >> sizeStack;

    cout << "Введите количество потоков producer: ";
    int countProducer;
    cin >> countProducer;
    
    cout << "Введите количество потоков consumer: ";
    int countConsumer;
    cin >> countConsumer;

    HANDLE* hProducer = new HANDLE[countProducer];
    HANDLE* hConsumer = new HANDLE[countConsumer];
    
    DWORD IDProducer, IDConsumer;
    
    HANDLE hAccess = CreateSemaphore(NULL, 1, 1, L"Access");
    HANDLE hFull = CreateSemaphore(NULL, sizeStack, sizeStack, L"Full");
    HANDLE hEmpty = CreateSemaphore(NULL, 0, 1, L"Empty");

    MonitorStack* stack = new MonitorStack(sizeStack);

    TransmitedParametrs param(countProducer, sizeStack, hAccess, hFull, hEmpty);

    for (int i = 0; i < countProducer; i++) {
        param.count = 2;

        hProducer[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)producer, &param, 0, &IDProducer);
        if (hProducer[i] == NULL) {
            return GetLastError();
        }
    }

    for (int i = 0; i < countConsumer; i++) {
        param.count = 2;

        hConsumer[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)consumer, &param, 0, &IDConsumer);
        if (hConsumer[i] == NULL) {
            return GetLastError();
        }
    }

    for (int i = 0; i < countProducer; i++) {
        WaitForSingleObject(hProducer[i], INFINITE);
    }
    for (int i = 0; i < countConsumer; i++) {
        WaitForSingleObject(hConsumer[i], INFINITE);
    }

    CloseHandle(hAccess);
    CloseHandle(hFull);
    CloseHandle(hEmpty);

    for (int i = 0; i < countProducer; i++) {
        CloseHandle(hProducer[i]);
    }
    for (int i = 0; i < countConsumer; i++) {
        CloseHandle(hConsumer[i]);
    }
    return 0;
}
