#ifndef _Prog_H
#define _Prog_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


// ******************************* STRUCTURES *******************************//

typedef struct days {
	int lactureDay;
	int beginTime;
	int howLong;
}days, *Pdays;

typedef struct lacture {
	char* lacturerName;
	Pdays lacturerDays;
	int lactureDayesSize;
}lacture, *Placture;

typedef struct exercise {
	char* exeName;                 
	Pdays exeDays;
	int exeDaysSize;
}exe, *Pexe;

typedef struct lab {
	char* labName;
	Pdays labDays;
	int labDaysSize;
}lab, *Plab;

typedef struct courses {
	char *courseName;
	Placture courseLactures;
	Pexe courseExe;
	Plab courseLab;
	int courseLacturesSize;
	int courseExeSize;
	int courseLabSize;
}courses, *Pcourses;

// ******************************* FUNCTIONS DECLERATION *******************************//

Pcourses Read_file_line(Pcourses );

Pcourses Analize_row(Pcourses , char* );

Pcourses Add_course(Pcourses , char* );

void Initiate_course(Pcourses , char* );

void Add_lacture(Pcourses , char* );

void Add_exerciser(Pcourses , char* );

void Add_laboratory(Pcourses , char* );

Pdays Allocate_day(Pdays , int , char* );

void Put_day_detailes(Pdays , char* , char* , int );

void Print_schedule(FILE* );

void Delete_hole_mainArr(Pcourses* );

void Delete_lacture(Placture );

void Delete_exercise(Pexe );

void Delete_lab(Plab );

void Find_schedule(Pcourses , FILE* , int/* , int**, int**/, int*, int, int);

int Check_if_windowless(/*int **, int*,int*/);

int Check_for_dayOff(int*, int/*, int**, int**/);

int Put_lacture_in_schedule(courses , int );

int Put_exercise_in_schedule(courses , int );

int Put_laboratory_in_schedule(courses , int );

void Remove_exercise_from_schedule(exe );

void Remove_lacture_from_schedule(lacture );

void Remove_laboratory_from_schedule(lab );

int Valid_line(char* , FILE* , int );

int Check_end_of_line(char* , int , char* , FILE* );

char* Valid_date(char* , char* );

int Valid_duration_line(char* );

int Valid_day_line(char* );

int Valid_time_line(char* );

int Ask_for_windowless();

int Ask_for_dayoff(int*);

//void Find_the_wanted_schedule(int, int, int*, int);

#endif // !_Prog_H

