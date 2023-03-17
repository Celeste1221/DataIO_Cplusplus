//Mary Celeste Fischer
//mfischer7@cnm.edu
//Project 7 Pt 2
//Functions.h

#pragma once

using namespace std;

//struct object for reading in data from file
struct encounters
{
	string key;
	string type;
	string incoming;
	string outgoing;
	int delta;
};
//for summarizing data
struct monthlyData 
{
	float nurseTriage;
	float telephone;
	float total;
};

//function prototypes
void WriteHeader();
int ConvertTime(encounters calcs);
bool OpenAndReadFile(string fileName, vector <encounters> &myVector);
float PercentNurseTriage(vector <encounters> &myVector);
float PercentTelephone(vector <encounters> &myVector);
float PercentTotal(vector <encounters> &myVector);
bool WriteFileOut(vector <encounters> &myVector, monthlyData data, string *writeFile, string *summaryString);

