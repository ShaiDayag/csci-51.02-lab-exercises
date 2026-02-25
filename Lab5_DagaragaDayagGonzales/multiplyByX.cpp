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

#include "IntArray.h"

/*
 * When this file is compiled alongside multiplyByXTester.cpp,
 * this function's contents will be made available for
 * the deliberation of the contents of multiplyByXTester.cpp
 *
 * multiplyByX multiplies each element of IntArray 
 * by an integer multiplier
 */

void multiplyByX(IntArray *p, int x)
{
    // multiply x with all elements of IntArray
    // (overwrite the previous elements)
    for(int i = 0; i < p->getSize(); i++){
        // p->elements[i] *= p->elements[i] * x;
        p->setElement(i, p->getElement(i)*x);
    }
}