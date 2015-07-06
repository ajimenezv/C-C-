
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <io.h>

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_DOT 3

struct Dirs{
	TCHAR pathName[100];
	TCHAR newPathName[100];
};

TCHAR compa[100];
HANDLE rd;
HANDLE cmp;
bool swF;

static bool SearchDirectory(TCHAR* pathName);
static bool TraverseDirectory(TCHAR* pathName, TCHAR* newPathName);
DWORD FileType(LPWIN32_FIND_DATA);
DWORD WINAPI readThread(LPVOID param);
DWORD WINAPI compThread(LPVOID param);

int _tmain(int argc, LPTSTR argv[])
{

	HANDLE th[100];
	HANDLE cmpTh;
	Dirs dir[100];
	Dirs d;
	
	rd = CreateSemaphore(NULL, 0, 1, NULL);
	cmp = CreateSemaphore(NULL, 0, 1, NULL);
	
	int i;
	for (i = 0; i < argc - 2; i++){
		
		lstrcpy(dir[i].pathName, argv[i+2]);
		//_tprintf(_T("DIR: %s\n"), dir[i].pathName);
		//SearchDirectory(dir[i].pathName);

		th[i] = CreateThread(NULL, 0, readThread, &dir[i].pathName, 0, NULL);
	}
	lstrcpy(d.pathName, argv[1]);
	cmpTh = CreateThread(NULL, 0, compThread, &d.pathName, 0, NULL);

	ReleaseSemaphore(rd, 1, NULL);
	
	WaitForMultipleObjects(argc-2, th, TRUE, INFINITE);
	WaitForSingleObject(cmpTh, INFINITE);
	
	for (int i = 1; i < argc - 2; i++){
		CloseHandle(th[i]);
	}
	CloseHandle(cmpTh);
	CloseHandle(rd);
	CloseHandle(cmp);
	return 0;
	
}

DWORD WINAPI readThread(LPVOID param){
	TCHAR* dir=((TCHAR*)param);
	swF = false;
	//_tprintf(_T("->Hi read\n"));
	WaitForSingleObject(rd, INFINITE);
	SearchDirectory(dir);
	//_tprintf(_T("->Bye read\n"));
	if (swF == true){
		_tprintf(_T("->Equal Directories!\n"));
		ReleaseSemaphore(rd, 1, NULL);
	}
	else{
		_tprintf(_T("->BAAAAAAAAAAD!\n"));
	}
	return 0;
}

DWORD WINAPI compThread(LPVOID param){
	TCHAR* dir = ((TCHAR*)param);
	while (true){
		//_tprintf(_T("->Hi comp\n"));
		WaitForSingleObject(cmp, INFINITE);
		//_tprintf(_T("->%s\n"),dir);
		if(TraverseDirectory(dir, compa)==true){
			//_tprintf(_T("->GOOOOOD!\n"));
			swF = true;
		}
		else{
			swF = false;
			_tprintf(_T("->Different Directories!\n"));
			exit(0);
			
		}
		//_tprintf(_T("->Bye comp\n"));
		ReleaseSemaphore(rd, 1, NULL);
	}

	return 0;
}


static bool SearchDirectory(TCHAR *pathName){
	HANDLE SearchHandle;

	WIN32_FIND_DATA FindData;
	DWORD FType;
	TCHAR cpy[100];
	lstrcpy(cpy,pathName);
	bool sw = false;
	//_tprintf(_T("\n---> Plain Run on %s:\n"),cpy);

	SetCurrentDirectory(pathName);
	SearchHandle = FindFirstFile(_T("*"), &FindData);
	do {
		FType = FileType(&FindData);
		if (FType == TYPE_FILE){
				//_tprintf(_T("FILE: %s\n"),FindData.cFileName);
				strcpy(compa, FindData.cFileName);
				ReleaseSemaphore(cmp, 1, NULL);
				WaitForSingleObject(rd, INFINITE);
			
		}
		if (FType == TYPE_DIR) {

			//_tprintf(_T("DIR : %s\n"), FindData.cFileName);
			strcpy(compa, FindData.cFileName);
			ReleaseSemaphore(cmp, 1, NULL);
			WaitForSingleObject(rd, INFINITE);
			lstrcat(cpy, "\\");
			lstrcat(cpy, FindData.cFileName);
		
			if (SearchDirectory(cpy) == true){
				sw = true;
			}
			lstrcpy(cpy,pathName);

		}
	} while (FindNextFile(SearchHandle, &FindData));

	FindClose(SearchHandle);

	if (sw == true){
		return sw;
	}
	else{
		return false;
	}
}

static bool TraverseDirectory(TCHAR* pathName,TCHAR* newPathName) {
	HANDLE SearchHandle;
	WIN32_FIND_DATA FindData;
	DWORD FType;
	TCHAR cpy[100];
	lstrcpy(cpy, pathName);
	bool sw = false;
	//_tprintf(_T("\n---> Compare %s:\n"),newPathName);

	SetCurrentDirectory(pathName);
	SearchHandle = FindFirstFile(_T("*"), &FindData);
	do {
		FType = FileType(&FindData);
		if (FType == TYPE_FILE){
			//	_tprintf(_T("FILE: %s\n"),FindData.cFileName);
				if (strcmp(newPathName, FindData.cFileName) == 0){
					sw= true;
				}
		}
		if (FType == TYPE_DIR) {

			//_tprintf(_T("DIR : %s\n"), FindData.cFileName);
			if (strcmp(newPathName, FindData.cFileName) == 0){
				sw = true;
			}
			lstrcat(cpy, "\\");
			lstrcat(cpy, FindData.cFileName);

	
			if (TraverseDirectory(cpy,newPathName) == true){
				sw = true;
			}
			lstrcpy(cpy, pathName);

		}
	} while (FindNextFile(SearchHandle, &FindData));

	FindClose(SearchHandle);
	if (sw == true){
		return true;
	}
	else{
		return false;
	}
}
static DWORD FileType(LPWIN32_FIND_DATA pFileData) {
	BOOL IsDir;
	DWORD FType;
	FType = TYPE_FILE;
	IsDir = (pFileData->dwFileAttributes &
		FILE_ATTRIBUTE_DIRECTORY) != 0;
	if (IsDir)
	if (lstrcmp(pFileData->cFileName, _T(".")) == 0
		|| lstrcmp(pFileData->cFileName, _T("..")) == 0)
		FType = TYPE_DOT;
	else FType = TYPE_DIR;
	return FType;
}


