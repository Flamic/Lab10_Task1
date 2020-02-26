#ifndef STUDENT_H
#define STUDENT_H

#define NAME_SIZE 50
#define COUNT_OF_MARKS 7
#define DATE_STR_SIZE 11

typedef struct
{
	unsigned char day;
	unsigned char month;
	unsigned short year;
} Birthday;

typedef struct
{
	char surname[NAME_SIZE];
	char name[NAME_SIZE];
	Birthday birthday;
	unsigned marks[COUNT_OF_MARKS];
} Student;

typedef struct StudentsList
{
	Student student;
	struct StudentsList *next;
} SList;


// Returns 1 if element was inserted successfully, else 0
int AddToStart(SList **head, Student student);

// Returns count of removed students
unsigned RemoveByMark(SList **head, unsigned mark);

// Free SList
void FreeList(SList **head);


// Returns count of students if file was read successfully, else 0 if there was no file and -1 if error
long ReadStudentData(FILE* file, SList **head);

// Read from console surname, name, birthday date and marks. Returns struct Student
Student ReadStudent();

// Returns string with birthday date
char* GetBirthday(const Birthday *date, char str[], size_t size);

// Returns string with marks of student
char* MarksToString(const Student *student, char str[], size_t size);

// Returns averege mark of student
double GetAverageMark(const Student *student);

// Prints student table with columns <Surname>, <Name>, <Birthday>, <Exam marks>
void PrintStudentTable(const SList *students);

// Returns 1 if names of students was printed successfully, else 0
int PrintLowMarkStudents(const SList *head, size_t size, size_t countToPrint);

// Returns 1 if list was sorted successfully, else 0
int SortByNameR(SList *head, size_t size);

#endif // !STUDENT_H