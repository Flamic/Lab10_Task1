/*
	2. Відсортувати дані за прізвищами студентів в порядку протилежному алфавітному.
	Організувати послідовність структур в однозв’язний список. Реалізувати операцію
	вставки нового елемента у відсортований список і операцію вилучення зі списку
	даних про студентів з оцінками 2. Визначити п’ять студентів з найнижчим середнім
	рейтинговим балом.
*/

#include "stdafx.h"
#include "StudentsManager.h"
#include <string.h>
#include <stdlib.h>

#define VERSION "0.2 (alpha)"

#define COMMAND_LENGTH 20

int main()
{
#ifdef VERSION
	printf("Students Manager. Version " VERSION ". Created by Yevhenii Luzhetskyi.\n\n");
#endif
	char com[COMMAND_LENGTH] = "\0";
	printf("Enter 'help' to see the full list of commands.\nEnter 'macro1' to run "
		"default macro function (open -> sort -> print -> badlist -> removeby -> print).\n\n");
	
	do
	{
		printf("COM>");
		rewind(stdin);
		fgets(com, COMMAND_LENGTH, stdin);
		char* p = strchr(com, '\n');
		if (p) *p = '\0';

		if (!strcmp(com, "help"))
		{
			ShowHelp();
		}
		else if (!strcmp(com, "clear"))
		{
			Clear();
		}
		else if (!strcmp(com, "open"))
		{
			OpenFile();
		}
		else if (!strcmp(com, "add"))
		{
			AddRequest();
		}
		else if (!strcmp(com, "badlist"))
		{
			ShowBadList();
		}
		else if (!strcmp(com, "removeby"))
		{
			RemoveStudentByMark();
		}
		else if (!strcmp(com, "sort"))
		{
			SortList();
		}
		else if (!strcmp(com, "print"))
		{
			PrintTable();
		}
		else if (!strcmp(com, "synch"))
		{
			SynchWithFile();
		}
		else if (!strcmp(com, "macro1"))
		{
			if (OpenFile())
			{
				SortList();
				PrintTable();
				ShowBadList();
				RemoveStudentByMark();
				PrintTable();
			}
		}

		printf("\n");
	} while (strcmp(com, "exit"));
	
	Clear();
	return 0;
}
