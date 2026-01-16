/*
We have not discussed the C++ language code in our program with anyone other than our instructor or the teaching assistants assigned to this course.
We have not used C++ language code obtained from another student, or any other unauthorized source, either modified or unmodified.
If any C++ language code or documentation used in our program was obtained from another source, such as a textbook or course notes, that has been clearly noted with a proper citation in the comments of our program.
*/

#include <iostream>
using namespace std;


/**
 * Shifts each letter in the input string by one position in the alphabet.   
 * Lowercase letters: a->b, b->c, ..., y->z, z->a (wraps around) 
 * Uppercase letters: A->B, B->C, ..., Y->Z, Z->A (wraps around) 
 * Non-letter characters are unchanged.
 * @param str the string being shifted
 */
string letterShiftString(string str){
    string newStr; // Initializes empty string to store the result
    int strVal; // Stores ASCII value of current character
    for (int i = 0; i < str.length() ; i++){ // Iterating through each character in the input string
        strVal = int(char(str[i])); // Gets the ASCII value of the current character


	  // Checks if character is a letter (but not 'Z' or 'z')
        if((strVal >= 65 && strVal < 90 ) || (strVal >= 97 && strVal < 122)){ // 65-89 is 'A'-'Y', 97-121 is 'a'-'y'
            newStr += char(strVal + 1); // Shifts to the next letter
        }
        // Handles the wrap-around cases: 'Z' -> 'A' and 'z' -> 'a'
  else if(strVal == 90 || strVal == 122){ // ASCII 90 is 'Z', ASCII 122 is 'z'
            newStr += char(strVal - 25); // Subtract 25 to wrap from Z to A (90-25=65='A') or z to a (122-25=97='a')
        }else{
            newStr += char(str[i]); // Non-letter characters will remain unchanged
        }
    }
    return newStr; // Returns the new, modified string
}


/**
 * Changes the contents of a pointer pointing towards a char variable by
 * one position in the alphabet.
 * Follows the same letter shifting logic in function letterShiftString.
 * @param ch the pointer to the character being shifted
 */
char letterShiftChar(char* ch){
// Check if character is a letter (but not 'Z' or 'z')
// *ch dereferences the pointer to get the actual character value
    if((int(*ch) >= 65 && int(*ch) < 90 ) || (int(*ch) >= 97 && int(*ch) < 122)){ // Same as previous function
        *ch = char(int(*ch)+1); // Shifts to next letter
	  // The asterisk (*) modifies the value at the memory address
    }
    else if(int(*ch) == 90 || int(*ch) == 122){
        *ch = char(int(*ch)-25); // Same logic as previous function
    }
    return *ch; // Returns the modified character
}


/**
 * Takes an input string and returns a string that is similar with the
 * input only at the first character, with the rest of the characters
 * being replaced by a hashtag (#).
 * Ex: Input: Fan ; Output: F##
 * @param str the string being censored
 */
string hashtagCensor(string str){
    string censor; // Initializes string variable censor
    censor += str[0]; // Censor now consists of the 1st character of input str
    for(int i = 1; i < str.length(); i++){ // Runs through str.length() - 1 times
        censor += "#"; // Adds a hashtag to the end of censor
    }
    return censor; // Returns the potentially hashtag filled string as output
}


/**
 * The main function to be run.
 * After a prompt, it then accepts a string (expected to be the user’s 
 * name) as input.
 * Outputs 4 lines.
 * 1st line: The input. (name’s contents remain unchanged)
 * 2nd line: The input with all but its first character replaced by 
 * hashtags (name’s contents remain unchanged)
 * 3rd line: The input, but all of its alphabet characters are shifted to
 * the next letter in the alphabet (or a if the character is z) (name’s 
 * contents remain unchanged)
 * 4th line: The same as the fourth, except it was done using pointers to 
 * the characters that consist the string as inputs to another function, 
 * which does change the contents of the variable name.
 */
int main(void){
    string name; // Initializes string variable name
    cout << "What is your name?" << endl; // A prompt to the user to enter their name
    cin >> name; // Assigns inputted string to variable name
    cout << name << endl; // Outputs line 1
    cout << hashtagCensor(name) << endl; // Outputs line 2
    cout << letterShiftString(name) << endl; // Outputs line 3
    for(int i = 0; i < name.length(); i++){ // Goes through all the characters of name’s contents
        letterShiftChar(&name[i]); // Applies letter shifting to the chosen character
    }
    cout << name << endl; // Outputs line 4
    return 0; // Obligatory, to know if main function runs without error
}
