#include "ProgHeader.h"


// ******************************* GLOBAL VARIABLES *******************************//

char schedule[6][12][70];       // represent a schedule
int mainArrSize = 0;            // represent the number of courses
char dayHours[][6] = { "8:30","9:30","10:30", "11:30", "12:50", "13:50","14:50", "15:50","16:50", "17:50" ,"18:50", "19:50" };
char weekDays[][10] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };
int NoSchedule = 0;              // schedule number

// ******************************* FUNCTIONS *******************************//


// Save all the courses information in structures:::

Pcourses Read_file_line(Pcourses mainArr)
{
	FILE* fp, *errorsFile;
	char line[128];
	int i, j, cnt;

	if (!(errorsFile = fopen("Errors.txt", "w")))
	{
		//HANDLE THE CASE
		exit(1);
	}

	if (!(fp = fopen("CoursesList.csv", "r")))
	{
		//HANDLE THE CASE
		exit(1);
	}

	for (i = 0; i < 6; i++)          // Initiate schedule matrix
		for (j = 0; j < 12; j++)
			strcpy(schedule[i][j], ",");

	for (cnt = 0, i = 0; (line[i] = fgetc(fp)) != EOF; i++)    // To check if the last sentence recived
		if (line[i] == '\n')
		{
			cnt++;
			line[i] = '\0';
			if (!(Valid_line(line, errorsFile, cnt)))
			{
				line[i] = ' ';
				i = -1;
				continue;
			}
			mainArr = Analize_row(mainArr, line);
			line[i] = ' ';
			i = -1;
		}
	fclose(fp);
	fclose(errorsFile);
	return mainArr;
}

Pcourses Analize_row(Pcourses mainArr, char* line)
{
	Plab currLab;
	Pcourses currCourse;
	Placture currLacture;
	Pexe currExe;
	Pdays currDay;
	FILE* fp;
	char row[128];
	int cnt, j, i, flag;
	static int lactOrExe;

	flag = j = i = cnt = 0;

	if (mainArr == NULL)
		currCourse = mainArr;
	else
		currCourse = mainArr + mainArrSize - 1;  

	while ((row[j] = line[i]) != '\0')
	{
		if (row[j] == ',')     
		{
			cnt++;
			if (cnt > 6)
			{
				//	HANDLE THE CASE
				exit(1);
			}

			if (flag)
				switch (cnt)
				{
				case 1:          //Get course
				{
					row[j] = '\0';
					mainArr = Add_course(mainArr, row); 
					row[j] = ' ';
					flag = j = 0;
					currCourse = mainArr + mainArrSize - 1;
					break;
				}

				case 2:
				{
					row[j] = '\0';
					strcat(row, (line + i));
					Add_lacture(currCourse, row);
					lactOrExe = 0;
					return mainArr;
				}

				case 3:
				{
					row[j] = '\0';
					strcat(row, (line + i));
					Add_exerciser(currCourse, row);
					lactOrExe = 1;
					return mainArr;
				}

				case 4:
				{
					row[j] = '\0';
					strcat(row, (line + i));
					Add_laboratory(currCourse, row);
					lactOrExe = 2;
					return mainArr;
				}

				case 5:
				{
					row[j] = '\0';
					strcat(row, (line + i));

					switch (lactOrExe)
					{
					case 0:
					{
						currLacture = currCourse->courseLactures;
						currLacture += (currCourse->courseLacturesSize - 1);

						currLacture->lacturerDays = Allocate_day(currLacture->lacturerDays, currLacture->lactureDayesSize, row);
						currLacture->lactureDayesSize++;
						break;
					}
					case 1:
					{
						currExe = currCourse->courseExe;
						currExe += (currCourse->courseExeSize - 1);

						currExe->exeDays = Allocate_day(currExe->exeDays, currExe->exeDaysSize, row);
						currExe->exeDaysSize++;
						break;
					}

					case 2:
					{
						currLab = currCourse->courseLab;
						currLab += (currCourse->courseLabSize - 1);

						currLab->labDays = Allocate_day(currLab->labDays, currLab->labDaysSize, row);
						currLab->labDaysSize++;
						break;
					}

					}
					return mainArr;
				}

				}
		}

		else
		{
			flag = 1;
			j++;
		}

		i++;
	}
}


