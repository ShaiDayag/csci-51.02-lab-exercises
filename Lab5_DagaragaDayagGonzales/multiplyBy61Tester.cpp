/*
Pete Justin Dagaraga (231874)
Shaan Graal Dayag (231928)
Robynne Alexa Gonzales (232944)

February 26, 2026
*/


/*
We have not discussed the C++ language code in our program with anyone other than our instructor or the teaching assistants assigned to this course.
We have not used C++ language code obtained from another student, or any other unauthorized source, either modified or unmodified.
If any C++ language code or documentation used in our program was obtained from another source, such as a textbook or course notes, that has been clearly noted with a proper citation in the comments of our program.
*/

#include <iostream>
#include "IntArray.h"
using namespace std;

/*
 * Contents of multiplyBy61 are generated via compiling this
 * cpp file alongside the appropriate s file 
 * In this case, the s file would be multiplyBy61.s, of which
 * is intended to be generated via the program compiled from
 * multiplyByXGenerator
 */

extern void multiplyBy61(IntArray *p);

/*
 * main function that tests out if the generated s file not only
 * provides something that makes the external function multiplyBy61
 * work, but also ensures that it works right.
 * If expected and function result match, then the function works.
 */

int main()
{
    int s;
    cout << "Array Size: ";
    cin >> s;
    
    // - create a new IntArray with its own elements
    IntArray* array = new IntArray(s);
    for(int i = 0; i < array->getSize(); i++){
        array->setElement(i, i+1);
    }

    // - print out the generated test array
    cout << endl << "Array Before Multiplication: " << endl;
    for(int i = 0; i < array->getSize(); i++){
        cout << array->getElement(i) << " ";
    }
    cout << endl;

    // - print out the expected result of the multiplyBy61 function
    cout << endl << "Expected Result: " << endl;
    for(int i = 0; i < array->getSize(); i++){
        cout << array->getElement(i)*61 << " ";
    }
    cout << endl;

    // - this time, multiply it using the external function.
    // - By nature of the function, the array's contents are changed permanently
    multiplyBy61(array);

    // - print the result after function application
    cout << endl << "Function\'s Result: " << endl;
    for(int i = 0; i < array->getSize(); i++){
        cout << array->getElement(i) << " ";            // See how there is no longer a number multiplying getElement. It is assumed that the function changed array's contents
    }
    cout << endl;

    // (don't forget to properly deallocate the memory after usage!)
    delete array;
    array = NULL;

    return 0;
}