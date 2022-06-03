#include <iostream>
#include <Windows.h>

CRITICAL_SECTION cs;
HANDLE hEvent[2];
int size, k, A, mult;

void work(LPVOID arrays) {
    int time;
    double* arr = (double*)arrays;
    std::cout << "Введите временной интервал: " << std::endl;
    std::cin >> time;

    int j = 0;
    int temp = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] < A) {
            j = i + 1;
            while (j < size && arr[j] < A) {
                j++;
            }

            if (j == size) {
                i = size;
            }
            else {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        Sleep(time);
    }

    SetEvent(hEvent[0]);
}

void multElement(LPVOID arrays) {
    EnterCriticalSection(&cs);
    WaitForSingleObject(hEvent[1], INFINITE);
    double* arr = (double*)arrays;
    mult = 1;

    for (int i = k; i < size; i++) {
        mult *= arr[i];
    }
    LeaveCriticalSection(&cs);
}


int main()
{
    setlocale(LC_ALL, "rus");

    HANDLE hThread[2];
    DWORD  IDThread[2];

    InitializeCriticalSection(&cs);
    hEvent[0] = CreateEventA(NULL, FALSE, FALSE, NULL);
    hEvent[1] = CreateEventA(NULL, FALSE, FALSE, NULL);
    if (hEvent[0] == NULL || hEvent[1] == NULL) {
        return GetLastError();
    }

    double* arrays;
    std::cout << "Введите размерность: " << std::endl;
    std::cin >> size;
    arrays = new double[size];

    std::cout << "Введите элементы массива: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cin >> arrays[i];
    }

    std::cout << "Размерность: " << size << std::endl; 
    std::cout << "Элементы массива: ";
    for (int i = 0; i < size; i++) {
        std::cout << arrays[i] << " ";
    }
    std::cout << std::endl;


    hThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)work, (void*)arrays, CREATE_SUSPENDED, &IDThread[0]);
    if (hThread[0] == NULL) {
        return GetLastError();
    }


    std::cout << "Введите число k: " << std::endl;
    std::cin >> k;
    if (k > size) {
        k = 1;
    }

    std::cout << "Введите число A: " << std::endl;
    std::cin >> A;
    if (A > size) {
        A = 1;
    }

    ResumeThread(hThread[0]);
    hThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)multElement, (void*)arrays, 0, &IDThread[1]);
    if (hThread[1] == NULL) {
        return GetLastError();
    }

    WaitForSingleObject(hEvent[0], INFINITE);
    std::cout << "Итоговый массив: ";
    for (int i = 0; i < k; i++) {
        std::cout << arrays[i] << " ";
    }

    SetEvent(hEvent[1]);
    TryEnterCriticalSection(&cs);

    for (int i = k; i < size; i++) {
        std::cout << arrays[i] << " ";
    }

    std::cout << std::endl;
    std::cout << "Произведение = " << mult << std::endl;

    LeaveCriticalSection(&cs);
   
    DeleteCriticalSection(&cs);
   
    CloseHandle(hThread[0]); 
    CloseHandle(hThread[1]);
    CloseHandle(hEvent[0]);
    CloseHandle(hEvent[1]);

    delete[] arrays;

    return 0;
}
