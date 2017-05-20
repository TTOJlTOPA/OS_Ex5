#include <iostream>
#include <fstream>
#include <Windows.h>
#include "student.h"
using namespace std;

ifstream fileCreateIn(char* fileName);
ofstream fileCreateOut(char* fileName);

int main() 
{
	setlocale(LC_ALL, "rus");

	char lpszAppName[] = "Client.exe";
	char* lpszCommandLine = new char[1000];
	char* fileName = new char[260];
	int studentsNum;
	int num;
	int command;
	int index;
	bool flag = true;
	char name[10];
	double grade;
	Student student;
	Student* students;
	ifstream fin;
	ofstream fout;

	HANDLE hReadPipe;
	HANDLE hInheritReadPipe;
	HANDLE hWritePipe;
	HANDLE hInheritWritePipe;
	HANDLE hServerEnableRead;
	HANDLE hClientEnableRead;
	DWORD dwBytesWrite;
	DWORD dwBytesRead;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	printf("%s: ", "Введите имя файла");
	scanf("%s", fileName);
	fout = fileCreateOut(fileName);

	CreatePipe(&hReadPipe, &hWritePipe, NULL, 0);
	DuplicateHandle(GetCurrentProcess(), hWritePipe, GetCurrentProcess(), &hInheritWritePipe, 0, TRUE, DUPLICATE_SAME_ACCESS);
	DuplicateHandle(GetCurrentProcess(), hReadPipe, GetCurrentProcess(), &hInheritReadPipe, 0, TRUE, DUPLICATE_SAME_ACCESS);

	printf("%s: ", "Введите количество студентов");
	scanf("%d", &studentsNum);
	fout << studentsNum;
	students = new Student[studentsNum];
	for (int i = 0; i < studentsNum; i++)
	{
		printf("%s: ", "Введите номер зачётки");
		scanf("%d", &(students[i].num));
		printf("%s: ", "Введите имя");
		scanf("%s", students[i].name);
		printf("%s: ", "Введите средний балл");
		scanf("%lf", &(students[i].grade));
		fout.write((char*)&students[i], sizeof(Student));
	}
	fout.close();

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	sprintf(lpszCommandLine, "%s %d %d", lpszAppName, (int)hInheritReadPipe, (int)hInheritWritePipe);
	hServerEnableRead = CreateEvent(NULL, NULL, FALSE, "ServerEnableRead");
	CreateProcess(NULL, lpszCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	while (flag) {
		WaitForSingleObject(hServerEnableRead, INFINITE);
		ReadFile(hReadPipe, &command, sizeof(int), &dwBytesRead, NULL);
		switch (command) {
		case 1:
			WaitForSingleObject(hServerEnableRead, INFINITE);
			ReadFile(hReadPipe, &students[index], sizeof(Student), &dwBytesRead, NULL);
			fout = fileCreateOut(fileName);
			fout << studentsNum;
			for (int i = 0; i < studentsNum; i++) {
				fout.write((char*)&students[i], sizeof(Student));
			}
			fout.close();
			break;
		case 2:
			WaitForSingleObject(hServerEnableRead, INFINITE);
			ReadFile(hReadPipe, &num, sizeof(int), &dwBytesRead, NULL);
			fin = fileCreateIn(fileName);
			fin >> studentsNum;
			students = new Student[studentsNum];
			for (int i = 0; i < studentsNum; i++)
			{
				fin.read((char*)&students[i], sizeof(Student));
				if (students[i].num == num) {
					WriteFile(hWritePipe, &students[i], sizeof(Student), &dwBytesWrite, NULL);
					index = i;
				}
			}
			fin.close();
			hClientEnableRead = OpenEvent(EVENT_MODIFY_STATE, FALSE, "ClientEnableRead");
			SetEvent(hClientEnableRead);
			break;
		default:
			printf("%s:\n", "Итоговый файл");
			fin = fileCreateIn(fileName);
			fin >> studentsNum;
			students = new Student[studentsNum];
			for (int i = 0; i < studentsNum; i++)
			{
				fin.read((char*)&students[i], sizeof(Student));
				printf("Номер зачётки: %d\nИмя студента: %s\nСредний балл: %lf\n", students[i].num, students[i].name, students[i].grade);
			}
			fin.close();
			flag = false;
			break;
		}
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(hServerEnableRead);
	CloseHandle(hClientEnableRead);
	CloseHandle(hInheritReadPipe);
	CloseHandle(hInheritWritePipe);

	return 0;
}