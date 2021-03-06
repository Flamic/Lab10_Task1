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


#define COMMAND_LENGTH 20


int main()
{
	char com[COMMAND_LENGTH] = "\0";
	char filePath[NAMELEN] = "\0";
	SList *list = NULL;
	size_t countOfStudents = 0;

	printf("Enter 'help' to see the full list of commands\n");

	do
	{
		printf("COM>");
		scanf("%s", com);
		//==========Help==========//
		if (!strcmp(com, "help"))
		{
			printf(
				"clear		- remove all records from students list\n"
				"open		- open file with students list (requests file path)\n"
				"add		- add new student to the list (requests student data)\n"
				"badlist\t	- print list of 5 worst students\n"
				"removeby	- remove all students with entered mark\n"
				"sort		- sort students list in reversed alphabetical order\n"
				"print		- print table with all students from list\n"
				"synch		- rewrite connected file with new students data\n"
				"exit		- close program\n");
		}
		//==========Clear==========//
		else if (!strcmp(com, "clear"))
		{
			FreeList(&list);
			printf("The list has been cleared (all entries have been deleted)\n");
		}
		//==========Open==========//
		else if (!strcmp(com, "open"))
		{
			filePath[0] = '\0';
			FreeList(&list);

			printf("Enter file path: ");
			getc(stdin);
			fgets(filePath, NAMELEN, stdin);
			*strchr(filePath, '\n') = '\0';

			FILE* inputFile = fopen(filePath, "r");
			if (inputFile == NULL)
			{
				filePath[0] = '\0';
				perror("Opening file error");
				printf("\n");
				continue;
			}

			countOfStudents = ReadStudentData(inputFile, &list);
			fclose(inputFile);

			printf("%u student(s) has(ve) been successfully read\n", countOfStudents);
		}
		//==========Add==========//
		else if (!strcmp(com, "add"))
		{
			if (AddToTail(&list, ReadStudent()))
			{
				++countOfStudents;
				printf("Student was successfully added!\n");
			}
			else printf("Operation failed. Function 'ReadStudent' returned 0.\n");
		}
		//==========BadList==========//
		else if (!strcmp(com, "badlist"))
		{
			if (!PrintLowMarkStudents(list, countOfStudents, COUNT_OF_THE_LAST))
			{
				printf("Operation failed. Function 'PrintLowMarkStudents' returned 0.");
			}
		}
		//==========RemoveBy==========//
		else if (!strcmp(com, "removeby"))
		{
			unsigned mark = 0;
			printf("Enter mark: ");
			if (!scanf("%u", &mark) || mark < MIN_MARK || mark > MAX_MARK)
			{
				rewind(stdin);
				printf("Invalid value! Operation was canceled.\n");
				continue;
			}
			unsigned countOfRemoved = RemoveByMark(&list, mark);
			countOfStudents -= countOfRemoved;
			printf("%u student(s) has(ve) been removed\n", countOfRemoved);
		}
		//==========Sort==========//
		else if (!strcmp(com, "sort"))
		{
			SortByNameRev(&list);
			printf("List was sorted in reversed alphabetical order\n");
		}
		//==========Print==========//
		else if (!strcmp(com, "print"))
		{
			PrintStudentTable(list);
		}
		//==========Synch==========//
		else if (!strcmp(com, "synch"))
		{
			if (filePath[0] == '\0')
			{
				printf("Enter file path: ");
				getc(stdin);
				fgets(filePath, NAMELEN, stdin);
				*strchr(filePath, '\n') = '\0';
			}

			FILE* outputFile = fopen(filePath, "w");
			if (outputFile == NULL)
			{
				perror("Opening file error");
				printf("\n");
				continue;
			}
			WriteStudentData(outputFile, list);
			fclose(outputFile);

			printf("File %s has been successfully saved\n", filePath);
		}

		printf("\n");
	} while (strcmp(com, "exit"));
	
	FreeList(&list);
	return 0;
}
