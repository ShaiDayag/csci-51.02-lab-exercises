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

#include <cstddef>

/*
 * struct IntArray is a data structure that is akin
 * to an array whose elements are all integers
 */

struct IntArray {
    private:
        int size;
        int *elements;
    public:
        // IntArray constructor
        IntArray(int s) {
            size = s;
            elements = new int[size];
        }

        // IntArray destructor
        ~IntArray() {
            delete[] elements;
            elements = NULL;
        }

        // returns size of IntArray
        int getSize() {
            return size;
        }

        // returns elements of IntArray
        int* getElements() {
            return elements;
        }

        // returns an element of elements of IntArray
        int getElement(int index) {
            return elements[index];
        } 

        // replaces the element at a certain index with a new value
        void setElement(int index, int value){
            elements[index] = value;
        }
};