Pcourses Add_course(Pcourses mainArr, char* row)
{
	Pcourses tmp;

	tmp = (Pcourses)realloc(mainArr, sizeof(courses)*(mainArrSize + 1));
	if (!tmp)
	{
		//HANDLE THE CASE
		exit(1);
	}

	Initiate_course(tmp + mainArrSize, row);
	return tmp;
}

void Initiate_course(Pcourses currCourse, char* name)
{
	currCourse->courseName = (char*)malloc(strlen(name) + 1);
	if (!(currCourse->courseName))
	{
		//HANDLE THE CASE
		exit(1);
	}

	strcpy(currCourse->courseName, name);

	currCourse->courseExe = NULL;
	currCourse->courseExeSize = 0;
	currCourse->courseLactures = NULL;
	currCourse->courseLacturesSize = 0;
	currCourse->courseLabSize = 0;
	currCourse->courseLab = NULL;
	mainArrSize++;
}

void Add_lacture(Pcourses currCourse, char* row)  // row = lact,,day,time,duration
{
	char name[52];
	int i, lactIndex;
	Placture tmp;

	for (i = 0; (name[i] = row[i]) != ','; i++);
	name[i] = '\0';
	lactIndex = currCourse->courseLacturesSize;

	tmp = (Placture)realloc(currCourse->courseLactures, (lactIndex + 1) * sizeof(lacture));  //Allocate lacture
	if (!tmp)
	{
		//HANDLE THE CASE
		exit(1);
	}

	(tmp + lactIndex)->lacturerName = (char*)malloc(strlen(name) + 1);     //Allocate name
	if (!((tmp + lactIndex)->lacturerName))
	{
		//HANDLE THE CASE
		exit(1);
	}
	strcpy((tmp + lactIndex)->lacturerName, name);
	// Initiate lact filds:
	(tmp + lactIndex)->lacturerDays = NULL;
	currCourse->courseLacturesSize++;

	(tmp + lactIndex)->lacturerDays = Allocate_day(NULL, 0, row + i + 3);
	(tmp + lactIndex)->lactureDayesSize = 1;

	currCourse->courseLactures = tmp;
}

void Add_exerciser(Pcourses currCourse, char* row)
{
	char name[52];
	int i, exeIndex;
	Pexe tmp;

	for (i = 0; (name[i] = row[i]) != ','; i++);
	name[i] = '\0';
	exeIndex = currCourse->courseExeSize;

	tmp = (Pexe)realloc(currCourse->courseExe, (exeIndex + 1) * sizeof(exe));  //Allocate exe
	if (!tmp)
	{
		//HANDLE THE CASE
		exit(1);
	}

	(tmp + exeIndex)->exeName = (char*)malloc(strlen(name) + 1);     //Allocate name
	if (!((tmp + exeIndex)->exeName))
	{
		//HANDLE THE CASE
		exit(1);
	}
	strcpy((tmp + exeIndex)->exeName, name);
	// Initiate lact filds:
	(tmp + exeIndex)->exeDays = NULL;
	currCourse->courseExeSize++;

	(tmp + exeIndex)->exeDays = Allocate_day(NULL, 0, row + i + 2);
	(tmp + exeIndex)->exeDaysSize = 1;

	currCourse->courseExe = tmp;
}

void Add_laboratory(Pcourses currCourse, char* row)
{
	char name[52];
	int i, labIndex;
	Plab tmp;

	for (i = 0; (name[i] = row[i]) != ','; i++);
	name[i] = '\0';
	labIndex = currCourse->courseLabSize;

	tmp = (Plab)realloc(currCourse->courseLab, (labIndex + 1) * sizeof(lab));  //Allocate lab
	if (!tmp)
	{
		//HANDLE THE CASE
		exit(1);
	}

	(tmp + labIndex)->labName = (char*)malloc(strlen(name) + 1);     //Allocate name
	if (!((tmp + labIndex)->labName))
	{
		//HANDLE THE CASE
		exit(1);
	}
	strcpy((tmp + labIndex)->labName, name);
	// Initiate lact filds:
	(tmp + labIndex)->labDays = NULL;
	currCourse->courseLabSize++;

	(tmp + labIndex)->labDays = Allocate_day(NULL, 0, row + i + 1);
	(tmp + labIndex)->labDaysSize = 1;
	currCourse->courseLab = tmp;
}

