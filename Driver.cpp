//Driver.cpp

//preprocessor directives
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "Functions.h"
using namespace std;

int main() 
{
	//declare variables
	vector <encounters> responseTimes; //vector to push struct object data into
	string fileToOpen = "EncountersFile.csv"; //file to read in from
	monthlyData analyzedData{}; //struct for processed data: percent of encounters with response times (delta) <= 2 hours by type
	string writeFileReturn;
	string outputFile;
	string summary;

	//display header
	WriteHeader();

	//read in data and return bool to indicate success or failure, check in main and notify user
	bool checkFunction = OpenAndReadFile(fileToOpen, responseTimes);
	if (checkFunction == true)
	{
		cout << "\nFile opened successfully. Importing data from file.\n";
	}
	else
	{
		cout << "\nFile not found.\n\n";
	}

	//analyze data and produce 3 data facts
	analyzedData.nurseTriage = PercentNurseTriage(responseTimes);
	analyzedData.telephone = PercentTelephone(responseTimes);
	analyzedData.total = PercentTotal(responseTimes);

	//call WriteFileOut to write to output analysis to file and and print to console, 
	//check to see if successful, notify user
	if ((WriteFileOut(responseTimes, analyzedData, &outputFile, &summary) == true))
	{
		cout << "\nFile successfully saved.\n"
			<< summary; //this is the string variable that contains the summary analysis, 
		//it is returned to main via pointer through the function
	}
	else
	{
		cout << "\nUnable to write to file.\n";
	}
	return 0;
}