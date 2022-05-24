#define _CRT_SECURE_NO_WARNINGS
/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>*/ //uncomment this block to check for heap memory allocation leaks.
// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct StudentCourseGrade
{
	char courseName[35];
	int grade;
}StudentCourseGrade;

typedef struct Student
{
	char name[35];
	StudentCourseGrade* grades; //dynamic array of courses
	int numberOfCourses;
}Student;


//Part A
void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
int countPipes(const char* lineBuffer, int maxCount);
char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents);
void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor);
void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents);

//Part B
Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents);
Student* readFromBinFile(const char* fileName);
void writeToBinFile(const char* fileName, Student* students, int numberOfStudents);

int main()
{
	//Part A
	int* coursesPerStudent = NULL;
	int numberOfStudents = 0;
	//checking countPipes
	printf("%d\n", countPipes("sds||w4|p", 1000));

	//checking countStudentsAndCourses
	countStudentsAndCourses("studentList.txt", &coursesPerStudent, &numberOfStudents);
	for (int i = 0; i < numberOfStudents; i++)
	{
		printf("%d ", coursesPerStudent[i]);
	}
	printf("\n");


	//checking makeStudentArrayFromFile
	char*** students = makeStudentArrayFromFile("studentList.txt", &coursesPerStudent, &numberOfStudents);
	printf("*****students arr before giving a factor*****\n\n");
	for (int i = 0; i < numberOfStudents; i++)
	{
		printf("the details of student number %d is:\n", i + 1);
		for (int j = 0; j < ((coursesPerStudent[i] * 2) + 1); j++)
		{
			printf("%s\n", (students[i][j]));
		}
		printf("\n");
	}
	//checking factorGivenCourse
	factorGivenCourse(students, coursesPerStudent, numberOfStudents, "Advanced Topics in C", +5);
	printf("*****students arr after giving a factor*****\n\n");
	printStudentArray(students, coursesPerStudent, numberOfStudents);
	//checking studentsToFile
	/*studentsToFile(students, coursesPerStudent, numberOfStudents);*/ //this frees all memory. Part B fails if this line runs. uncomment for testing (and comment out Part B)

	//Part B

	printf("***PART B***\n");
	/// Checking Transformstudents Func
	Student* transformedStudents = transformStudentArray(students, coursesPerStudent, numberOfStudents);
	for (int i = 0; i < numberOfStudents; i++)
	{
		printf("details of student number %d:\n", i + 1);
		printf("the name of the student is %s\n", transformedStudents[i].name);
		printf("the num of courses he has are %d\n\n", transformedStudents[i].numberOfCourses);
		for (int j = 0; j < transformedStudents[i].numberOfCourses; j++)
		{
			printf("course: %s\n", transformedStudents[i].grades[j].courseName);
			printf("grade: %d\n", transformedStudents[i].grades[j].grade);

		}
		printf("\n");

	}
	//  Checking Readfronbinfile func
	Student* testReadStudents = readFromBinFile("students.bin"); // First we Read from bin file to the Student Struct
	// Check  WriteFrombinfile Func
	writeToBinFile("students.bin", transformedStudents, numberOfStudents); // Then to check ourself we write what we read back to the bin file

	//add code to free all arrays of struct Student


	/*_CrtDumpMemoryLeaks();*/ //uncomment this block to check for heap memory allocation leaks.
	// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019

	return 0;
}

void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{

	int i = 0;
	int StudentsNum = 0;
	FILE* pFile = NULL;
	char buffer[1023]; //each line the most 1023 characters
	pFile = fopen(fileName, "rt"); //opening text file
	if (pFile == NULL)
	{
		printf("Error opening File");
		exit(1);
	}
	while (!feof(pFile))
	{
		fgets(buffer, 1023, pFile);//rading one line from txt file to the buffer str(str with only 1021 characters+'\n'+'\0'
		StudentsNum++; //counting num of students

	}
	*numberOfStudents = StudentsNum;

	rewind(pFile); // returns the cursor to the start of the txt file

	*coursesPerStudent = (int*)malloc(sizeof(int) * StudentsNum); // put the address to set arr of ints
	if (!*coursesPerStudent)
	{
		printf("Memory Allocation\n");
		exit(1);
	}

	while (!feof(pFile))
	{
		fgets(buffer, 1023, pFile);// insert pointers for num of courses for each student
		int num = countPipes(buffer, 1023);
		(*coursesPerStudent)[i] = num; //insert num to the arr
		i++;
	}
	fclose(pFile);



}

