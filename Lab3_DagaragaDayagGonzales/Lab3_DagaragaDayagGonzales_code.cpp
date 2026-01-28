/*
Pete Justin Dagaraga (231874)
Shaan Graal Dayag (231928)
Robynne Alexa Gonzales (232944)

January 28, 2026
*/


/*
We have not discussed the C++ language code in our program with anyone other than our instructor or the teaching assistants assigned to this course.
We have not used C++ language code obtained from another student, or any other unauthorized source, either modified or unmodified.
If any C++ language code or documentation used in our program was obtained from another source, such as a textbook or course notes, that has been clearly noted with a proper citation in the comments of our program.
*/


#include <iostream>
using namespace std;

/*
 * struct IntNode holds an integer and a pointer towards another IntNode
 * It will be used as the components for IntList and IntStack shown below
 */

struct IntNode {
    int data;
    IntNode* next;
};

/*
 * struct IntList is a linked list for integer values only, made by IntNodes.
 * The head is a pointer to the first IntNode of the IntList, of which points
 * to another memory address for the next IntNode of the IntList, and so on so
 * forth.
 */

struct IntList {
    IntNode* head;
    
    void init() {       // initialize empty list
        head = NULL;
    }
    
    // a. Creating the first node of a linked list (return a pointer to the created IntNode)
    IntNode* createFirst(int value) {
        IntNode* newNode = new IntNode;     // dynamically allocating memory for a new node
        newNode->data = value;              // set the data field to the given value
        newNode->next = head;               // new node is linked to existing list. Preserves old nodes; no memory leaks.
        head = newNode;                     // head is updated to point to new node
        return newNode;                     // returns pointer to new node
    }
    
    // b. Inserting a node after any other node (pass the pointer of the other IntNode to
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
    
    // c. Getting the first node (return a pointer, or NULL if it does not exist)
    IntNode* getFirst() {
        return head; // this returns pointer to the first node, or NUll if its empty
    }
    
    // d. Getting the node after any other node (return NULL if it does not exist)
    IntNode* getNext(IntNode* currentNode) {
        if (currentNode == NULL) {
            return NULL;            // cannot get the next of NULL, returns NULL
        }
        return currentNode->next;   // return pointer to next node
    }
    
    // e. Deleting a node
    void deleteNode(IntNode* nodeToDelete) {
        if (nodeToDelete == NULL || head == NULL) {
            return; // nothing to delete so exit function
        }
        
        // special case: if deleting head, just move head pointer forward
        if (head == nodeToDelete) {     // check if deleting first node
            head = head->next;          // move head to next node
            delete nodeToDelete;        // free memory
            return;                     // exit func
        }
        
        // find the node before the one to delete
        IntNode* current = head;        // start from head
        // we need the previous node to update its 'next' pointer
        // we need to update the previous node's next pointer to bypass the node we're deleting
        while (current != NULL && current->next != nodeToDelete) {
            current = current->next;    // moves to next node to keep searching
        }
        
        // if found, delete the node
        if (current != NULL) {                  // checks to find if we found the previous node
            current->next = nodeToDelete->next; // bypass node to delete
            delete nodeToDelete;                // free memory of deleted node
        }
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
        cout << "List: ";
        while (current != NULL) {
            cout << current->data << " ";   // print data of current node
            current = current->next;
        }
        cout << endl;   // prints new line
    }
};

/*
 * struct IntStack is a stack for integer values only, made through IntNodes.
 * Also has a count variable to keep track of how many IntNodes are in the IntStack.
 * To traverse, start from the top, remove the top element via pop, and repeat until
 * the element to find has been popped. Unless the removed elements have been saved
 * in some other form, those elements are pretty much deleted.
 */

struct IntStack {
    IntNode* top;   // pointer to top node; most recently pushed
    int count;      // counter for tracking stack size
    
    void init() {   // initialize empty stack
        top = NULL;
        count = 0;
    }
    
    // PUSH an integer onto the stack (adds an IntNode)
    void push(int value) {
        IntNode* newNode = new IntNode;
        newNode->data = value;
        newNode->next = top;            // new node links to current top
        top = newNode;                  // updates top to point to new node
        count++;                        // increment count
    }
    
