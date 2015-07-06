
#define UNICODE
#define _UNICODE

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <process.h>

struct Operation{
	char date[100];
	float total;
};

TCHAR n[100];
TCHAR m[100];
TCHAR out[100];
int M,N;

HANDLE sems[100];

void createFiles();

DWORD WINAPI Thread(LPVOID param)
{
	int* id = ((int*)param);
	int acc = (*id) + 1;
	Operation op;
	DWORD nr;
	TCHAR name[100];
	TCHAR buf[100];
	char bufOut[100];
	char buf2[100];
	float total = 0;
	memset(bufOut, 0, 100);

	for (int i = 0; i < 2; i++){
		
		lstrcpy(name, _T("account"));
		_stprintf(buf, _T("%d"), acc);
		lstrcat(name, buf);
		lstrcat(name, _T("month"));
		_stprintf(buf, _T("%d"), i+1);
		lstrcat(name, buf);
		lstrcat(name, _T(".bin"));

		//_tprintf(_T("FILE: %s id: %d\n"),name,acc);
		
		HANDLE hr = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		while (ReadFile(hr, &op, sizeof(struct Operation), &nr, NULL) && nr > 0) {
			//printf("date: %s \ntotal: %f\n -----\n", op.date, op.total);
			total += op.total;
		}
		CloseHandle(hr);

		//printf("waiting for %d\n", acc - 1);
		WaitForSingleObject(sems[acc - 1], INFINITE);
		//printf("Signaled %d\n", acc - 1);
		DWORD nw;
		HANDLE hw = CreateFile(out, FILE_APPEND_DATA, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (acc == 1){
			strcpy(bufOut,("Month "));
			printf(("%s"), bufOut);
			WriteFile(hw, bufOut, strlen(bufOut), &nw, NULL);
			memset(bufOut, 0, 100);
		}
		sprintf(buf2, "%f ", total);
		strcat(bufOut, buf2);
		printf(("%s"), bufOut);
		WriteFile(hw, bufOut, strlen(bufOut), &nw, NULL);

		memset(bufOut, 0, 100);
		CloseHandle(hw);
		//printf("-------------------------\n", total);

		if (acc < N){
			ReleaseSemaphore(sems[acc], 1, 0);
		}if (acc == N){
			ReleaseSemaphore(sems[0], 1, 0);
		}

	}
	
	return 0;
}
int _tmain(int argc, LPTSTR argv[])
{
	//createFiles();
	HANDLE th[100];
	
	lstrcpy(n, argv[1]);
	lstrcpy(m, argv[2]);
	lstrcpy(out, argv[3]);

	N = _ttoi(n);
	M = _ttoi(m);

	
	for (int i = 0; i < N; i++){
		sems[i] = CreateSemaphore(NULL, 0, 1, NULL);
	}

	int id[100];
	ReleaseSemaphore(sems[0], 1, NULL);
	for (int i = 0; i < N; i++){
		id[i] = i ;
		th[i] = CreateThread(NULL, 0, Thread, &id[i], 0, NULL);
		
	}
	

	WaitForMultipleObjects(N-1, th, TRUE, INFINITE);

	for (int i = 0; i < N; i++){
		CloseHandle(th[i]);
		CloseHandle(sems[i]);
	}
	while (true){

	}
	return 0;
}

void createFiles(){
	DWORD nw = 0, nr;
	HANDLE hw = CreateFile(_T("account1month1.bin"), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	Operation op;

	op.total = 12345.50;
	strcpy(op.date, "01.01.2014");
	WriteFile(hw, &op, sizeof(struct Operation), &nw, NULL);

	op.total = -345.50;
	strcpy(op.date, "20.01.2014");
	WriteFile(hw, &op, sizeof(struct Operation), &nw, NULL);

	op.total = -2000.00;
	strcpy(op.date, "25.01.2014");
	WriteFile(hw, &op, sizeof(struct Operation), &nw, NULL);

	CloseHandle(hw);

	hw = CreateFile(_T("account1month2.bin"), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	op.total = 45.50;
	strcpy(op.date, "08.02.2014");
	WriteFile(hw, &op, sizeof(struct Operation), &nw, NULL);

	op.total = -5.50;
	strcpy(op.date, "20.02.2014");
	WriteFile(hw, &op, sizeof(struct Operation), &nw, NULL);

	op.total = 1000.00;
	strcpy(op.date, "23.02.2014");
	WriteFile(hw, &op, sizeof(struct Operation), &nw, NULL);

	CloseHandle(hw);

	hw = CreateFile(_T("account2month1.bin"), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	op.total = 250.00;
	strcpy(op.date, "11.01.2014");
	WriteFile(hw, &op, sizeof(struct Operation), &nw, NULL);

	op.total = 1250.50;
	strcpy(op.date, "20.01.2014");
	WriteFile(hw, &op, sizeof(struct Operation), &nw, NULL);

	CloseHandle(hw);

	hw = CreateFile(_T("account2month2.bin"), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	op.total = 2000.00;
	strcpy(op.date, "07.02.2014");
	WriteFile(hw, &op, sizeof(struct Operation), &nw, NULL);

	op.total = 2800.00;
	strcpy(op.date, "11.02.2014");
	WriteFile(hw, &op, sizeof(struct Operation), &nw, NULL);

	op.total = -4500.00;
	strcpy(op.date, "23.02.2014");
	WriteFile(hw, &op, sizeof(struct Operation), &nw, NULL);

	CloseHandle(hw);

}