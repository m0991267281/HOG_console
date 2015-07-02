// Webcam_Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cv.h>
#include "./Feature/HOGFeature.h"

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

#define MAXNAME 40
#define FEATURE_LIST "./picture_list.txt"

#define NEGATIVE_COEFFICIENT "-1"
#define NEGATIVE_SAMPLE "./Sample/Negative_Sample_0/"

#define POSITIVE_COEFFICIENT "1"
#define POSITIVE_SAMPLE_1 "./Sample/Positive_Sample_1/"
#define POSITIVE_SAMPLE_2 "./Sample/Positive_Sample_2/"

#define TRAINING_DATA "./TrainingData"
#define TESTING_DATA "./TestingData"


using namespace std;

int Training_Or_Testing_MakeData( char * DirName, char * FileList, char *TrainOrTest_Dir, char * Coefficient );
int Read_Dir_FileName( char* DirName, char* FileList );


int _tmain(int argc, _TCHAR* argv[])
{
	char * Negative_Sample_Dir = new char();
	Negative_Sample_Dir = NEGATIVE_SAMPLE;

	char * FileList = new char();
	FileList = FEATURE_LIST;

	char * Train_Dir = new char();
	Train_Dir = TRAINING_DATA;

	char * Negative_Coefficient = new char();
	Negative_Coefficient = NEGATIVE_COEFFICIENT;

	Training_Or_Testing_MakeData( Negative_Sample_Dir, FileList, Train_Dir, Negative_Coefficient );

	char * Positive_Sample_Dir_1 = new char();
	Positive_Sample_Dir_1 = POSITIVE_SAMPLE_1;

	char * Positive_Coefficient = new char();
	Positive_Coefficient = POSITIVE_COEFFICIENT;

	Training_Or_Testing_MakeData( Positive_Sample_Dir_1, FileList, Train_Dir, Positive_Coefficient );

	
	// ------------------------------------------------------------------------------------------------------

	char * Positive_Sample_Dir_2 = new char();
	Positive_Sample_Dir_2 = POSITIVE_SAMPLE_2;
	// 	Training_Or_Testing_MakeData( Positive_Sample_Dir_2, FileList, Train_Dir, Positive_Coefficient );
	char * Test_Dir = new char();
	Test_Dir = TESTING_DATA;
	Training_Or_Testing_MakeData( Positive_Sample_Dir_2, FileList, Test_Dir, Positive_Coefficient );
	
	return 0;
}



int Training_Or_Testing_MakeData( char * DirName, char * FileList, char *TrainOrTest_Dir, char * Coefficient )
{
	//char * DirName = new char();

	// Negative Sample
	//DirName = NEGATIVE_SAMPLE;

	// Positive Sample
	//DirName = NEGATIVE_SAMPLE;

	//char * FileList = new char();
	//FileList = FEATURE_LIST;

	Read_Dir_FileName( DirName, FileList );


	ifstream infile( FileList );
	string Source,Source_TMP;

	Source_TMP = DirName ;

	FILE *fp;

	// TRAIN
	fp = fopen( TrainOrTest_Dir,"a");

	// TEST
	//fp = fopen(TESTING_DATA,"w");

	while( getline(infile, Source) != NULL ){

		Source = Source_TMP + Source;
		const char * Source_Ptr = Source.c_str();
		
		printf("%s\n", Source_Ptr);

		// Input the Image
		IplImage* Img_buff_Ptr = cvLoadImage( Source_Ptr,1);

		CHOGFeature CMyHOG;	
		CMyHOG.Init();

		int iFeatureDim = CMyHOG.GetFeatureDim(64,128);

		CvMat* MFeature = cvCreateMat(1,iFeatureDim,CV_32FC1);
		float* pfFeature = CMyHOG.GetFeature( Img_buff_Ptr );

		memcpy( MFeature->data.fl, pfFeature,iFeatureDim*sizeof(float));

		// Negative Sample
		fprintf(fp,"%s ",Coefficient);

		// Positive Sample
		//fprintf(fp,"%s ",POSITIVE_SAMPLE_1);

		//fprintf(fp,"%s ",POSITIVE_SAMPLE_2);


		for(int j = 0;j < iFeatureDim;j++)
		{

			if((MFeature->data.fl[j]))

				fprintf(fp,"%d:%f ",j+1,MFeature->data.fl[j]);

		}

		fprintf(fp,"\n");

	}

	return 0;

}




int Read_Dir_FileName( char* DirName, char* FileList )
{
	DIR *dp;
	struct dirent *ep;

	dp = opendir ( DirName );
	if (dp != NULL)
	{
		FILE * fp = fopen( FileList, "w");

		int Jump_for_dot_counter = 0;
		while ( ep = readdir (dp) )
		{
			puts (ep->d_name);
			Jump_for_dot_counter++;

			if ( Jump_for_dot_counter >= 3 )
			{
				fprintf(fp, "%s \n", ep->d_name);
			}

		}

		fclose(fp);

		(void) closedir (dp);
	}
	else
	{
		perror ("Couldn't open the directory");
	}

	//system("pause");
	return 0;

}
