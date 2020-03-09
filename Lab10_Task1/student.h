#ifndef STUDENT_H
#define STUDENT_H

#define NAME_SIZE 50			// Size of surname and name string
#define MIN_MARK 0				// Minimal mark
#define MAX_MARK 5				// Maximal mark
#define MAX_COUNT_OF_MARKS 7	// Maximum capacity of a table marks cell
#define COUNT_OF_MARKS 5		// Const count of marks

#if COUNT_OF_MARKS > MAX_COUNT_OF_MARKS
#error Fatal error! COUNT_OF_MARKS must be lower than MAX_COUNT_OF_MARKS
#else

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

typedef enum
{
	FALSE,
	TRUE
} Bool;

// Returns 1 if string contains only alphabetical characters, else 0
int IsStrAlpha(char str[]);

// Returns 1 if element was inserted at the head successfully, else 0
int AddToHead(SList** head, Student student);

// Returns 1 if element was inserted at the tail successfully, else 0
int AddToTail(SList** head, Student student);

// Returns count of removed students
unsigned RemoveByMark(SList** head, unsigned mark);

// Free SList
void FreeList(SList** head);

// Returns count of students if file was read successfully, else 0 if error
size_t ReadStudentData(FILE* file, SList** head);

// Write student list to file (rewrites file)
void WriteStudentData(FILE* file, const SList* head);

// Returns 1 if birthday is correct, else 0
int IsBirthdayCorrect(const Birthday* birthday);

// Returns 1 if mark is correct, else 0
int IsMarkCorrect(unsigned mark);

// Read from console surname, name, birthday date and marks. Returns generated struct Student.
// Example of input data:
//	Enter surname: Swift
//	Enter name: John
//	Enter birthday: 16.03.1998
//	Enter 5 marks: 5 5 3 4 3
Student ReadStudent();

// Returns string with birthday date
char* GetBirthday(const Birthday* date, char str[], size_t size);

// Print birthday in format dd.mm.yyyy to stream 'file'
void PrintBirthday(FILE* file, const Birthday* date);

// Returns string with marks of student
char* MarksToString(const Student* student, char str[], size_t size);

// Print list of marks (3, 5, ...) to stream 'file'
void PrintMarks(FILE* file, const Student* student);

// Returns averege mark of student
double GetAverageMark(const Student* student);

// Print student table with columns <Surname>, <Name>, <Birthday>, <Exam marks>
void PrintStudentTable(const SList* students);

// Returns 1 if names of the worst students was printed successfully, else 0
int PrintLowMarkStudents(const SList* head, size_t size, size_t countToPrint);

// Swap student1 and student2
SList* Swap(SList* student1, SList* student2, SList* root);

// Sort list in reversed alphabetical order
void SortByNameRev(SList** head);

#endif
#endif // !STUDENT_H