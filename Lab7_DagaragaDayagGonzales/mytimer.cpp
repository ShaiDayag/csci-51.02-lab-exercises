#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;

int main(int argc, char* argv[]){
	pid_t parentID = getpid();
	char parentIDstring[10];
	snprintf(parentIDstring, 11, "%d", parentID);
	string isEndChildren =  "pgrep -f --parent=";
	isEndChildren += parentIDstring;
	isEndChildren.append(" defunct >> defunctprocess.txt");
	string endAll =  "echo \"pgrep --parent=";
	endAll += parentIDstring;
	endAll.append(" | xargs kill > /dev/null\" >> finalclear.sh");

	if( fork() == 0 ){
		// For XClock
		system("whereis xclock | cut -d' ' -f2 >> xclock_path.txt");
		ifstream file("xclock_path.txt");
		string line;
		if( file.is_open() ){
			getline(file, line);
			file.close();
			system("rm xclock_path.txt");
		}else{
			cerr << "Can\'t open file!" << endl;
		}
		const char* path = line.c_str();
		if( execl(path, "myXclock", NULL) == -1){
			cout << "It didn't work" << endl;
		}
	} else if ( fork() == 0 ){
		// For Wait Enter
		string waitEnter;
		getline(cin, waitEnter);
		cout << "\"Terminated\"" << endl;
	} else {
		// For Parent
		int counter = 0;
		int activeChildren = 2;
		int relevantDefunctCount = -1;
		while(activeChildren == 2){
			time_t timestamp = time(NULL);
			struct tm datetime = *localtime(&timestamp);
			char dateoutput[22];
			strftime(dateoutput, 22, "[%F] %T", &datetime);
			cout << dateoutput << endl;
			counter++;
			if(counter == 3){
				cout << "\"This program has gone on for far too long. Close the myXclock window or press Enter on this window to exit.\"" << endl;
				counter = 0;
			}
			this_thread::sleep_for(chrono::seconds(3));

			system(isEndChildren.c_str());
			ifstream file("defunctprocess.txt");
			string linechecker;
			if( file.is_open() ){
				while(getline(file, linechecker)){
					relevantDefunctCount++;
				}
				file.close();
				system("rm defunctprocess.txt");
			}else{
				cerr << "Can\'t open file!" << endl;
			}
			activeChildren -= relevantDefunctCount;
			relevantDefunctCount = -1;
		}
		string lastScript = "echo \"#/bin/bash\" >> finalclear.sh | echo \"\" >> finalclear.sh | ";
		lastScript.append(endAll);
		lastScript.append("| echo \"rm finalclear.sh\" >> finalclear.sh");
		system(lastScript.c_str());
		system("chmod u+x finalclear.sh");
		system("./finalclear.sh");
	}
}