int countPipes(const char* lineBuffer, int maxCount) //counting '|'
{
	int count = 0;
	if (lineBuffer == NULL)
	{
		return -1;
	}
	if (maxCount <= 0)
	{
		return 0;
	}
	for (int i = 0; lineBuffer[i] != '\0' || i > maxCount - 3; i++) //if the line is too long the last byte will be i[1020] because 1021 is for \n and 1022 is for \n
	{
		if (lineBuffer[i] == '|')
		{
			count++;
		}
	}
	return count;

}

char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	countStudentsAndCourses(fileName, coursesPerStudent, numberOfStudents); //set the arr ints of size of courses each student
	int i = 0;
	FILE* pFile = NULL;
	char buffer[1023]; //each line the most 1023 characters
	pFile = fopen(fileName, "rt"); //opening text file
	if (pFile == NULL)
	{
		printf("Error opening File");
		exit(1);
	}

	char*** students = (char***)malloc(sizeof(char**) * (*numberOfStudents)); // students arr[9]
	if (!students)
	{
		exit(1);
	}
	for (int i = 0; i < *numberOfStudents; i++)
	{
		students[i] = (char**)malloc(sizeof(char*) * (1 + ((*coursesPerStudent)[i] * 2)));// each student has arr of pointers with sizeof owner + num of names + num of courses(*students[i] because the ptr is ***)
		if (!students[i])
		{
			exit(1);
		}
	}
	//until it good

	for (int i = 0; i < *numberOfStudents; i++)
	{
		fgets(buffer, 1023, pFile); //read one line from txt file
		char* temp_buffer = buffer;
		char* temp_str = NULL;

		for (int j = 0; j < ((*coursesPerStudent)[i] * 2) + 1; j++)// pass the ddresses of corsesper[j] and look on the specific variable
		{
			char* temp_str = strtok(temp_buffer, "|,");
			students[i][j] = (char*)malloc(sizeof(char) * strlen(temp_str) + 1); //pass the addresses of students[i] and initiallize the str's //
			strcpy(students[i][j], temp_str);
			temp_buffer += strlen(temp_str) + 1;
			while (temp_str != NULL)
			{
				temp_str = strtok(NULL, "|,");
			}


		}

	}
	fclose(pFile);
	return students;
}

void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor)
{
	if (factor < -20 || factor>20)
	{
		printf("factor is illegal\n");
		return;
	}
	for (int i = 0; i < numberOfStudents; i++) //students[9]
	{
		for (int j = 0; j < (coursesPerStudent[i] * 2) + 1; j++)
		{
			if (strcmp(students[i][j], courseName) == 0) //if the student learnt this course he will get factor
			{
				int grade = atoi(students[i][j + 1]);
				grade = grade + factor; //updating the new grade
				if (grade < 0)
				{
					grade = 0;
				}
				else if (grade > 100)
				{
					grade = 100;
				}
				_itoa(grade, students[i][j + 1], 10); //updating the new str



			}
		}
	}
}

void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents)
{
	for (int i = 0; i < numberOfStudents; i++)
	{
		printf("name: %s\n*********\n", students[i][0]);
		for (int j = 1; j <= 2 * coursesPerStudent[i]; j += 2)
		{
			printf("course: %s\n", students[i][j]);
			printf("grade: %s\n", students[i][j + 1]);
		}
		printf("\n");
	}
}

