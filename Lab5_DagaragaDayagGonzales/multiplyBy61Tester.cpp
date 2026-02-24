#include <iostream>
#include "IntArray.h"
using namespace std;

extern void multiplyBy61(IntArray *p);

int main()
{
    int s, n;
    cout << "size: ";
    cin >> s;
    
    // - create a new IntArray with your own elements
    IntArray* array = new IntArray(s);
    for(int i = 0; i < array->getSize(); i++){
        array->setElement(i, i+1);
    }

    // (don't forget to properly allocate/deallocate the memory!)
    // - print it out
    cout << "array(1, size): ";
    for(int i = 0; i < array->getSize(); i++){
        cout << array->getElement(i) << " ";
    }
    cout << endl;
    // - multiply it by a number of your choosing via multiplyByX()
    multiplyBy61(array);
    // - print the result
    cout << "result: ";
    for(int i = 0; i < array->getSize(); i++){
        cout << array->getElement(i) << " ";
    }
    cout << endl;

    delete array;
    array = NULL;
    return 0;
}