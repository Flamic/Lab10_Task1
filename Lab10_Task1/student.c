#include "stdafx.h"
#include "student.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

//----------------------------------------------------------------
// Returns 1 if string contains only alphabetical characters, else 0
int IsStrAlpha(unsigned char str[])
{
	if (str[0] == '-' || str[0] == '\0' || !isalpha(str[0]))
	{
		return 0;
	}
	for (size_t i = 1; str[i] != '\0'; ++i)
	{
		if (!isalpha((unsigned char)str[i]))
		{
			if (!(str[i] == '-' && isalpha(str[i - 1]) && isalpha(str[i + 1])))
			{
				return 0;
			}
		}
	}
	return 1;
}
//----------------------------------------------------------------
// Returns 1 if element was inserted at the head successfully, else 0
int AddToHead(SList **head, Student student)
{
	if (head == NULL)
	{
		return 0;
	}

	SList *element = (SList*)malloc(sizeof(SList));
	if (element == NULL)
	{
		return 0;
	}
	
	element->student = student;
	element->next = (*head == NULL ? NULL : *head);
	*head = element;
	
	return 1;
}
//----------------------------------------------------------------
// Returns 1 if element was inserted at the tail successfully, else 0
int AddToTail(SList** head, Student student)
{
	if (head == NULL)
	{
		return 0;
	}

	SList* element = (SList*)malloc(sizeof(SList));
	if (element == NULL)
	{
		return 0;
	}

	element->student = student;
	element->next = NULL;
	
	if (*head == NULL)
	{
		*head = element;
	}
	else
	{
		SList* tmp = *head;
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = element;
	}

	return 1;
}
//----------------------------------------------------------------
// Returns count of removed students
unsigned RemoveByMark(SList **head, unsigned mark)
{
	if (head == NULL || *head == NULL)
	{
		return 0;
	}
	
	size_t countOfRemoved = 0;
	SList *tmp = *head;
	Bool isRemoved;

	while (tmp->next != NULL)
	{
		isRemoved = FALSE;
		for (size_t i = 0; i < COUNT_OF_MARKS; ++i)
		{
			if (tmp->next->student.marks[i] == mark)
			{
				isRemoved = TRUE;
				++countOfRemoved;
				SList *tmp2 = tmp->next;
				tmp->next = tmp2->next;
				free(tmp2);

				break;
			}
		}
		if (!isRemoved)
		{
			tmp = tmp->next;
		}
	}

	for (size_t i = 0; i < COUNT_OF_MARKS; ++i)
	{
		if ((*head)->student.marks[i] == mark)
		{
			++countOfRemoved;
			tmp = *head;
			*head = (*head)->next;
			free(tmp);

			break;
		}
	}

	return countOfRemoved;
}
//----------------------------------------------------------------
// Free SList
void FreeList(SList **head)
{
	if (head == NULL)
	{
		return;
	}

	SList *tmp = *head;
	while (tmp != NULL)
	{
		*head = tmp->next;
		free(tmp);
		tmp = *head;
	}
}
//----------------------------------------------------------------
// Returns count of students if file was read successfully, else 0 if error
size_t ReadStudentData(FILE* file, SList **head, unsigned maxCount)
{
	if (file == NULL || head == NULL)
	{
		return 0;
	}
	FreeList(head);

	Student student;
	size_t i = 0;

	for (; !feof(file) && i < maxCount; ++i)
	{
		fgetc(file);
		fgetc(file);
		if (feof(file)) break;
		fseek(file, -2, SEEK_CUR);
		
		if (fscanf(file, "%" STR_NAME_SIZE "s\t%" STR_NAME_SIZE "s\t%u.%u.%u\t",
			&student.surname,
			&student.name,
			&student.birthday.day,
			&student.birthday.month,
			&student.birthday.year) != 5
			|| !IsStrAlpha(&student.surname)
			|| !IsStrAlpha(&student.name)
			|| !IsBirthdayCorrect(&student.birthday))
		{
			FreeList(head);
			return 0;
		}

		for (size_t j = 0; j < COUNT_OF_MARKS; ++j)
		{
			if (fscanf(file, "%u,", &student.marks[j]) != 1
				|| !IsMarkCorrect(student.marks[j]))
			{
				FreeList(head);
				return 0;
			}
		}

		if (!AddToTail(head, student))
		{
			FreeList(head);
			return 0;
		}
	}

	return i;
}
//----------------------------------------------------------------
// Write student list to file (rewrites file)
void WriteStudentData(FILE* file, const SList* head)
{
	if (file == NULL || head == NULL)
	{
		return;
	}

	do
	{
		fprintf(file, "%s\t%s\t", head->student.surname, head->student.name);
		PrintBirthday(file, &(head->student.birthday));
		fprintf(file, "\t");
		PrintMarks(file, &(head->student));
		fprintf(file, "\n");
	} while ((head = head->next) != NULL);
}
//----------------------------------------------------------------
// Returns 1 if birthday is correct, else 0
int IsBirthdayCorrect(const Birthday *birthday)
{
	time_t t;
	time(&t);
	int curYear = localtime(&t)->tm_year + 1900;
#ifdef MAX_AGE
	int lowYearLimit = curYear - MAX_AGE;
#else
	int lowYearLimit = 0;
#endif
	if (birthday->day >= 1
		&& birthday->month >= 1 && birthday->month <= 12
		&& birthday->year > lowYearLimit && birthday->year < curYear)
	{
		if (birthday->month == 2)
		{
			if (((birthday->year % 4 && birthday->day <= 28)
				|| (!(birthday->year % 4) && birthday->day <= 29)))
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		if (birthday->month == 1
			|| birthday->month == 3
			|| birthday->month == 5
			|| birthday->month == 7
			|| birthday->month == 8
			|| birthday->month == 10
			|| birthday->month == 12)
		{
			if (birthday->day <= 31) return 1;
			else return 0;
		}
		else
		{
			if (birthday->day <= 30) return 1;
		}
	}

	return 0;
}
//----------------------------------------------------------------
// Returns 1 if mark is correct, else 0
int IsMarkCorrect(unsigned mark)
{
	if (mark >= MIN_MARK && mark <= MAX_MARK)
	{
		return 1;
	}

	return 0;
}
//----------------------------------------------------------------
// Read from console surname, name, birthday date and marks. Returns generated struct Student
Student ReadStudent()
{
	Student student;
	do
	{
		rewind(stdin);
		printf("Enter surname (max length %u): ", NAME_SIZE);
		fgets(&(student.surname), NAME_SIZE, stdin);
		char* p = strchr(&(student.surname), '\n');
		if (p) *p = '\0';
	} while ((!student.surname || !IsStrAlpha(student.surname)) ? printf("Incorrect surname!\n") : 0);

	do
	{
		rewind(stdin);
		printf("Enter name (max length %u): ", NAME_SIZE);
		fgets(&(student.name), NAME_SIZE, stdin);
		char* p = strchr(&(student.name), '\n');
		if (p) *p = '\0';
	} while ((!student.name || !IsStrAlpha(student.name)) ? printf("Incorrect name!\n") : 0);

	rewind(stdin);
	printf("Enter birthday date: ");
	while (scanf("%u.%u.%u",
		&(student.birthday.day),
		&(student.birthday.month),
		&(student.birthday.year)) != 3
		|| !IsBirthdayCorrect(&(student.birthday)))
	{
		rewind(stdin);
		printf("Incorrect date!\nEnter correct birthday date: ");
	}

	rewind(stdin);
	printf("Enter %u marks: ", COUNT_OF_MARKS);
	for (size_t j = 0; j < COUNT_OF_MARKS; ++j)
	{
		while (!scanf("%u", &(student.marks[j])) || !IsMarkCorrect(student.marks[j]))
		{
			rewind(stdin);
			printf("Some marks are incorrect! Please retype marks starting from %u mark: \n", j + 1);
		}
	}

	return student;
}
//----------------------------------------------------------------
// Returns string with birthday date
char* GetBirthday(const Birthday *date, char str[], size_t size)
{
	if (date == NULL)
	{
		return str;
	}
	snprintf(str, size, "%02u.%02u.%u", date->day, date->month, date->year);
	return str;
}
//----------------------------------------------------------------
// Print birthday in format dd.mm.yyyy to stream 'file'
void PrintBirthday(FILE *file, const Birthday *date)
{
	if (date == NULL || file == NULL)
	{
		return;
	}
	fprintf(file, "%02u.%02u.%-5u", date->day, date->month, date->year);
}
//----------------------------------------------------------------
// Returns string with marks of student
char* MarksToString(const Student *student, char str[], size_t size)
{
	if (student == NULL)
	{
		return str;
	}
	size_t offset = 0;
	for (size_t i = 0; i < COUNT_OF_MARKS; ++i)
	{
		offset += snprintf(str + offset, size - offset, "%u, ", student->marks[i]);
	}
	*(str + offset - 2) = '\0';
	return str;
}
//----------------------------------------------------------------
// Print list of marks (3, 5, ...) to stream 'file'
void PrintMarks(FILE *file, const Student *student)
{
	if (student == NULL || file == NULL)
	{
		return;
	}
	for (size_t i = 0; i < COUNT_OF_MARKS-1; ++i)
	{
		fprintf(file, "%u, ", student->marks[i]);
	}
	fprintf(file, "%u", student->marks[COUNT_OF_MARKS - 1]);
}
//----------------------------------------------------------------
// Returns averege mark of student
double GetAverageMark(const Student *student)
{
	if (student == NULL)
	{
		return 0;
	}
	long sum = 0;
	for (size_t i = 0; i < COUNT_OF_MARKS; ++i)
	{
		sum += student->marks[i];
	}
	return 1.0 * sum / COUNT_OF_MARKS;
}
//----------------------------------------------------------------
// Print student table with columns <Surname>, <Name>, <Birthday>, <Exam marks>
void PrintStudentTable(const SList *students)
{
	if (students == NULL)
	{
		printf("<List of students is empty>\n");
		return;
	}

	printf("%c----------------------------------------------------------------------------%c\n",
		TABLE_TOP_LEFT, TABLE_TOP_RIGHT);
	printf("%c%-20s%c%-20s%c%-12s%c%-21s%c\n", TABLE_VERTICAL,
		"       Surname", TABLE_VERTICAL,
		"        Name", TABLE_VERTICAL,
		"  Birthday", TABLE_VERTICAL,
		"      Exam marks", TABLE_VERTICAL);
	do
	{
		printf("%c----------------------------------------------------------------------------%c\n"
			"%c %-19s%c %-19s%c ",
			TABLE_VERTICAL, TABLE_VERTICAL, TABLE_VERTICAL,
			&(students->student.surname), TABLE_VERTICAL,
			&(students->student.name), TABLE_VERTICAL);
		PrintBirthday(stdout, &(students->student.birthday));
		printf("%c ", TABLE_VERTICAL);
		PrintMarks(stdout, students);
		for (size_t i = COUNT_OF_MARKS; i < MAX_COUNT_OF_MARKS; ++i)
		{
			printf("   ");
		}
		printf(" %c\n", TABLE_VERTICAL);
	} while ((students = students->next) != NULL);

	printf("%c----------------------------------------------------------------------------%c\n",
		TABLE_BOTTOM_LEFT, TABLE_BOTTOM_RIGHT);
}
//----------------------------------------------------------------
// Swap size_t elements of array (array[ind1] with array[ind2])
void SwapSize_t(size_t *array, size_t ind1, size_t ind2)
{
	size_t tmp = array[ind1];
	array[ind1] = array[ind2];
	array[ind2] = tmp;
}
//----------------------------------------------------------------
// Swap double elements of array (array[ind1] with array[ind2])
void SwapDouble(double *array, size_t ind1, size_t ind2)
{
	double tmp = array[ind1];
	array[ind1] = array[ind2];
	array[ind2] = tmp;
}
//----------------------------------------------------------------
// Returns 1 if names of the worst students was printed successfully, else 0
int PrintLowMarkStudents(const SList *head, size_t size, size_t countToPrint)
{
	if (head == NULL || size == 0 || countToPrint == 0)
	{
		return 1;
	}

	if (size <= countToPrint)
	{
		do
		{
			printf("%s %s\n", head->student.surname, head->student.name);
		} while ((head = head->next) != NULL);
		return 1;
	}

	SList *savedHead = head;
	size_t *lastIndex = (size_t*)malloc(MIN(size, countToPrint) * sizeof(size_t));
	double *minAvg = (double*)malloc(MIN(size, countToPrint) * sizeof(double));

	if (lastIndex == NULL || minAvg == NULL)
	{
		return 0;
	}
	
	for (size_t i = 0; i < countToPrint; ++i)
	{
		lastIndex[i] = i;
		minAvg[i] = GetAverageMark(&(head->student));
		head = head->next;
	}
	for (size_t i = countToPrint; i < size; ++i)
	{
		double maxDif = 0;
		double average = GetAverageMark(&(head->student));
		size_t element;
		
		for (size_t j = 0; j < countToPrint; ++j)
		{
			if (minAvg[j] - average > maxDif)
			{
				maxDif = minAvg[j] - average;
				element = j;
			}
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
	{
		for (size_t j = i + 1; j < countToPrint; ++j)
		{
			if (lastIndex[i] > lastIndex[j])
			{
				SwapSize_t(lastIndex, i, j);
				SwapDouble(minAvg, i, j);
			}
		}
	}

	size_t j = 0;
	for (size_t i = 0; i < size; ++i)
	{
		if (i == lastIndex[j])
		{
			printf("%s %s\n", head->student.surname, head->student.name);
			++j;
			if (j == countToPrint)
			{
				break;
			}
		}
		head = head->next;
	}

	free(lastIndex);
	free(minAvg);
	return 1;
}
//----------------------------------------------------------------
// (private) Returns -1 if student1 is before student2 in alphabetical order
// or 0 if they are equal, else 1
int Compare(const Student *student1, const Student *student2)
{
	if (strcmp(student1->surname, student2->surname) < 0
		|| (strcmp(student1->surname, student2->surname) == 0
			&& strcmp(student1->name, student2->name) < 0))
	{
		return -1;
	}
	else if (strcmp(student1->surname, student2->surname) == 0
		&& strcmp(student1->name, student2->name) == 0)
	{
		return 0;
	}
	return 1;
}
//----------------------------------------------------------------
// Swap student1 and student2
SList* Swap(SList* head, SList* student1, SList* student2)
{
	SList* prev1, *prev2, *next1, *next2;
	prev1 = head;
	prev2 = head;
	if (prev1 == student1)
	{
		prev1 = NULL;
	}
	else
	{
		while (prev1->next != student1)
		{
			prev1 = prev1->next;
		}
	}
	if (prev2 == student2)
	{
		prev2 = NULL;
	}
	else
	{
		while (prev2->next != student2)
		{
			prev2 = prev2->next;
		}
	}
	next1 = student1->next;
	next2 = student2->next;
	if (student2 == next1)
	{
		student2->next = student1;
		student1->next = next2;
		if (student1 != head)
		{
			prev1->next = student2;
		}
	}
	else
		if (student1 == next2)
		{
			student1->next = student2;
			student2->next = next1;
			if (student2 != head)
			{
				prev2->next = student2;
			}
		}
		else
		{
			if (student1 != head)
			{
				prev1->next = student2;
			}
			student2->next = next1;
			if (student2 != head)
			{
				prev2->next = student1;
			}
			student1->next = next2;
		}
	if (student1 == head)
	{
		return(student2);
	}
	if (student2 == head)
	{
		return(student1);
	}
	return(head);
}
//----------------------------------------------------------------
// Sort list in reversed alphabetical order
void SortByNameRev(SList **head)
{
	if (head == NULL) return;

	SList* min;
	SList* tmp;
	SList* selected = *head;
	while (selected != NULL)
	{
		min = selected;
		tmp = selected->next;
		while (tmp != NULL)
		{
			if (Compare(&(min->student), &(tmp->student)) < 0) min = tmp;
			tmp = tmp->next;
		}
		if (min != selected)
		{
			*head = Swap(*head, selected, min);
			selected = min->next;
		}
		else selected = selected->next;
	}
}