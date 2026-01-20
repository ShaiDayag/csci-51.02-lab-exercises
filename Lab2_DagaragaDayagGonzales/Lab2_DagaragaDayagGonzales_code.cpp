/*
Pete Justin Dagaraga (231874)
Shaan Graal Dayag (231928)
Robynne Alexa Gonzales (232944)

Month Day, 2026
*/

/*
We have not discussed the C++ language code in our program with anyone other than our instructor or the teaching assistants assigned to this course.
We have not used C++ language code obtained from another student, or any other unauthorized source, either modified or unmodified.
If any C++ language code or documentation used in our program was obtained from another source, such as a textbook or course notes, that has been clearly noted with a proper citation in the comments of our program.
*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

string spaceClearer(string str){ //removes spaces before and after the actual message content
	int strStart; int strEnd;
	string cleared;
	for(int j = 0 ; j < str.length() ; j++){
		if (char(str[j]) != ' ') {
			strStart = j;
			break;
		}
	}
	for(int k = str.length()-1 ; k >= strStart ; k--){
		if (char(str[k]) != ' '){
			strEnd = k;
			break;
		}
	}
	for(int m = strStart ; m <= strEnd ; m++){
		cleared += str[m];
	}
	return cleared;
}

bool isNumberChar(char c){
	return (int(char(c)) >= 48 && int(char(c)) <= 57); // check if the digit is 0 to 9
}

bool is1LongerThan2(string str1, string str2){ //thought process for using this is if it somehow gets an int, but there is still remaining content, then it's just a message of numbers or some sort
	return str1.length() > str2.length();
}

string messageType(string message){ //helps determine if we are yelling or number carding
	string test;
	char buffer[message.length()];
	if(message.length() > 1){
		snprintf(buffer, message.length()+1, "%d", atoi(message.c_str())); //somehow doesn't work if it isn't message.length()+1 ???
		test += buffer;
		if (is1LongerThan2(message,test)){
			return "string";
		}else{
			return "number";
		}
	}else{
		if (isNumberChar(message[0])){
			return "number";
		}else{
			return "string";
		}
	}
}



int main(void){
	int c; int x; int y; string message;
	cin >> c;
	for(int i = 0 ; i < c ; i++){
		cin >> x >> y;
		getline(cin, message);
		message = spaceClearer(message);
		cout << "Agent #" << i+1 << " is at (" << x << ", " << y << ")" << endl;
		//cout << message << endl;
		if(messageType(message) == "number"){
			cout << "Agent #" << i+1 << " holds up the number: " << message << endl;
		}else{
			cout << "Agent #" << i+1 << " yells: " << "\"" << message << "\"" << endl;
		}
		/*
		cout << message.length() << endl;
		char test[message.length()];
		cout << atoi(message.c_str()) << endl;
		snprintf(test, message.length()+1, "%d", atoi(message.c_str()));
		string mess;
		mess += test;
		cout << mess << " is the message" << endl;
		cout << mess.length() << " is the length" << endl;
		*/
	}
	return 0;
}

/*
    Potential Errors/Concerns:
    >If message is -0, is it supposed to yell "-0", or write 0 on the card?
    >If message is 003, is it supposed to yell "003", or write 3 on the card?
    >If message is +5, is it supposed to yell "+5", or write 5 on the card?
*/