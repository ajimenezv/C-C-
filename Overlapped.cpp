
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
	LARGE_INTEGER filePos,fSize;
	OVERLAPPED ov = { 0, 0, 0, 0, NULL };

	hr = CreateFile(argv[1], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	fSize.QuadPart = sizeof(struct Student);
	TCHAR opc[3];


	do{
		_tprintf(_T("Select an option:\n R->Read.\n W->Write.\n E->Exit.\n"));
		_tscanf(_T("%s"), opc);

		if ((_tcscmp(opc, _T("R")) == 0)){
			_tprintf(_T("Write the number of the record.\n"));
			_tscanf(_T("%d"), &num);
			
			LockFile(hr, ov.Offset, ov.OffsetHigh, fSize.LowPart, fSize.HighPart);

			filePos.QuadPart = (num - 1) * sizeof (struct Student);
			ov.Offset = filePos.LowPart;
			ov.OffsetHigh = filePos.HighPart;
			ReadFile(hr, &s, sizeof (struct Student), &nR, &ov);

			UnlockFile(hr, ov.Offset, ov.OffsetHigh, fSize.LowPart, fSize.HighPart);

			_tprintf(_T("%d %ld %s %s %d\n"), s.id, s.reg, s.surname, s.name, s.mark);
		}

		if ((_tcscmp(opc, _T("W")) == 0)){
			_tprintf(_T("Write the number of the record.\n"));
			_tscanf(_T("%d"), &num);
			_tprintf(_T("Change the info.\n"));
			_tscanf(_T("%d %ld %s %s %d"), &s.id, &s.reg, s.surname, s.name, &s.mark);

			LockFile(hr, ov.Offset, ov.OffsetHigh, fSize.LowPart, fSize.HighPart);

			filePos.QuadPart = (num - 1) * sizeof (struct Student);
			ov.Offset = filePos.LowPart;
			ov.OffsetHigh = filePos.HighPart;
			WriteFile(hr, &s, sizeof(struct Student), &nR, &ov);

			UnlockFile(hr, ov.Offset, ov.OffsetHigh, fSize.LowPart, fSize.HighPart);
		}

		if (_tcscmp(opc, _T("E")) == 0){
			CloseHandle(hr);
		}
		
	}while (_tcscmp(opc, _T("E")) != 0);

	return 0;
}