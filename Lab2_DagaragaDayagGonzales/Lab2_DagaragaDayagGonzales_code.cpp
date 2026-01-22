/*
Pete Justin Dagaraga (231874)
Shaan Graal Dayag (231928)
Robynne Alexa Gonzales (232944)

January 22, 2026
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

/**
 * Cuts away extra whitespaces before and after the inputted string
 * This does not remove any whitespaces found within the string
 * Returns a spliced string that has the actual message content
 * @param str the string being spliced
 */
string spaceClearer(string str){ 
	int strStart; int strEnd; //initializing int variables that denote the starting and ending indices of the actual string content
	string cleared; //initializing empty string to be returned
	for(int j = 0 ; j < str.length() ; j++){ //loops through each character starting at the left of the string
		if (char(str[j]) != ' ') { //if the character at index j is not a whitespace
			strStart = j; //the value of j is the starting index of the string that has actual content
			break; //ends the loop as strStart has been found
		}
	}
	for(int k = str.length()-1 ; k >= strStart ; k--){ //loops through each character starting at the right of the string, ends at the starting index of the actual string content
		if (char(str[k]) != ' '){ //if the character at index k is not a whitespace
			strEnd = k; //the value of k is the ending index of the string that has actual content
			break; //ends the loop as strEnd has been found
		}
	}
	for(int m = strStart ; m <= strEnd ; m++){ //loops through each character found in between (inclusive) locations strStart and strEnd
		cleared += str[m]; //each character that is of content in str is added to cleared
	}
	return cleared; //returns the string with no extra whitespaces before and after it
}

/**
 * Returns true if the character is a number
 * Returns false if the character is not a number
 * Used to check if messages with a single character are truly numbers
 * @param c the character being checked
 */

bool isNumberChar(char c){
	return (int(char(c)) >= 48 && int(char(c)) <= 57); // check if the digit is 0 to 9
}

/**
 * Returns true if str1 is longer than str2
 * Returns false if str1 is not longer than str2
 * Logic behind this function's usage is that if the message becomes shorter when
 * converted into an integer, then it is most likely a not a single number message.
 * If the length remains the same, that means all characters involved are numbers, thus
 * the message being a single number message.
 * @param str1 as original message, @param str2 as "translated into integer" message
 */

bool is1LongerThan2(string str1, string str2){ //If str2 somehow gets a value that can easily be an integer, but there is still remaining content unused in the original message, then the message is not of a single integer
	return str1.length() > str2.length();
}

/**
 * Returns the kind of message that is being said by the agents.
 * Will return "string" if the message is not just a single number (not a decimal)
 * Will return "number" if the message is just a single number (not a decimal)
 * @param message as the message whose kind is to be determined
 */

string messageType(string message){ //helps determine if we are yelling or number carding
	string test; //initialized string that will be used when checking messages that are longer than one character
	char buffer[message.length()]; //initialized a char array with length of the message's length. To be filled when checking messages that are longer than one character
	int num = atoi(message.c_str()); //initialized num to hold the integer value of the conversion attempt of message into an integer
	if(message.length() > 1){ //if message is not a single character
		snprintf(buffer, message.length()+1, "%d", num); //transcribes the integer into buffer. snprintf always has the last character as a /0, which is why there is a +1, to fully transcribe its contents
		test += buffer; //buffer now fills the string with its contents
		if (is1LongerThan2(message,test)){ //checks if message is longer than test
			return "string"; //if so, then the message was not of a single integer value written in proper order
		}else{
			return "number"; //if not, then the message was of a single integer value written in proper order
		}
	}else{ //if message is a single character only
		if (isNumberChar(message[0])){ //checks if the character is a number
			return "number"; //if so, then the message is an integer
		}else{
			return "string"; //if not, then the message is some other non-numerical character
		}
	}
}

/*
 * The main function to be run
 * Gathers input from a text file
 * First line indicates number of agents N
 * The lines afterwards indicate the x coordinate, y coordinate, and message of the agent
 * Program assumes that all lines are of the proper input
 * Expected output is N pairs of lines
 * Every 1st line states the agent's coordinates
 * Every 2nd line states the agent's message and how they are delivering it.
*/

int main(void){
	int c; int x; int y; string message; //initializes number of agents, x and y coordinates, and message string
	cin >> c; //c holds the value signifying number of agents
	for(int i = 0 ; i < c ; i++){ //loops c amount of times, covering all agents
		cin >> x >> y; //x and y now hold the coordinates of the ith agent
		getline(cin, message); //message obtained from the rest of what is left in the line
		message = spaceClearer(message); //removed all whitespaces before and after message's true content
		cout << "Agent #" << i+1 << " is at (" << x << ", " << y << ")" << endl; //outputs coordinates of an agent
		if(messageType(message) == "number"){ //if message is a single number
			cout << "Agent #" << i+1 << " holds up the number: " << message << endl; //outputs agent holding up a card of that number
		}else{
			cout << "Agent #" << i+1 << " yells: " << "\"" << message << "\"" << endl; //outputs agent yelling the message
		}
	}
	return 0; //obligatory, to help determine error if main function fails.
}

/*
    Miscellaneous:
    >If message is -0, it is supposed to yell "-0"
    >If message is 003, despite how in regular input results into an integer of 3, it will be treated as something to be yelled out "003"
    >If message is +5, it is supposed to yell "+5". Having it on a card just needs it to be 5
*/