Pdays Allocate_day(Pdays dayArr, int index, char* row)
{
	char day[12], time[6];
	Pdays tmp;
	int  i, j;



	tmp = (Pdays)realloc(dayArr, (index + 1) * sizeof(days));
	if (!tmp)
	{
		//HANDLE THE CASE
		exit(1);
	}


	for (i = 0; (day[i] = row[i]) != ','; i++);
	day[i] = '\0';
	for (j = 0; (time[j] = row[i + 1]) != ','; j++, i++);
	time[j] = '\0';
	j = atoi(row + 2 + i);

	Put_day_detailes(tmp + index, day, time, j);

	return tmp;
}

void Put_day_detailes(Pdays currDay, char* day, char* time, int duration)
{
	int i;

	currDay->howLong = duration;             //Put duration

	for (i = 0; i < 6; i++)
		if (strcmp(weekDays[i], day) == 0)   //Put day
			currDay->lactureDay = i;
	
	if (time[0] == '0')                      //Put the beginning time 
		time += 1;
	for (i = 0; i < 12; i++)
		if (!(strcmp(time, dayHours[i])))
			currDay->beginTime = i;
}

// End of saving


// Free allocated memory:::

void Delete_hole_mainArr(Pcourses* mainArr)
{
	int numOfLactures, numOfExe, numOfLab;

	while (mainArrSize-- > 0)
	{
		numOfLactures = (*mainArr)[mainArrSize].courseLacturesSize;
		numOfExe = (*mainArr)[mainArrSize].courseExeSize;
		numOfLab = (*mainArr)[mainArrSize].courseLabSize;

		while (numOfLactures-- > 0)
			Delete_lacture(&((*mainArr)[mainArrSize].courseLactures[numOfLactures]));

		while (numOfExe-- > 0)
			Delete_exercise(&((*mainArr)[mainArrSize].courseExe[numOfExe]));

		while (numOfLab-- > 0)
			Delete_lab(&((*mainArr)[mainArrSize].courseLab[numOfLab]));

		free((*mainArr)[mainArrSize].courseName);
	}

	free(*mainArr);
	*mainArr = NULL;
}


void Delete_lacture(Placture lact) {

	if (lact != NULL)
	{
		free(lact->lacturerDays);
		free(lact->lacturerName);
	}

	lact = NULL;
}

void Delete_exercise(Pexe exercise) {

	if (exercise != NULL) {

		free(exercise->exeDays);
		free(exercise->exeName);
	}

	exercise = NULL;
}

void Delete_lab(Plab labor) {

	if (labor != NULL) {

		free(labor->labDays);
		free(labor->labName);
	}

	labor = NULL;
}

// End of free

// Find schdules:::

