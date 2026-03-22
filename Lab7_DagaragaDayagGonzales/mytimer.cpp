#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;

int main(int argc, char* argv[]){
	pid_t parentID = getpid();
	if( fork() == 0 ){
		// Parent PID + 1
		// For XClock (NOT COMPLETE)
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
		// PID = Parent PID + 2
		// For Wait Enter (NOT COMPLETE)
		string waitEnter;
		getline(cin, waitEnter);
		cout << "\"Terminated\"" << endl;
		
	} else {
		// Parent PID
		// For Parent (NOT COMPLETE)
		cout << "A loop should be here " << parentID << endl;
		int counter = 0;
		do{
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
		}while(true);
	}
}
