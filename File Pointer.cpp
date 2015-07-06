
#define UNICODE
#define _UNICODE

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <windows.h>
#include <tchar.h>
#include <stdio.h>



struct Student{
	int id;
	int reg;
	TCHAR surname[31];
	TCHAR name[31];
	int mark;
};

int _tmain(int argc, LPTSTR argv[])
{
	
	Student s;
	HANDLE hr,hw;
	DWORD nR = 0, nW=0, num;
	LONG filePos;
	OVERLAPPED ov = { 0, 0, 0, 0, NULL };

	hr = CreateFile(argv[1], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	
	TCHAR opc[3];


	do{
		_tprintf(_T("Select an option:\n R->Read.\n W->Write.\n E->Exit.\n"));
		_tscanf(_T("%s"), opc);

		if ((_tcscmp(opc, _T("R")) == 0)){
			_tprintf(_T("Write the number of the record.\n"));
			_tscanf(_T("%d"), &num);
			filePos = (num - 1) * sizeof (struct Student);
			SetFilePointer(hr,filePos,NULL,FILE_BEGIN);
			ReadFile(hr, &s, sizeof (struct Student), &nR, NULL);
			_tprintf(_T("%d %ld %s %s %d\n"), s.id, s.reg, s.surname, s.name, s.mark);
		}

		if ((_tcscmp(opc, _T("W")) == 0)){
			_tprintf(_T("Write the number of the record.\n"));
			_tscanf(_T("%d"), &num);
			_tprintf(_T("Change the info.\n"));
			_tscanf(_T("%d %ld %s %s %d"), &s.id, &s.reg, s.surname, s.name, &s.mark);

			filePos = (num - 1) * sizeof (struct Student);
			SetFilePointer(hr, filePos, NULL, FILE_BEGIN);
			WriteFile(hr, &s, sizeof(struct Student), &nR, NULL);
		}

		if (_tcscmp(opc, _T("E")) == 0){
			CloseHandle(hr);
		}
		
	}while (_tcscmp(opc, _T("E")) != 0);

	return 0;
}