void Find_schedule(Pcourses mainArr, FILE* fp, int t,/* int** windowlessSaver, int* windowlessSize,*/ int* dayOffSaver, int winFlag, int dayOffFlag)
{
	int i, j, k;

	if (t == mainArrSize)
	{
		NoSchedule++;
		if (winFlag && dayOffFlag)
		{
			if (Check_if_windowless() && Check_for_dayOff(dayOffSaver, dayOffFlag))
				Print_schedule(fp);
		}

		else if (dayOffFlag && Check_for_dayOff(dayOffSaver, dayOffFlag))
			Print_schedule(fp);

		else if (winFlag && Check_if_windowless())
			Print_schedule(fp);

		else if (!(winFlag && dayOffFlag))
		Print_schedule(fp);

		return;
	}

	for (i = 0; i < mainArr[t].courseLacturesSize; i++)
	{
		if (Put_lacture_in_schedule(mainArr[t], i))
		{
			if (mainArr[t].courseExe != NULL && mainArr[t].courseLab == NULL)
			{
				for (j = 0; j < mainArr[t].courseExeSize; j++)
					if (Put_exercise_in_schedule(mainArr[t], j))
					{
						Find_schedule(mainArr, fp, t + 1, /*windowlessSaver, windowlessSize,*/ dayOffSaver, winFlag, dayOffFlag);

						Remove_exercise_from_schedule(mainArr[t].courseExe[j]);
					}
			}
			else if (mainArr[t].courseExe != NULL && mainArr[t].courseLab != NULL)
			{
				for (j = 0; j < mainArr[t].courseExeSize; j++)
					if (Put_exercise_in_schedule(mainArr[t], j))
					{
						for (k = 0; k < mainArr[t].courseLabSize; k++)
							if (Put_laboratory_in_schedule(mainArr[t], k))
							{
								Find_schedule(mainArr, fp, t + 1,/* windowlessSaver, windowlessSize,*/ dayOffSaver, winFlag, dayOffFlag);
								Remove_laboratory_from_schedule(mainArr[t].courseLab[k]);
							}
						Remove_exercise_from_schedule(mainArr[t].courseExe[j]);
					}
			}
			else
				Find_schedule(mainArr, fp, t + 1, /*windowlessSaver, windowlessSize,*/ dayOffSaver, winFlag, dayOffFlag);

			Remove_lacture_from_schedule(mainArr[t].courseLactures[i]);
		}
	}
	return;
}

int Check_if_windowless(/*int **windowlessSaver, int* windowlessSize,int dayOffFlag*/)
{
	int i, j, flag, win/*, *tmp*/;

	for (i = 0; i < 6; i++)
	{
		flag = 0;
		win = 0;
		for (j = 0; j < 12; j++)
		{
			if (strcmp(schedule[i][j], ",")) //Enter when schdule != ","
			{
				if (win)
					return 0;
				flag = 1;
			}
			else if (flag)
				win = 1;
		}
	}

	//if (dayOffFlag == 0)
	//{
	//	(*windowlessSize)++;
	//	tmp = (int*)realloc(*windowlessSaver, sizeof(int)*(*windowlessSize));
	//	tmp[*windowlessSize - 1] = NoSchedule;
	//	*windowlessSaver = tmp;
	//	return 1;
	//}
	//return 2; // if we need to check for free day
	return 1;
}

int Check_for_dayOff(int *dayOffSaver, int dayOffFlag/*, int** windowlessSaver, int * windowlessSize*/)
{
	int i, j, cnt;

	if (dayOffFlag == -1)
	{
		for (i = 0; i < 6; i++)
			if (dayOffSaver[i] == 1)
				for (j = 0; j < 12; j++)
					if (strcmp(schedule[i][j], ","))//////////////////////////////////////////////////////////////////////////////////////////////////
					{
						return 0;
					}
	}
	else
	{
		for (i = 0; i < 6; i++)
		{
			cnt = 0;
			for (j = 0; j < 12; j++)
				if (strcmp(schedule[i][j], ",") == 0)
				{
					cnt++;
				}
			if (cnt == 12)
				dayOffFlag--;
		}
		if (dayOffFlag > 0)
			return 0;
	}
	//(*windowlessSize)++;
	//tmp = (int*)realloc(*windowlessSaver, sizeof(int)*(*windowlessSize));
	//tmp[*windowlessSize - 1] = NoSchedule;
	//*windowlessSaver = tmp;
	return 1;
}

int Put_lacture_in_schedule(courses course, int lactureIndex)
{
	int i, j;
	lacture lact;

	lact = course.courseLactures[lactureIndex];

	for (i = 0; i < lact.lactureDayesSize; i++)
	{
		j = 0;
		while (j < lact.lacturerDays[i].howLong)
		{

			if (strcmp(schedule[lact.lacturerDays[i].lactureDay][lact.lacturerDays[i].beginTime + j], ",") == 0)
			{
				strcpy(schedule[lact.lacturerDays[i].lactureDay][lact.lacturerDays[i].beginTime + j], course.courseName);
				strcat(schedule[lact.lacturerDays[i].lactureDay][lact.lacturerDays[i].beginTime + j], " with ");
				strcat(schedule[lact.lacturerDays[i].lactureDay][lact.lacturerDays[i].beginTime + j], lact.lacturerName);
				strcat(schedule[lact.lacturerDays[i].lactureDay][lact.lacturerDays[i].beginTime + j], "- lacture,");
			}

			else
			{
				while (i >= 0)
				{
					for (j--; j >= 0; j--)
						strcpy(schedule[lact.lacturerDays[i].lactureDay][lact.lacturerDays[i].beginTime + j], ",");
					i--;
					j = lact.lacturerDays[i].howLong;
				}
				return 0;
			}
			j++;
		}
	}

	return 1;
}

