#include "stdafx.h"
#include "student.h"
#include <stdlib.h>
#include <string.h>

// │ 179
// ─ 196
// ┐ 191
// └ 192
// ┘ 217
// ┌ 218

#define TABLE_TOP_LEFT 218
#define TABLE_TOP_RIGHT 191
#define TABLE_BOTTOM_LEFT 192
#define TABLE_BOTTOM_RIGHT 217
#define TABLE_VERTICAL 179

#define MIN(n1, n2) (((n1) < (n2)) ? (n1) : (n2))

// Returns 1 if element was inserted successfully, else 0
int AddToStart(SList **head, Student student)
{
	SList *element = (SList*)malloc(sizeof(SList));
	if (element == NULL) return 0;
	
	element->student = student;
	element->next = (*head == NULL ? NULL : *head);
	*head = element;
	
	return 1;
}

// Returns count of removed students
unsigned RemoveByMark(SList **head, unsigned mark)
{
	if (*head == NULL) return 0;
	
	SList *tmp = *head;
	int isRemoved;
	size_t count = 0;

	while (tmp->next != NULL)
	{
		isRemoved = 0;
		for (size_t i = 0; i < COUNT_OF_MARKS; ++i)
		{
			if (tmp->next->student.marks[i] == mark)
			{
				isRemoved = 1;
				++count;
				SList *tmp2 = tmp->next;
				tmp->next = tmp2->next;
				free(tmp2);

				break;
			}
		}
		if (!isRemoved) tmp = tmp->next;
	}

	for (size_t i = 0; i < COUNT_OF_MARKS; ++i)
	{
		if ((*head)->student.marks[i] == mark)
		{
			++count;
			tmp = *head;
			*head = (*head)->next;
			free(tmp);

			break;
		}
	}

	return count;
}

// Free SList
void FreeList(SList **head)
{
	SList *tmp = *head;
	while (tmp != NULL)
	{
		*head = tmp->next;
		free(tmp);
		tmp = *head;
	}
}

// Returns count of students if file was read successfully, else 0 if there was no file and -1 if error
long ReadStudentData(FILE* file, SList **head)
{
	if (file == NULL) return 0;	// If no file, returns 0

	SList *savedHead = *head;
	Student student;
	size_t i = 0;
	for (; !feof(file); ++i)
	{
		if (fscanf(file, "%s\t%s\t%u.%u.%u\t",
			&student.surname,
			&student.name,
			&student.birthday.day,
			&student.birthday.month,
			&student.birthday.year) != 5) return 0;
		
		size_t j;
		for (j = 0; j < COUNT_OF_MARKS && getc(file) != '\n' && !feof(file); ++j)
		{
			fseek(file, -1, SEEK_CUR);
			if (fscanf(file, "%u,", &student.marks[j]) != 1) return 0;
		}
		memset(student.marks + j, 0, (COUNT_OF_MARKS - j) * sizeof(student.marks) / COUNT_OF_MARKS);

		if (!AddToStart(head, student))	// If error, restores list and returns -1
		{
			*head = savedHead;
			return -1;
		}
	}

	return i;
}

// Read from console surname, name, birthday date and marks. Returns struct Student
Student ReadStudent()
{
	Student student;

	printf("Enter surname: ");
	scanf("%s", &(student.surname));

	printf("Enter name: ");
	scanf("%s", &(student.name));

	printf("Enter birthday date: ");
	scanf("%u.%u.%u", &(student.birthday.day), &(student.birthday.month), &(student.birthday.year));

	printf("Enter marks: ");
	size_t j;
	for (j = 0; j < COUNT_OF_MARKS; ++j)
	{
		scanf("%u", &(student.marks[j]));
		if (getc(stdin) == '\n')
		{
			++j;
			break;
		}
	}
	memset(student.marks + j, 0, (COUNT_OF_MARKS - j) * sizeof(student.marks) / COUNT_OF_MARKS);

	return student;
}

// Returns string with birthday date
char* GetBirthday(const Birthday *date, char str[], size_t size)
{
	snprintf(str, size, "%02u.%02u.%u", date->day, date->month, date->year);
	return str;
}

// Returns string with marks of student
char* MarksToString(const Student *student, char str[], size_t size)
{
	size_t offset = 0;
	for (size_t i = 0; i < COUNT_OF_MARKS; ++i)
		if (student->marks[i]) offset += snprintf(str + offset, size - offset, "%u, ", student->marks[i]);

	*(str + offset - 2) = '\0';
	return str;
}

// Returns averege mark of student
double GetAverageMark(const Student *student)
{
	long sum = 0;
	for (size_t i = 0; i < COUNT_OF_MARKS; ++i) sum += student->marks[i];
	return 1.0 * sum / COUNT_OF_MARKS;
}

