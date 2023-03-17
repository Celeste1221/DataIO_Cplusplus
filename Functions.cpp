//Functions.cpp

//preprocessor directives
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <iomanip>
#include "Functions.h"
using namespace std;

//print course header to console
void WriteHeader()
{
	cout << "By Celeste Fischer\n"
		<< "This program will analyze call response times based on the incoming contact time and the outgoing contact time "
		<< "for 2 different encounter types: \"nurse triage\" and \"telephone\".\n"
		<< "The results will be the percent of each type of encounter with response times under 2 hours, "
		<< "as well as the percent of all calls with response times under two hours.\n"
		<< "These results will be saved to a text file and written to the console.\n";
}
 
//converts incoming and outgoing strings which are in military time format (13:30) to int minutes from midnight (810),
//and subtracts outgoing from incoming; takes in struct object; OpenAndReadFile will call this to fill struct.delta object
int ConvertTime(encounters calcs)
{
	int incomingMinutes = 0;
	int outgoingMinutes = 0;
	
	//for strings that are 5 spaces long, calculate minutes elapsed between incoming and outgoing
	if (calcs.incoming.length() == 5)
	{
		//read everything to the left of the colon as one int, assign to a variable
		stringstream incTempHour;
		incTempHour << calcs.incoming[0] << calcs.incoming[1];

		//read everything to the right side of the colon as one int, assign to a variable
		stringstream incTempMin;
		incTempMin << calcs.incoming[3] << calcs.incoming[4];

		//multiply number on left of the colon by 60, then add that result to the number on the right of the colon ex 13:30 to (13*60)+30=810
		incomingMinutes = (stoi(incTempHour.str()) * 60) + (stoi(incTempMin.str()));
	}
	else if (calcs.incoming.length() == 4) //for strings that are 4 spaces long, calculate minutes elapsed between incoming and outgoing
	{
		//read everything to the left of the colon as one int, assign this to a variable
		stringstream incTempHour;
		incTempHour << calcs.incoming[0];

		//read everything to the right side of the colon as one int, assign this to a variable
		stringstream incTempMin;
		incTempMin << calcs.incoming[2] << calcs.incoming[3];

		incomingMinutes = (stoi(incTempHour.str()) * 60) + (stoi(incTempMin.str()));
	}

	if (calcs.outgoing.length() == 5)
	{
		//read everything to the left of the colon as one int, assign this to a variable
		stringstream outgTempHour;
		outgTempHour << calcs.outgoing[0] << calcs.outgoing[1];

		//read everything to the right side of the colon as one int, assign this to a variable
		stringstream outgTempMin;
		outgTempMin << calcs.outgoing[3] << calcs.outgoing[4];

		//multiply number on left of the colon by 60, then add that result to the number on the right of the colon ex 13:30 to (13*60)+30=810
		outgoingMinutes = (stoi(outgTempHour.str()) * 60) + (stoi(outgTempMin.str()));
	}
	else if (calcs.outgoing.length() == 4)
	{
		//read everything to the left of the colon as one int, assign this to a variable
		stringstream outgTempHour;
		outgTempHour << calcs.outgoing[0];

		//read everything to the right side of the colon as one int, assign this to a variable
		stringstream outgTempMin;
		outgTempMin << calcs.outgoing[2] << calcs.outgoing[3];

		//multiply number on left of the colon by 60, then add that result to the number on the right of the colon ex 13:30 to (13*60)+30=810
		outgoingMinutes = (stoi(outgTempHour.str()) * 60) + (stoi(outgTempMin.str()));
	}
	
	//if outgoing minutes is less than incoming minutes, need to add 1440 minutes (one day) for calculation
	int totalTime;
	if (outgoingMinutes < incomingMinutes)
	{
		totalTime = (outgoingMinutes + 1440) - incomingMinutes;
	}
	else
	{
		totalTime = outgoingMinutes - incomingMinutes;
	}
	return totalTime;
}

