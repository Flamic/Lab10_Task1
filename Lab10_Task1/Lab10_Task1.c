/*
	2. Відсортувати дані за прізвищами студентів в порядку протилежному алфавітному.
	Організувати послідовність структур в однозв’язний список. Реалізувати операцію
	вставки нового елемента у відсортований список і операцію вилучення зі списку
	даних про студентів з оцінками 2. Визначити п’ять студентів з найнижчим середнім
	рейтинговим балом.
*/

#include "stdafx.h"
#include "student.h"
#include <string.h>
#include <stdlib.h>

#define CRT_SECURE_NO_WARNINGS
#define COMMAND_LENGTH 20
#define COUNT_OF_THE_LAST 5
#define FILE_PATH "students.dat"

int main()
{
	SList *list = NULL;
	char com[COMMAND_LENGTH] = { 0 };
	size_t countOfStudents = 0;
	
	do
	{
		scanf("%s", com);
		if (!strcmp(com, "add"))
			if (AddToStart(&list, ReadStudent())) ++countOfStudents;
			else printf("Operation was failed. Function 'ReadStudent()' returned 0.\n");
		else if (!strcmp(com, "badlist")) PrintLowMarkStudents(list, countOfStudents, COUNT_OF_THE_LAST);
		else if (!strcmp(com, "removeby"))
		{
			size_t mark = 0;
			if (!scanf("%u", &mark))
			{
				rewind(stdin);
				printf("Invalid value! Operation was canceled.\n");
				continue;
			}
			countOfStudents -= RemoveByMark(&list, 2);
		}
		else if (!strcmp(com, "sort")) SortByNameR(list, countOfStudents);
	} while (com != "exit");
	/*
	FILE* inputFile = fopen(FILE_PATH, "r");
	if (inputFile == NULL)
	{
		perror("Opening file \"%s\" error", FILE_PATH);
		getch();
		return EXIT_FAILURE;
	}

	long countOfStudents = ReadStudentData(inputFile, &list);
	fclose(inputFile);

	if (countOfStudents == -1)
	{
		printf("List generation failure");
		getch();
		return EXIT_FAILURE;
	}
	if (countOfStudents == 0)
	{
		printf("File \"%s\" has no data or is corrupted", FILE_PATH);
		getch();
		return 0;
	}

	printf("Unsorted table:\n");
	PrintStudentTable(list);

	if (!SortByNameR(list, countOfStudents))
	{
		printf("Sorting failure");
		getch();
		return EXIT_FAILURE;
	}

	printf("\nSorted table (in reverse alphabetical order):\n");
	PrintStudentTable(list);
	printf("\n");

	// Adding new student
	if (!AddToStart(&list, ReadStudent()))
	{
		printf("Invalid input data");
		getch();
		return EXIT_FAILURE;
	}
	++countOfStudents;
	SortByNameR(list, countOfStudents);
	printf("New student was added\n");

	printf("\nRemoving students with mark '2'\n");
	countOfStudents -= RemoveByMark(&list, 2);

	printf("\nNew sorted table:\n");
	PrintStudentTable(list);

	printf("\nStudents with lowest average rating mark:\n");
	PrintLowMarkStudents(list, countOfStudents, COUNT_OF_THE_LAST);
	
	FreeList(&list);		*/
	getch();
	return 0;
}

/*
printf("\nAdding new student (Swift John  08.05.2000  3, 5, 3, 4, 4)...\n");
if (!AddToStart(&list, (Student) { "Swift", "John", { 8, 5, 2000 }, { 3, 5, 3, 4, 4 } }))
{
printf("Adding element failure");
getch();
return EXIT_FAILURE;
}
++countOfStudents;
SortByNameR(list, countOfStudents);
*/