int Put_exercise_in_schedule(courses course, int exeIndex)
{
	int i, j;
	exe exercise;

	exercise = course.courseExe[exeIndex];

	for (i = 0; i < exercise.exeDaysSize; i++)
	{
		j = 0;
		while (j < exercise.exeDays[i].howLong)
		{

			if (strcmp(schedule[exercise.exeDays[i].lactureDay][exercise.exeDays[i].beginTime + j], ",") == 0)
			{
				strcpy(schedule[exercise.exeDays[i].lactureDay][exercise.exeDays[i].beginTime + j], course.courseName);
				strcat(schedule[exercise.exeDays[i].lactureDay][exercise.exeDays[i].beginTime + j], " with ");
				strcat(schedule[exercise.exeDays[i].lactureDay][exercise.exeDays[i].beginTime + j], exercise.exeName);
				strcat(schedule[exercise.exeDays[i].lactureDay][exercise.exeDays[i].beginTime + j], "- exercise,");
			}

			else
			{
				while (i >= 0)
				{
					for (j--; j >= 0; j--)
						strcpy(schedule[exercise.exeDays[i].lactureDay][exercise.exeDays[i].beginTime + j], ",");
					i--;
					j = exercise.exeDays[i].howLong;
				}
				return 0;
			}
			j++;
		}
	}

	return 1;
}

int Put_laboratory_in_schedule(courses course, int labIndex)
{
	int i, j;
	lab labor;

	labor = course.courseLab[labIndex];

	for (i = 0; i < labor.labDaysSize; i++)
	{
		j = 0;
		while (j < labor.labDays[i].howLong)
		{

			if (strcmp(schedule[labor.labDays[i].lactureDay][labor.labDays[i].beginTime + j], ",") == 0)
			{
				strcpy(schedule[labor.labDays[i].lactureDay][labor.labDays[i].beginTime + j], course.courseName);
				strcat(schedule[labor.labDays[i].lactureDay][labor.labDays[i].beginTime + j], " with ");
				strcat(schedule[labor.labDays[i].lactureDay][labor.labDays[i].beginTime + j], labor.labName);
				strcat(schedule[labor.labDays[i].lactureDay][labor.labDays[i].beginTime + j], "- laboratory,");
			}

			else
			{
				while (i >= 0)
				{
					for (j--; j >= 0; j--)
						strcpy(schedule[labor.labDays[i].lactureDay][labor.labDays[i].beginTime + j], ",");
					i--;
					j = labor.labDays[i].howLong;
				}
				return 0;
			}
			j++;
		}
	}

	return 1;
}

void Remove_exercise_from_schedule(exe exercise)
{
	int i, j;

	i = exercise.exeDaysSize - 1;

	while (i >= 0)
	{
		for (j = 0; j < exercise.exeDays[i].howLong; j++)

			strcpy(schedule[exercise.exeDays[i].lactureDay][exercise.exeDays[i].beginTime + j], ",");

		i--;
	}
}

void Remove_lacture_from_schedule(lacture lact)
{
	int i, j;

	i = lact.lactureDayesSize - 1;

	while (i >= 0)
	{
		for (j = 0; j < lact.lacturerDays[i].howLong; j++)

			strcpy(schedule[lact.lacturerDays[i].lactureDay][lact.lacturerDays[i].beginTime + j], ",");
		i--;
	}
}

void Remove_laboratory_from_schedule(lab labor)
{
	int i, j;

	i = labor.labDaysSize - 1;

	while (i >= 0)
	{
		for (j = 0; j < labor.labDays[i].howLong; j++)

			strcpy(schedule[labor.labDays[i].lactureDay][labor.labDays[i].beginTime + j], ",");
		i--;
	}
}


