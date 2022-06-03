#include <windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	int size = atoi(argv[1]);
	int symbol = atoi(argv[2]);
	char* arr = argv[3];
	int* newArr = new int[size];

	int sum = 1;

	for (int i = 0; i < symbol; i++)
	{ 
		newArr[i] = arr[i] - '0';
		if (newArr[i] != 0) {
			sum = sum * newArr[i];
		}
	}

	for (int i = 0; i < symbol; i++) {
		cout << newArr[i] << " ";
	}

	cout << endl << sum << endl;

	system("pause");
	return 1;
}