void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents)
{
	int i = 0;
	FILE* pFile = NULL;
	pFile = fopen("studentList.txt", "wt"); //opening text file for writing
	if (pFile == NULL)
	{
		printf("Error opening File");
		exit(1);
	}
	for (int i = 0; i < numberOfStudents; i++) //students[9]
	{
		for (int j = 0; j < (coursesPerStudent[i] * 2) + 1; j++)
		{
			fputs(students[i][j], pFile); //writing one string from code to txt file
			if (j % 2 == 0 && j != (coursesPerStudent[i] * 2))
			{
				fputc('|', pFile); //adding '|' after even string and not including the last string
			}
			else if (j % 2 == 1)
			{
				fputc(',', pFile); //adding ',' after odd string
			}
		}
		/*fputc('\n', pFile);*/ //adding '\n' after and of line
	}
	fclose(pFile);
	for (int i = 0; i < numberOfStudents; i++) //students[9]
	{
		for (int j = 0; j < (coursesPerStudent[i] * 2) + 1; j++)
		{
			free(students[i][j]);
		}
		free(students[i]);
	}
	free(students);
	free(coursesPerStudent);
}

void writeToBinFile(const char* fileName, Student* students, int numberOfStudents)
{
	FILE* pbfile;
	pbfile = fopen(fileName, "wb");
	if (!pbfile) printf("Unable to open file!");
	else
	{
		fwrite(&numberOfStudents, sizeof(int), 1, pbfile);
		for (int i = 0; i < numberOfStudents; i++)
		{
			fwrite(&students[i].name, sizeof(char), 35, pbfile); /// writing student name
			fwrite(&students[i].numberOfCourses, sizeof(int), 1, pbfile); // writing number of courses for current student
			for (int j = 0; j < students[i].numberOfCourses; j++)
			{
				fwrite(&students[i].grades[j].courseName, sizeof(char), 35, pbfile); // writing  current course name
				fwrite(&students[i].grades[j].grade, sizeof(int), 1, pbfile); // writing  grade for the current course
			}
		}
	}
	fclose(pbfile);
}

Student* readFromBinFile(const char* fileName)
{
	FILE* pbfile;
	pbfile = fopen(fileName, "rb");
	if (!pbfile) printf("Unable to open file!");
	else
	{
		int numofstudents = 0;
		fread(&numofstudents, sizeof(int), 1, pbfile); // read number of students from binfile to int
		Student* students = (Student*)malloc(sizeof(Student) * numofstudents);
		for (int i = 0; i < numofstudents; i++)
		{
			fread(&students[i].name, sizeof(char), 35, pbfile); // Reading Current student name
			fread(&students[i].numberOfCourses, sizeof(int), 1, pbfile);
			students[i].grades = (StudentCourseGrade*)malloc(sizeof(StudentCourseGrade) * students[i].numberOfCourses);
			for (int j = 0; j < students[i].numberOfCourses; j++)
			{
				fread(&students[i].grades[j].courseName, sizeof(char), 35, pbfile); // reading current course name
				fread(&students[i].grades[j].grade, sizeof(int), 1, pbfile); // reading current course grade
			}
		}
		fclose(pbfile);
		return students;
	}
}


Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents)
{
	Student* students_arr = (Student*)malloc(sizeof(Student) * numberOfStudents); //students_arr[9]
	if (!students_arr)
	{
		printf("Memory Allocation\n");
		exit(1);
	}
	for (int i = 0; i < numberOfStudents; i++)
	{
		strcpy(students_arr[i].name, students[i][0]); //set name str
		students_arr[i].numberOfCourses = coursesPerStudent[i]; //set num of courses for each student
		students_arr[i].grades = (StudentCourseGrade*)malloc(sizeof(StudentCourseGrade) * coursesPerStudent[i]);
		if (!students_arr[i].grades)
		{
			printf("Memory Allocation\n");
			exit(1);
		}
		int k = 0; //for grades arr
		for (int j = 0; j < coursesPerStudent[i] * 2; j += 2)
		{
			strcpy(students_arr[i].grades[k].courseName, students[i][j + 1]); // entering course name
			students_arr[i].grades[k].grade = atoi(students[i][j + 2]); // entering course grade
			k++;

		}

	}
	return students_arr;
}
