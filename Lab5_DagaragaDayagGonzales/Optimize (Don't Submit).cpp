#include <iostream>
using namespace std;

struct IntNode {
    int data;
    IntNode* next;
};

struct IntList {
    IntNode* head;
    
    void init() {       // initialize empty list
        head = NULL;
    }

    ~IntList(){
        IntNode* current = head;            // start from head
        while (current != NULL) {           // looping through all nodes
            IntNode* next = current->next;  // save pointer to next node
            delete current;                 // deletes the current node
            current = next;                 // move to the next node
        }
        delete head;
        head = NULL;
    }
    
    //Creating the first node of a linked list (return a pointer to the created IntNode)
    IntNode* createFirst(int value) {
        IntNode* newNode = new IntNode;     // dynamically allocating memory for a new node
        newNode->data = value;              // set the data field to the given value
        newNode->next = head;               // new node is linked to existing list. Preserves old nodes; no memory leaks.
        head = newNode;                     // head is updated to point to new node
        return newNode;                     // returns pointer to new node
    }
    
    // Inserting a node after any other node (pass the pointer of the other IntNode to
    // this function; return a pointer to the new IntNode)
    IntNode* insertAfter(IntNode* prevNode, int value) {
        if (prevNode == NULL) {
            return NULL;        // cannot insert after a NULL node, return NULL
        }
        
        IntNode* newNode = new IntNode;
        newNode->data = value;
        newNode->next = prevNode->next;     // link the new node to what the prevNode pointed to
        prevNode->next = newNode;           // update prevNode to point to new node
        return newNode;
    }
    
    // Getting the first node (return a pointer, or NULL if it does not exist)
    IntNode* getFirst() {
        return head; // this returns pointer to the first node, or NUll if its empty
    }
    
    // Getting the node after any other node (return NULL if it does not exist)
    IntNode* getNext(IntNode* currentNode) {
        if (currentNode == NULL) {
            return NULL;            // cannot get the next of NULL, returns NULL
        }
        return currentNode->next;   // return pointer to next node
    }
    
    // additional function to clean up all nodes (for no memory leaks)
    void cleanup() {
        IntNode* current = head;            // start from head
        while (current != NULL) {           // looping through all nodes
            IntNode* next = current->next;  // save pointer to next node
            delete current;                 // deletes the current node
            current = next;                 // move to the next node
        }
        head = NULL; // indicates list is empty
    }
    
    // additional function to display the IntList
    void display() {
        IntNode* current = head;
        cout << "Binary of inputed number's absolute value in reverse: ";
        while (current != NULL) {
            cout << current->data;   // print data of current node
            current = current->next;
        }
        cout << endl;   // prints new line
    }
};

/*
 * Helper function allOnesLeft returns whether the rest of
 * the bits after a certain bit are all ones.
 */

bool allOnesLeft(int currentLocation, bool* binaryForm, int bitCount){
    for(int j = currentLocation+1; j < bitCount; j++){
        // if not a one, then stop checking, because it is not all ones left
        if(!binaryForm[j]){
            return false;
        }
    }
    // if all bits after current bit are ones, then it is in fact all ones left
    return true;
}

/*
 * Helper function reshiftEAX decides whether it is still
 * necessary to right shift %eax back to what it was before
 * the left shifts
 */

void reshiftEAX(int currentLocation, bool* binaryForm, int highestPower, int bitCount){
    for(int j = currentLocation+1; j < bitCount; j++){
        // if there exists a one that remains after this bit, then right shift %eax as many times as it was left shifted
        if(binaryForm[j]){
            cout << "	sarl	$" << highestPower-currentLocation << ", %eax" << endl;
            break;
        }
    }
    // if not, then no need to right shift. 
    // It's all zeros left, so the absolute value of the multiplier
    // has already been reached via sum, and %eax will have its contents
    // replaced to store a different variable to be used by setElement
}

/**
 * Based on Russian Peasant Algorithm, function evaluate prints out 
 * the necessary instructions to multiply an element by almost any 
 * integer multiplier (limit depends on the hardware).
 *
 * @param x serves as the integer multiplier that will affect the
 * outputed multiplication instructions.
 */

void evaluate(int x){
    // Initializes up the relevant variables
    bool isNegative = false;
    int remaining = x;
    if (remaining < 0){
        remaining = -remaining;
        isNegative = true;
    }
    int bitCount = 0;   // how many bits needed to form x in binary
    IntList order;      // will serve as a temporary dynamic linked list of 1s or 0s
    order.init();       // empty IntList initialized
    IntNode* currentNode;

    cout << endl;
    // Applying Russian's Peasant Algorithm to generate binary form of x
    while(remaining != 0){
        cout << "Remaining: " << remaining;
        if(remaining % 2 == 0){
            cout << " r0" << endl;
            if(order.head == NULL){
                currentNode = order.createFirst(0);
            }else{
                currentNode = order.insertAfter(currentNode, 0);
            }
        }else{
            cout << " r1" << endl;
            if(order.head == NULL){
                currentNode = order.createFirst(1);
            }else{
                currentNode = order.insertAfter(currentNode, 1);
            }
        }
        remaining = remaining / 2;
        bitCount++; // counts how many bits were involved
    }
    int highestPower = bitCount-1;  // highest power of two involved in reaching sum x with powers of 2 only
    cout << endl << "Highest power for 2 applicable (for inputed number's absolute value) is: " << (highestPower < 0 ? 0 : highestPower) << endl;
    order.display();
    bool binaryForm[bitCount];
    currentNode = order.getFirst();

    // reverses the order of bits and sends the result to a bool array to get proper binary form of x
    // true = 1, false = 0
    for(int i = highestPower; i >= 0; i--){
        binaryForm[i] = (currentNode->data == 1 ? true : false);
        currentNode = order.getNext(currentNode);
    } 

    // free up storage
    order.cleanup();
    delete currentNode;
    currentNode = NULL;

    cout << "Binary form of the inputed number's absolute value: ";
    for(bool i : binaryForm){ cout << (i ? 1 : 0);}

    cout << endl << endl << "Multiplication Instructions:" << endl;

    cout << "	movl	$0, %edx" << endl; // ensures that our product register no longer equals to "i" as in array index for IntArray's elements
    bool all_ones_after = false; // a special boolean that can allow a shortcut for the multiplication procedure when applicable

    for(int i = 0; i < bitCount; i++){
        // if bit is 1
        if(binaryForm[i]){
            if(i == bitCount-1){
                cout << "	addl	%eax, %edx" << endl;
            }else{
                // checks if the rest of the bits are ones
                all_ones_after = allOnesLeft(i, binaryForm, bitCount);
                // if above if such, then utilize a shorter version for calculating the product from this point, then end the instruction generator

                if (all_ones_after){                                                
                                                                                    // Example: x = 23, binary form is 10111. i is 2
                    cout << "	sall	$" << highestPower-i+1 << ", %eax" << endl; // 4-2-1 = 3. %eax = 8x
                    cout << "	addl	%eax, %edx" << endl;                        // %edx = 16x + 8x = 24x
                    cout << "	sarl	$" << highestPower-i+1 << ", %eax" << endl; // %eax = 8x/8 = x
                    cout << "	subl	%eax, %edx" << endl;                        // %edx = 24x - x = 23x
                    break;                                                          // Since we technically traversed through the rest of the bits, consider this part of the function finished

                }else{ // if not, then shift %eax and add the result to the product register %edx as intended
                    cout << "	sall	$" << highestPower-i << ", %eax" << endl;   // Left shift %eax by the appropriate amount
                    cout << "	addl	%eax, %edx" << endl;                        // Add %eax and %edx, with the sum stored at %edx 
                    reshiftEAX(i, binaryForm, highestPower, bitCount);              // if the rest of the bits are not 0, then right shift %eax back to what it was before the left shifts.
                }
            }
        }
    }
    // if the supplied multiplier is negative, include the instruction within
    if (isNegative){
        cout << "	negl	%edx" << endl;
    }
}

// 10 = 1*8 + 0*4 + 1*2 + 0*1
// 11 = 1*8 + 0*4 + 1*2 + 1*1
int main(void){
    int x;
    cout << "Insert number: ";
    cin >> x;
    evaluate(x);
    return 0;
}