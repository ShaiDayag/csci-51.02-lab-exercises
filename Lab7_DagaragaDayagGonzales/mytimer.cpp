/*
Pete Justin Dagaraga (231874)
Shaan Graal Dayag (231928)
Robynne Alexa Gonzales (232944)

March 23, 2026
*/


/*
We have not discussed the C++ language code and Bash scripting in our program with anyone other than our instructor or the teaching assistants assigned to this course.
We have not used C++ language code and Bash scripting obtained from another student, or any other unauthorized source, either modified or unmodified.
If any C++ language code and Bash scripting or documentation of either were used in our program was obtained from another source, such as a textbook or course notes, that has been clearly noted with a proper citation in the comments of our program.
*/

#include <iostream>
#include <unistd.h> // For POSIX support
#include <stdio.h>	// For snprintf
#include <fstream>	// Text file reading
#include <string>	// String manipulation
#include <ctime>	// To display time
#include <chrono>	// To provide seconds for the wait
#include <thread>	// Allows a process to wait
using namespace std;

int main(int argc, char* argv[]){
	// Get the parent's PID, then convert it into String
	pid_t parentID = getpid();
	char parentIDstring[10];
	snprintf(parentIDstring, 11, "%d", parentID);

	// Sets up defunct process detection command for later
	string isEndChildren =  "pgrep -f --parent=";
	isEndChildren += parentIDstring;
	isEndChildren.append(" defunct >> defunctprocess.txt");

	// Sets up part of the line to be written in a later script 
	string endAll =  "echo \"pgrep --parent=";
	endAll += parentIDstring;
	endAll.append(" | xargs kill > /dev/null\" >> finalclear.sh");

	if( fork() == 0 ){
		// For XClock
		
		// Determines the path of the xclock program, and writes it to a temporary file
		system("whereis xclock | cut -d' ' -f2 >> xclock_path.txt");
		
		// Opens the temporary file to store the path in a string variable, then deletes the file
		ifstream file("xclock_path.txt");
		string line;
		if( file.is_open() ){
			getline(file, line);
			file.close();
			system("rm xclock_path.txt");
		}else{
			cerr << "Can\'t open file!" << endl;
		}

		// With the path now available, use it to run the myXclock process using execl
		const char* path = line.c_str();
		if( execl(path, "myXclock", NULL) == -1){
			cout << "It didn't work" << endl;
		}

	} else if ( fork() == 0 ){
		// For Waiting for Enter Input

		// Just waits for user to press Enter. Any input given here is irrelevant to the running of the program
		string waitEnter;
		getline(cin, waitEnter);

		// Prints out terminated when this process ends as intended
		cout << "\"Terminated\"" << endl;

	} else {
		// For Parent: Display Date and Double Checking Children Process State
		
		// Initialize important variables
		int counter = 0;
		int activeChildren = 2;
		int relevantDefunctCount = -1; // Starts here as when searching for defunct processes that were made by parent, that also includes one that is not necessary an important child process to be tracked

		while(activeChildren == 2){
			
			// Obtains and then displays current date
			time_t timestamp = time(NULL);
			struct tm datetime = *localtime(&timestamp);
			char dateoutput[22];

			// [YYYY-MM-DD] HH:MM:SS
			strftime(dateoutput, 22, "[%F] %T", &datetime);
			cout << dateoutput << endl;

			// Increments counter. When at 3, additional message is printed.
			counter++;
			if(counter == 3){
				cout << "\"This program has gone on for far too long. Close the myXclock window or press Enter on this window to exit.\"" << endl;
				counter = 0; // Reset counter back to 0
			}

			// Instead of busy waiting by endless looping, utilize sleep.
			this_thread::sleep_for(chrono::seconds(3));

			// Checks for all recently defunct processes, and sends them to a temporary file
			system(isEndChildren.c_str());

			// Opens temporary file to count number of defunct processes
			ifstream file("defunctprocess.txt");
			string linechecker;
			if( file.is_open() ){

				// For every defunct process, increase relevantDefunctCount by 1. After all processes have been determined, delete the temporary file
				while(getline(file, linechecker)){
					relevantDefunctCount++;
				}
				file.close();
				system("rm defunctprocess.txt");
			}else{
				cerr << "Can\'t open file!" << endl;
			}

			// Informs parent on whether it should terminate
			activeChildren -= relevantDefunctCount;

			// Reset back to initial value
			relevantDefunctCount = -1;
		}

		// When parent is about to terminate, it also terminates its other child processes
		string lastScript = "echo \"#/bin/bash\" >> finalclear.sh | echo \"\" >> finalclear.sh | ";
		lastScript.append(endAll);
		lastScript.append("| echo \"rm finalclear.sh\" >> finalclear.sh"); // Ensures temporary script is self-deleting
		system(lastScript.c_str());
		system("chmod u+x finalclear.sh");
		system("./finalclear.sh"); // Terminates the two children and one parent processes
	}
}
