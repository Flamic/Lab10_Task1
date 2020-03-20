#ifndef STUDENTS_MANAGER_H
#define STUDENTS_MANAGER_H

#define NAMELEN 255
#define COUNT_OF_THE_LAST 5

// Print all available commands
void ShowHelp();

// Remove all records from the students list
void Clear();

// Open file with the students list (requests file path)
void OpenFile();

// Add a new student to the list (requests student data)
void AddRequest();

// Print the list of the 5 worst students
void ShowBadList();

// Remove all students with the entered mark
void RemoveStudentByMark();

// Sort the students list in reversed alphabetical order
void SortList();

// Print the table with all students from the list
void PrintTable();

// Rewrite connected file with the new students data (or create a new file if no connection)
void SynchWithFile();

#endif
