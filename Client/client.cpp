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
		printf("%s (%s, %s, %s): ", "Введите комманду", "1 - модификация", "2 - чтение", "3 - выход");
		scanf("%d", &command);
		switch (command) {
			case 1:
				printf("%s: ", "Введите номер зачётки");
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
				printf("Номер зачётки: %d\nИмя: %s\nСредний балл: %lf\n", student.num, student.name, student.grade);
				printf("%s: ", "Введите новый номер зачётки");
				scanf("%d", &student.num);
				printf("%s: ", "Введите имя нового студента");
				scanf("%s", student.name);
				printf("%s: ", "Введите средний балл нового студента");
				scanf("%lf", &student.grade);
				printf("%s: ", "Вы хотите отправить изменения?(1 - Да,0 - Нет)");
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
				printf("%s: ", "Введите номер зачётки");
				scanf("%d", &num);
				WriteFile(hWritePipe, &command, sizeof(int), &dwBytesWrite, NULL);
				hServerEnableRead = OpenEvent(EVENT_MODIFY_STATE, FALSE, "ServerEnableRead");
				SetEvent(hServerEnableRead);
				WriteFile(hWritePipe, &num, sizeof(int), &dwBytesWrite, NULL);
				hServerEnableRead = OpenEvent(EVENT_MODIFY_STATE, FALSE, "ServerEnableRead");
				SetEvent(hServerEnableRead);
				WaitForSingleObject(hClientEnableRead, INFINITE);
				ReadFile(hReadPipe, &student, sizeof(Student), &dwBytesRead, NULL);
				printf("Номер зачётки: %d\nИмя: %s\nСредний балл: %lf\n", student.num, student.name, student.grade);
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