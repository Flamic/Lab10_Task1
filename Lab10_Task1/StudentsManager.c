#include "stdafx.h"
#include "student.h"
#include "StudentsManager.h"
#include <string.h>

char filePath[NAMELEN] = "\0";
SList* list = NULL;
size_t countOfStudents = 0;

void ShowHelp()
{
	printf(
		"clear		- remove all records from the students list\n"
		"open		- open file with the students list (requests file path)\n"
		"add		- add a new student to the list (requests student data)\n"
		"badlist\t	- print the list of the 5 worst students\n"
		"removeby	- remove all students with the entered mark\n"
		"sort		- sort the students list in reversed alphabetical order\n"
		"print		- print the table with all students from the list\n"
		"synch		- rewrite connected file with the new students data\n"
		"exit		- close program\n");

}

void Clear()
{
	FreeList(&list);
	printf("The list has been cleared (all entries have been deleted)\n");
}

void OpenFile()
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
		return;
	}

	countOfStudents = ReadStudentData(inputFile, &list);
	fclose(inputFile);

	printf("%u student(s) has(ve) been successfully read\n", countOfStudents);
}

void AddRequest()
{
	if (AddToTail(&list, ReadStudent()))
	{
		++countOfStudents;
		printf("Student was successfully added!\n");
	}
	else printf("Operation failed. Function 'ReadStudent' returned 0.\n");
}

void ShowBadList()
{
	if (!PrintLowMarkStudents(list, countOfStudents, COUNT_OF_THE_LAST))
	{
		printf("Operation failed. Function 'PrintLowMarkStudents' returned 0.\n");
	}
}

void RemoveStudentByMark()
{
	unsigned mark = 0;
	printf("Enter mark: ");
	if (!scanf("%u", &mark) || mark < MIN_MARK || mark > MAX_MARK)
	{
		rewind(stdin);
		printf("Invalid value! Operation was canceled.\n");
		return;
	}
	unsigned countOfRemoved = RemoveByMark(&list, mark);
	countOfStudents -= countOfRemoved;
	printf("%u student(s) has(ve) been removed\n", countOfRemoved);
}

void SortList()
{
	SortByNameRev(&list);
	printf("List was sorted in reversed alphabetical order\n");
}

void PrintTable()
{
	PrintStudentTable(list);
}

void SynchWithFile()
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
		return;
	}
	WriteStudentData(outputFile, list);
	fclose(outputFile);

	printf("File %s has been successfully saved\n", filePath);
}