int Valid_line(char* line, FILE* writeError, int x)
{
	int i, j, k, m;
	char numOfLine[4];
	char errorMsg[50];
	char missingData[37] = "There is a missing data in line ";

	itoa(x, numOfLine, 10);
	strcat(missingData, numOfLine);

	for (i = 0; line[i] != ',' && line[i] != '\0'; i++);

	if (!Check_end_of_line(line, i, missingData, writeError))
		return 0;


	if (i == 0) // not course name  ','
	{
		for (j = i + 1; line[j] != ',' && line[j] != '\0'; j++);

		if (!Check_end_of_line(line, j, missingData, writeError))
			return 0;


		if (j == i + 1) // maybe exe or lab   ',,'
		{
			for (k = j + 1; line[k] != ',' && line[k] != '\0'; k++);

			if (!Check_end_of_line(line, k, missingData, writeError))
				return 0;

			if (k == j + 1) // of lab or day ',,,'
			{
				for (m = k + 1; line[m] != ',' && line[m] != '\0'; m++);

				if (!Check_end_of_line(line, m, missingData, writeError))
					return 0;

				strcpy(errorMsg, Valid_date(line + m + 1, numOfLine));
				if (strcmp(errorMsg, "Ok") != 0)
				{
					fprintf(writeError, "%s\n", errorMsg);
					return 0;  // check date of exe or date
				}
				return 1;
			}

			else         // exe
			{
				if (line[k + 1] == ',')
				{// ',, nameOfExe , '

					strcpy(errorMsg, Valid_date(line + k + 2, numOfLine));
					if (strcmp(errorMsg, "Ok") != 0)
					{
						fprintf(writeError, "%s\n", errorMsg);
						return 0;  // check date of exe or date
					}
					return 1;
				}
				else
				{
					fprintf(writeError, "problem in line %d, exerciser & laboratory can't be in the same line", numOfLine);
					return 0;
				}
			}
		}

		else // lacturer
		{
			if (line[j + 2] == ',' && line[j + 1] == ',')
			{
				strcpy(errorMsg, Valid_date(line + j + 3, numOfLine));
				if (strcmp(errorMsg, "Ok") != 0)
				{
					fprintf(writeError, "%s\n", errorMsg);
					return 0;
				}
				return 1;
			}
			else
			{
				fprintf(writeError, "problem in line %s, lacturer & (exerciser or laboratory) can't be in the same line", numOfLine);
				return 0;
			}
		}
	}

	else
	{
		for (j = i + 1; line[j] != ',' && line[j] != '\0'; j++);
		if (j == i + 1)
		{
			fprintf(writeError, "problem in line %s , course without a lacture", numOfLine);
			return 0;
		}
		else
		{
			if (!Check_end_of_line(line, j, missingData, writeError))
				return 0;


			if (line[j + 1] != ',' || line[j + 1] != ',')
			{
				fprintf(writeError, "Error input in line %s", numOfLine);
				return 0;
			}
			else
			{
				strcpy(errorMsg, Valid_date(line + j + 3, numOfLine));
				if (strcmp(errorMsg, "Ok") != 0)
				{
					fprintf(writeError, "%s\n", errorMsg);
					return 0;
				}
				return 1;
			}
		}
	}
}

int Check_end_of_line(char* line, int index, char* errorMsg, FILE* writeError)
{
	if (line[index] == '\0')
	{
		fprintf(writeError, "%s", errorMsg);
		return 0;
	}
	return 1;
}

char* Valid_date(char* line, char* x)  // x==number of line
{
	int i, j;
	char day[10], time[6];
	char errorDay[30] = "Error day input in line ";
	char errorTime[30] = "Error time input in line ";
	char errorDuration[30] = "Error duration input in line ";

	for (i = 0; line[i] != ',' && line[i] != '\0'; i++)
		day[i] = line[i];
	day[i] = '\0';

	if (!Valid_day_line(day))
	{
		strcat(errorDay, x);
		return errorDay;
	}

	for (i++, j = 0; line[i] != ',' && line[i] != '\0'; i++, j++)
		time[j] = line[i];
	time[j] = '\0';

	if (!Valid_time_line(time))
	{
		strcat(errorTime, x);
		return errorTime;
	}

	if (!Valid_duration_line(line + i + 1))
	{
		strcat(errorDuration, x);
		return errorDuration;
	}
	return "Ok";
}




