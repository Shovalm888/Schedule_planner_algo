#include"ProgHeader.h"
#include<conio.h>
// ******************************* MAIN *******************************//


int main() {
	FILE* fp;
	Pcourses mainArr = NULL;
	int  /*windowlessSize = 0,*/ winFlag;         // infoSize == infoSaver size
	/*int* windowlessSaver = NULL;*/      // Pointer that will save the number of all the windowless schedules
	int  dayOffFlag;
	int* dayOffSaver[6];

	mainArr = Read_file_line(mainArr);

	winFlag = Ask_for_windowless();

	dayOffFlag = Ask_for_dayoff(dayOffSaver);  // 0- No , pos num -Yes , -1 - Yes & got the days

	fp = fopen("Your_schedule.csv", "w");

	if (!fp)
	{
		//HANDLE THE CASE 
		exit(1);
	}

	Find_schedule(mainArr, fp, 0,/* &windowlessSaver, &windowlessSize,*/ dayOffSaver, winFlag, dayOffFlag);
	
	/*Find_the_wanted_schedule(dayOffFlag, winFlag, windowlessSaver, windowlessSize);*/

	fclose(fp);


	Delete_hole_mainArr(&mainArr);
	/*free(windowlessSaver);*/
	getch();
	return 0;
}









