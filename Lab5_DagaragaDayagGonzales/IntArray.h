#include <cstddef>

struct IntArray {
    private:
        int size;
        int *elements;
    public:
        IntArray(int s) {
            size = s;
            elements = new int[size];
        }

        ~IntArray() {
            delete[] elements;
            elements = NULL;
        }

        int getSize() {
            return size;
        }

        int* getElements() {
            return elements;
        }

        int getElement(int index) {
            return elements[index];
        } 

        void setElement(int index, int value){
            elements[index] = value;
        }
};