int Valid_duration_line(char* howLong)
{
	char duration[][2] = { "1","2","3","4","5" };
	int i;

	for (i = 0; i < 5; i++)
		if (strcmp(duration[i], howLong))
			return 1;
	return 0;
}

int Valid_day_line(char* name)
{
	int i;

	for (i = 0; i < 6; i++)
		if (strcmp(name, weekDays[i]) == 0)
			return 1;
	return 0;
}

int Valid_time_line(char* time)
{
	int i;

	if (time[0] == '0')
	{
		for (i = 0; i < 12; i++)
			if (!(strcmp((time + 1), dayHours[i])))
				return 1;
	}
	else
		for (i = 0; i < 12; i++)
			if (!(strcmp(time, dayHours[i])))
				return 1;

	return 0;
}


void Print_schedule(FILE* fp)
{
	int i, j;


	for (i = 0; i < 6; i++)
		fprintf(fp, ",%s", weekDays[i]);
	fprintf(fp, ",%d", NoSchedule);
	fprintf(fp, "\n");

	for (i = 0; i < 12; i++)
	{
		fprintf(fp, "%s,", dayHours[i]);
		for (j = 0; j < 6; j++)
			fprintf(fp, "%s", schedule[j][i]);
		fprintf(fp, "\n");
	}

	fprintf(fp, "\n\n");
}

int Ask_for_windowless()
{
	int flag;
	printf("\nwould you like to get your schedule windowless? < Press '1' for YES and '0' for NO >  ");
	scanf("%d", &flag);
	return (flag == 1);
}

int Ask_for_dayoff(int* dayOffSaver)
{
	int flag, day = 0, chosenDays = 0, cnt, tmp;

	printf("\nwould you like to get your schedule with day/s off? < Press '1' for YES and '0' for NO >  ");
	scanf("%d", &flag);
	if (flag == 0)
		return 0;

	printf("\nHow many days whould you like to take off in week? < 1 - 6 >  ");
	while (1)
	{
		scanf("%d", &cnt);
		if (cnt == 0)
			return 0;
		if (cnt > 0 && cnt < 6)
			break;
		printf("\nError input, please try again, how many days whould you like to take of?: ");
	}

	printf("\nWould you like to choos the days? < 1 for yes >  ");
	scanf("%d", &chosenDays);

	if (chosenDays == 1)
	{
		tmp = cnt;
		printf("\nDays : 1- Sunday , 2- Monday , 3- Tuesday , 4- Wednesday , 5- Thursday , 6- Friday\n");
		cnt = -1;

		while (tmp)
		{
			printf("\nEnter the day/s off you like to take: ");
			scanf("%d", &day);

			if (!(day > 0 && day < 7))
			{
				printf("\nWrong input, Enter a number between 1 to 6");
				continue;
			}
			dayOffSaver[day - 1] = 1;
			tmp--;
		}
	}
	return cnt;   // return negative if he chose the days and positive if he's not chose the days- that's mean return the number of days
}

//void Find_the_wanted_schedule(int dayOffFlag, int winFlag, int* windowlessSaver, int windowlessSize)
//{
//	int tmp, i;
//
//	tmp = dayOffFlag + winFlag;
//
//	switch (tmp)
//	{
//	case 0:
//		return;
//
//	case 1:
//	{
//		if (dayOffFlag == 1)
//			printf("Those are the schedules with your day's constraints:\n");
//		else
//			printf("Those are the schedules without wondows:\n");
//		for (i = 0; i < windowlessSize; i++)
//			printf("%d  ", windowlessSaver[i]);
//		return;
//	}
//
//	case 2:
//	{
//		printf("Those are the schedules with your constraints:\n");
//		for (i = 0; i < windowlessSize; i++)
//			printf("%d  ", windowlessSaver[i]);
//		return;
//	}
//
//	}
//}