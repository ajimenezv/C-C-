
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <io.h>
#include <time.h>

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_DOT 3

HANDLE eve;

DWORD nums[100];
TCHAR fileN[100];
int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep);
DWORD WINAPI readThread(LPVOID param);
DWORD WINAPI sortThread(LPVOID param);

int _tmain(int argc, LPTSTR argv[])
{/*
	DWORD num;
	srand(time(NULL));
	nums[0] = 10;
	for (int i = 1; i < 10; i++){
		nums[i] = rand() % 100 + 1;
	}
	*/
	strcpy(fileN, argv[1]);
	HANDLE th[5];

	
	th[0]=CreateThread(NULL, 0, readThread, NULL, 0, NULL);
	th[1] = CreateThread(NULL, 0, sortThread, NULL, 0, NULL);

	WaitForMultipleObjects(2,th, TRUE, INFINITE);
	
	CloseHandle(th[0]);
	CloseHandle(th[1]);

	
	return 0;

}

DWORD WINAPI readThread(LPVOID param){
	DWORD wF = 0, rF = 0;
	HANDLE hr=NULL;
	
	DWORD aux = 0;
	__try{
		__try{

			DWORD tot = 0, num1 = 0, num2 = 0;
			hr = CreateFile(fileN, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hr == INVALID_HANDLE_VALUE)
			{
				_tprintf(_T("File not Found!\n"));
				exit(0);
			}
			while (ReadFile(hr, &num1, sizeof (DWORD), &rF, NULL) && rF > 0){
				if (ReadFile(hr, &num2, sizeof (DWORD), &rF, NULL) == FALSE && rF==0){
					_tprintf(_T("%d\n"), num1);
					break;
				}
				DWORD a = num1, b = num2;
				while (a != 0 && b != 0)
				{
					if (a > b)
						a %= b;
					else
						b %= a;
				}
				if (a == 0)
					_tprintf(_T("%d,%d %d/%d\n"), num1, num2, num1 / b, num2 / b);
				else
					_tprintf(_T("%d,%d %d/%d\n"), num1, num2, num1 / a, num2 / a);

			}
		}
		__finally{
			CloseHandle(hr);

		}
	}__except(filter(GetExceptionCode(),GetExceptionInformation()))
	{
		exit(0);
	}
	return 0;
}


DWORD WINAPI sortThread(LPVOID param){
	DWORD res;
	
	DWORD wF = 0, rF = 0;
	HANDLE hr;
	hr = CreateFile(fileN, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hr == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File not Found!\n"));
		exit(0);
	}
	DWORD tot = 0, num1 = 0, num2 = 0;

	ReadFile(hr, &tot, sizeof (DWORD), &rF, NULL);
	DWORD num[1000];
	for (int j = 0; j < tot; j++){
		ReadFile(hr, &num1, sizeof (DWORD), &rF, NULL);
		num[j] = num1;
	}
	CloseHandle(hr);

	int i, j, flag = 1;
	DWORD temp;
	for (i = 1; (i <= tot) && flag; i++)
	{
		flag = 0;
		for (j = 0; j < (tot - 1); j++)
		{
			if (num[j + 1] > num[j])   
			{
				temp = num[j];         
				num[j] = num[j + 1];
				num[j + 1] = temp;
				flag = 1;              
			}
		}
	}


	for (int j = 0; j < tot; j++){
		_tprintf(_T("%d\n"), num[j]);
	}
	
	return 0;
}


int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {
	
	if (code == EXCEPTION_ACCESS_VIOLATION) {
		_tprintf(_T("Access Violation error!."));
		return EXCEPTION_EXECUTE_HANDLER;
	}
	if (code == EXCEPTION_INT_DIVIDE_BY_ZERO) {
		_tprintf(_T("Division by zero error!."));
		return EXCEPTION_EXECUTE_HANDLER;
	}
	if (code == EXCEPTION_ARRAY_BOUNDS_EXCEEDED) {
		_tprintf(_T("Array out of bounds error!."));
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else {
		_tprintf(_T("didn't catch AV, unexpected."));
		return EXCEPTION_CONTINUE_SEARCH;
	};
}