// Prints student table with columns <Surname>, <Name>, <Birthday>, <Exam marks>
void PrintStudentTable(const SList *students)
{
	if (students == NULL)
	{
		printf("<List of students is empty>\n");
		return;
	}
	char dateStr[DATE_STR_SIZE];
	char marksStr[COUNT_OF_MARKS * 5];

	printf("%c----------------------------------------------------------------------------%c\n", TABLE_TOP_LEFT, TABLE_TOP_RIGHT);
	printf("%c%-20s%c%-20s%c%-12s%c%-21s%c\n", TABLE_VERTICAL,
		"       Surname", TABLE_VERTICAL,
		"        Name", TABLE_VERTICAL,
		"  Birthday", TABLE_VERTICAL,
		"      Exam marks", TABLE_VERTICAL);
	do
	{
		GetBirthday(&(students->student.birthday), dateStr, DATE_STR_SIZE);
		MarksToString(&(students->student), marksStr, COUNT_OF_MARKS * 5);
		printf("%c----------------------------------------------------------------------------%c\n", TABLE_VERTICAL, TABLE_VERTICAL);
		printf("%c %-19s%c %-19s%c %-11s%c %-20s%c\n", TABLE_VERTICAL,
			&(students->student.surname), TABLE_VERTICAL,
			&(students->student.name), TABLE_VERTICAL,
			dateStr, TABLE_VERTICAL,
			marksStr, TABLE_VERTICAL);
	} while ((students = students->next) != NULL);
	printf("%c----------------------------------------------------------------------------%c\n", TABLE_BOTTOM_LEFT, TABLE_BOTTOM_RIGHT);
}

// Returns 1 if names of students was printed successfully, else 0
int PrintLowMarkStudents(const SList *head, size_t size, size_t countToPrint)
{
	if (head == NULL || size == 0 || countToPrint == 0) return 1;

	SList *savedHead = head;
	size_t *lastIndex = (size_t*)malloc(MIN(size, countToPrint) * sizeof(size_t));
	double *minAvg = (double*)malloc(MIN(size, countToPrint) * sizeof(double));

	if (lastIndex == NULL || minAvg == NULL) return 0;
	
	for (size_t i = 0; i < MIN(size, countToPrint); ++i)
	{
		lastIndex[i] = i;
		minAvg[i] = GetAverageMark(&(head->student));
		head = head->next;
	}
	for (size_t i = countToPrint; i < size; ++i)
	{
		double maxDif = 0, average = GetAverageMark(&(head->student));
		size_t element;
		
		for (size_t j = 0; j < countToPrint; ++j)
			if (minAvg[j] - average > maxDif)
			{
				maxDif = minAvg[j] - average;
				element = j;
			}
		if (maxDif > 0)
		{
			lastIndex[element] = i;
			minAvg[element] = average;
		}
		head = head->next;
	}
	head = savedHead;

	for (size_t i = 0; i < countToPrint - 1; ++i)
		for (size_t j = i + 1; j < countToPrint; ++j)
		{
			if (lastIndex[i] > lastIndex[j])
			{
				int tmp = lastIndex[i];
				lastIndex[i] = lastIndex[j];
				lastIndex[j] = tmp;

				double tmp2 = minAvg[i];
				minAvg[i] = minAvg[j];
				minAvg[j] = tmp2;
			}
		}

	size_t j = 0;
	for (size_t i = 0; i < size; ++i)
	{
		if (i == lastIndex[j])
		{
			printf("%s %s\n", head->student.surname, head->student.name);
			++j;
			if (j == countToPrint) break;
		}
		head = head->next;
	}

	free(lastIndex);
	free(minAvg);
	return 1;
}

// Returns 1 if list was sorted successfully, else 0
int SortByNameR(SList *head, size_t size)	//	With size
{
	if (size == 0) return 1;

	Student **students = (Student**)malloc(size * sizeof(Student*));
	if (students == NULL) return 0;

	for (size_t i = 0; i < size; ++i)
	{
		students[i] = &(head->student);
		head = head->next;
	}
	Student tmpStudent;
	for (size_t i = 0; i < size - 1; ++i)
		for (size_t j = i + 1; j < size; ++j)
		{
			if (strcmp(students[i]->surname, students[j]->surname) < 0
				|| (strcmp(students[i]->surname, students[j]->surname) == 0 && strcmp(students[i]->name, students[j]->name) < 0))
			{
				tmpStudent = *(students[i]);
				*(students[i]) = *(students[j]);
				*(students[j]) = tmpStudent;
			}
		}
	free(students);
	return 1;
}

/*void SortByNameR(SList *head)	//	Without size
{
	SList *savedHead = head;
	unsigned count = 0;
	for (; head != NULL; ++count) head = head->next;
	
	head = savedHead;
	Student **students = (Student**)malloc(count * sizeof(Student*));

	for (unsigned i = 0; i < count; ++i)
	{
		students[i] = &(head->student);
		head = head->next;
	}
	Student tmpStudent;
	for (unsigned i = 0; i < count - 1; ++i)
		for (unsigned j = i + 1; j < count; ++j)
		{
			if (strcmp(students[i]->surname, students[j]->surname) < 0
				|| (strcmp(students[i]->surname, students[j]->surname) == 0 && strcmp(students[i]->name, students[j]->name) < 0))
			{
				tmpStudent = *(students[i]);
				*(students[i]) = *(students[j]);
				*(students[j]) = tmpStudent;
			}
		}
	free(students);
}*/