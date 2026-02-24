#include "IntArray.h"

void multiplyBy61(IntArray *p)
{
    // multiply x with all elements of IntArray
    // (overwrite the previous elements)
    for(int i = 0; i < p->getSize(); i++){
        // p->elements[i] *= p->elements[i] * x;
        p->setElement(i, p->getElement(i)*61);
    }
}
