#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]){
	pid_t parentID = getpid();
	if( fork() == 0 ){
		// Parent PID + 1
		// For XClock (NOT COMPLETE)
		system("whereis xclock | cut -d' ' -f2 >> xclock_path.txt");
		ifstream file("xclock_path.txt");
		string line;
		const char* path;
		if( file.is_open() ){
			getline(file, line);
			file.close();
			system("rm xclock_path.txt");
			path = line.c_str();
		}else{
			cerr << "Can\'t open file!" << endl;
		}
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
		do{
		}while(true);
	}
}