    // POP an integer from the stack (removes an IntNode and returns its data in the process)
    int pop() {
        if (top == NULL) {      // edge case
            cerr << "Error: Cannot pop from empty stack. Returning 0." << endl;
            return 0;
        }
        
        IntNode* temp = top;    // pointer is saved at current top node
        int value = top->data;  // save data value to return
        top = top->next;        // moves to next node
        delete temp;            // free memory of old top node
        count--;                // decrement
        return value;           // popped value
    }
    
    // get CURRENT SIZE of the stack
    int size() {
        return count;
    }
    
    // additional function to clean up all nodes (for no memory leaks)
    void cleanup() {
        while (top != NULL) {       // loops while stack is NOT empty
            IntNode* temp = top;    // pointer saved to current top
            top = top->next;        // top moved to next node
            delete temp;            // free memory of old top node
        }
        count = 0;                  // reset the count
    }
    
    // additional function to display the IntStack
    void display() {
        IntNode* current = top;             // start from top
        cout << "Stack (top to bottom): ";
        while (current != NULL) {           // looping through all nodes
            cout << current->data << " ";   // print data of current node
            current = current->next;        // move to next
        }
        cout << endl;
    }
};

// main function
int main() {
    cout << "\nIntList Demonstration" << endl;
   
    IntList list;       // variable
    list.init();        // initialized empty list
    
    // create first node
    cout << "\n1. Creating first node with value 2" << endl;
    IntNode* first = list.createFirst(2);
    list.display();
    
    // insert after first node
    cout << "\n2. Inserting 3 after first node" << endl;
    IntNode* second = list.insertAfter(first, 3);
    list.display();
    
    cout << "\n3. Inserting 4 after second node" << endl;
    IntNode* third = list.insertAfter(second, 4);
    list.display();
    
    cout << "\n4. Inserting 5 after second node" << endl;
    list.insertAfter(second, 5);
    list.display();
    
    // traversal using getFirst() and getNext()
    cout << "\n5. Traversing the list using getFirst() and getNext():" << endl;
    IntNode* current = list.getFirst();     // gets pointer to first node
    while (current != NULL) {
        cout << current->data << " ";       // print data of current node
        current = list.getNext(current);    // getNext moves to next node
    }
    cout << endl;
    
    // delete a node
    cout << "\n6. Deleting the second node" << endl;
    list.deleteNode(second);
    list.display();
    
    cout << "\n7. Deleting the first node" << endl;
    list.deleteNode(first);
    list.display();
    
    // clean up for memory leaks
    list.cleanup();     // free remaining nodes
    
    cout << "\nIntStack Demonstration" << endl;
    
    IntStack stack;
    stack.init();       // initialized empty stack
    
    // push elements
    cout << "\n1. Pushing values: 6, 7, 8, 9" << endl;
    stack.push(6);
    stack.push(7);
    stack.push(8);
    stack.push(9);
    stack.display();
    cout << "Stack size: " << stack.size() << endl;
    
    // pop elements
    cout << "\n2. Popping two elements" << endl;
    int val1 = stack.pop();
    cout << "Popped: " << val1 << endl;
    int val2 = stack.pop();
    cout << "Popped: " << val2 << endl;
    stack.display();
    cout << "Stack size: " << stack.size() << endl;
    
    // push more elements
    cout << "\n3. Pushing 10 and 11" << endl;
    stack.push(10);
    stack.push(11);
    stack.display();
    cout << "Stack size: " << stack.size() << endl;
    
    // pop all elements
    cout << "\n4. Popping all elements" << endl;
    while (stack.size() > 0) {                      // loop while stack is not empty
        cout << "Popped: " << stack.pop() << endl;
    }
    cout << "Stack size: " << stack.size() << endl;
    
    // try to pop from empty stack
    cout << "\n5. Attempting to pop from empty stack" << endl;
    stack.pop();
    
    // clean up for memory leaks
    stack.cleanup();    // free any remaining nodes
    
    cout << "\nEnd" << endl;
    return 0;
}
