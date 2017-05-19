#include <iostream>
#include <Windows.h>
#include "student.h"
using namespace std;

int main(int args, char *argv[]) {
	setlocale(LC_ALL, "rus");

	int command;
	int num;
	int answer;
	bool flag = true;
	Student student;

	HANDLE hReadPipe;
	HANDLE hWritePipe;
	HANDLE hServerEnableRead;
	HANDLE hClientEnableRead;
	DWORD dwBytesWrite;
	DWORD dwBytesRead;

	hClientEnableRead = CreateEvent(NULL, NULL, FALSE, "ClientEnableRead");
	hReadPipe = (HANDLE)atoi(argv[1]);
	hWritePipe = (HANDLE)atoi(argv[2]);

	while (flag)
	{
		printf("%s (%s, %s, %s): ", "������� ��������", "1 - �����������", "2 - ������", "3 - �����");
		scanf("%d", &command);
		switch (command) {
			case 1:
				printf("%s: ", "������� ����� �������");
				scanf("%d", &num);
				++command;
				WriteFile(hWritePipe, &command, sizeof(int), &dwBytesWrite, NULL);
				hServerEnableRead = OpenEvent(EVENT_MODIFY_STATE, FALSE, "ServerEnableRead");
				SetEvent(hServerEnableRead);
				WriteFile(hWritePipe, &num, sizeof(int), &dwBytesWrite, NULL);
				hServerEnableRead = OpenEvent(EVENT_MODIFY_STATE, FALSE, "ServerEnableRead");
				SetEvent(hServerEnableRead);
				WaitForSingleObject(hClientEnableRead, INFINITE);
				ReadFile(hReadPipe, &student, sizeof(Student), &dwBytesRead, NULL);
				printf("����� �������: %d\n���: %s\n������� ����: %lf\n", student.num, student.name, student.grade);
				printf("%s: ", "������� ����� ����� �������");
				scanf("%d", &student.num);
				printf("%s: ", "������� ��� ������ ��������");
				scanf("%s", student.name);
				printf("%s: ", "������� ������� ���� ������ ��������");
				scanf("%lf", &student.grade);
				printf("%s: ", "�� ������ ��������� ���������?(1 - ��,0 - ���)");
				scanf("%d", &answer);
				if (answer == 1)
				{
					--command;
					WriteFile(hWritePipe, &command, sizeof(int), &dwBytesWrite, NULL);
					hServerEnableRead = OpenEvent(EVENT_MODIFY_STATE, FALSE, "ServerEnableRead");
					SetEvent(hServerEnableRead);
					WriteFile(hWritePipe, &student, sizeof(Student), &dwBytesWrite, NULL);
					hServerEnableRead = OpenEvent(EVENT_MODIFY_STATE, FALSE, "ServerEnableRead");
					SetEvent(hServerEnableRead);
				}
				break;
			case 2:
				printf("%s: ", "������� ����� �������");
				scanf("%d", &num);
				WriteFile(hWritePipe, &command, sizeof(int), &dwBytesWrite, NULL);
				hServerEnableRead = OpenEvent(EVENT_MODIFY_STATE, FALSE, "ServerEnableRead");
				SetEvent(hServerEnableRead);
				WriteFile(hWritePipe, &num, sizeof(int), &dwBytesWrite, NULL);
				hServerEnableRead = OpenEvent(EVENT_MODIFY_STATE, FALSE, "ServerEnableRead");
				SetEvent(hServerEnableRead);
				WaitForSingleObject(hClientEnableRead, INFINITE);
				ReadFile(hReadPipe, &student, sizeof(Student), &dwBytesRead, NULL);
				printf("����� �������: %d\n���: %s\n������� ����: %lf\n", student.num, student.name, student.grade);
				break;
			default:
				flag = false;
				break;
		}
	}

	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(hServerEnableRead);
	CloseHandle(hClientEnableRead);

	return 0;
}