//open input file and read data; takes in a file name and a reference to an encounters vector 
bool OpenAndReadFile(string fileName, vector <encounters> &myVector)
{
	bool skipLine = false;
	string line;
	int encountersKey = 1; 	//	put key outside of outer while loop

	ifstream inputFile; //for reading in from a file
	inputFile.open(fileName); //open file
	if (inputFile.is_open()) //if file is open, read in data to vector of structs, return true
	{
		while (!inputFile.eof()) //read in data from file, populate vector
		{
			getline(inputFile, line, '\n'); //parses line out of document (splits string on the newline character) 
			encounters encData; //struct object to hold data
			stringstream templine(line); //convert string to stringstream so getline can parse
			string data;
			int dataCounter = 0;  //counter for iterating through columns to read in

			//while loop parses each line by comma and add data to structure
			while (getline(templine, data, ',')) //iterates through comma separated columns
			{
				if (data == "Encounter Type" || data == "Incoming Time" || data == "Outgoing Time")
				{
					if (dataCounter == 2) //skip top line in csv file
					{
						encountersKey--; //keeps key to correct row
						skipLine = true;
					}
				}
				else //populate struct with each piece of data
				{
					stringstream encounterType;
					encounterType << "encounterData_" << encountersKey;
					encData.key = encounterType.str(); //converts fromt stringstream into string
					if (dataCounter == 0)
					{
						encData.type = data;
					}
					else if (dataCounter == 1)
					{
						encData.incoming = data;
					}
					else
					{
						encData.outgoing = data;
					}
				}
				dataCounter++;  //new line
			}
			//fill vector right after while loop 
			encountersKey++;
			if (skipLine == false) //if not reading the first/header line 
			{
				encData.delta = ConvertTime(encData); //call ConvertTime function to populate delta
				myVector.push_back(encData); //fill vector
			}
			else
			{
				skipLine = false;
			}
		}
		//close file right before return
		inputFile.close();
		return true;
	}
	else //if file not open, tell user, return false
	{
		return false;
	}
}

//Function calculate percent of nurse triage encounters less than 120 min
float PercentNurseTriage(vector <encounters> &myVector)
{
	//iterate through each encounter to count number of encounters that are nurse triage AND delta is <= 120, 
	//create counter to count them; divide the count by the size of vector and multiply by 100 for percent
	float encCounter = 0.0;
	float nurseTriageCounter = 0.0;
	for (int i = 0; i < myVector.size(); i++)
	{
		if (myVector[i].type == "nurse triage") //total of nurse triage type encounters
		{
			nurseTriageCounter++;
		}
		if (myVector[i].type == "nurse triage" && myVector[i].delta <= 120)
		{
			encCounter++;
		}
	}
	if (myVector.size() > 0) //don't divide by zero
	{
		return (encCounter / nurseTriageCounter) * 100.0; 
	}
	return -1.0;
}

//Function to calculate percent of telephone encounters less than 120 min
float PercentTelephone(vector <encounters> &myVector)
{
	//iterate through each encounter to count number of encounters that are telephone AND delta is <= 120, 
	//create counter to count them; divide the count by the size of vector and multiply by 100 for percent
	float encCounter = 0.0;
	float teleCounter = 0.0;
	for (int i = 0; i < myVector.size(); i++)
	{
		if (myVector[i].type == "telephone") //total of telephone type encounters
		{
			teleCounter++;
		}
		if (myVector[i].type == "telephone" && myVector[i].delta <= 120)
		{
			encCounter++;
		}
	}
	if (myVector.size() > 0) //don't divide by zero
	{
		return (encCounter / teleCounter) * 100.0; 
	}
	return -1.0;
}

//Function to calculate percent of all encounters less than 120 min
float PercentTotal(vector <encounters> &myVector)
{
	//iterate through each encounter to count number of encounters where delta is <= 120, 
	//create counter to count them; divide the count by the size of vector and multiply by 100 for percent
	float encCounter = 0.0;
	for (int i = 0; i < myVector.size(); i++)
	{
		if (myVector[i].delta <= 120)
		{
			encCounter++;
		}
	}
	if (myVector.size() > 0) //don't divide by zero
	{
		return (encCounter / myVector.size()) * 100.0; 
	}
	return -1.0;
}

//Function to write to file, takes in output file name, responseTimes vector, monthlyData struct, and pointer to var declared in main
bool WriteFileOut(vector <encounters> &myVector, monthlyData data, string *writeFile, string *summaryString)
{
	//use to output data analysis summary to file
	ofstream outputFile;
	//ask for summary output file name
	cout << "\nPlease enter a text file name to save this summary: (example.txt) \n";
	getline(cin, *writeFile);
	
	//open output file
	outputFile.open(*writeFile);
	if (outputFile.is_open()) 
	{
		stringstream ss;
		ss.setf(ios::fixed);
		ss << setprecision(2);
		ss << "\nCeleste Fischer\n"
			<< "The data analysis results have been saved in the file: "<< *writeFile << "\n"
			<< "Here are the metrics for the percent of encounters with response times under 2 hours, categorized by encounter types:\n"
			<< data.nurseTriage << "% of nurse triage encounters had response times under 2 hours. \n"
			<< data.telephone << "% of telephone encounters had response times under 2 hours. \n"
			<< data.total << "% of all encounters had response times under 2 hours. \n";

		outputFile << ss.str();
		*summaryString = ss.str(); //returns string to main through the function as a pointer variable
		return true;	
	}
	return false;
}


