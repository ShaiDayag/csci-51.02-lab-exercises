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


// item 1
struct IntNode {
   int data;
   IntNode* next;
};


// item 2 and the 5 required functions
struct IntList {
   IntNode* head;
  
   void init() {
       head = NULL;
   }
  
   // a. Creating the first node of a linked list (return a pointer to the created IntNode)
   IntNode* createFirst(int value) {
       IntNode* newNode = new IntNode;
       newNode->data = value;
       newNode->next = NULL;
       head = newNode;
       return newNode;
   }
  
   // b. Inserting a node after any other node (pass the pointer of the other IntNode to
   // this function; return a pointer to the new IntNode)
  
   IntNode* insertAfter(IntNode* prevNode, int value) {
       if (prevNode == NULL) {
           return NULL;
       }
      
       IntNode* newNode = new IntNode;
       newNode->data = value;
       newNode->next = prevNode->next;
       prevNode->next = newNode;
       return newNode;
   }
  
   // c. Getting the first node (return a pointer, or NULL if it does not exist)
   IntNode* getFirst() {
       return head;
   }
  
   // d. Getting the node after any other node (return NULL if it does not exist)
   IntNode* getNext(IntNode* currentNode) {
       if (currentNode == NULL) {
           return NULL;
       }
       return currentNode->next;
   }
  
   // e. Deleting a node
   void deleteNode(IntNode* nodeToDelete) {
       if (nodeToDelete == NULL || head == NULL) {
           return;
       }
      
       // special case: deleting the head
       if (head == nodeToDelete) {
           head = head->next;
           delete nodeToDelete;
           return;
       }
      
       // find the node before the one to delete
       IntNode* current = head;
       while (current != NULL && current->next != nodeToDelete) {
           current = current->next;
       }
      
       // if found, delete the node
       if (current != NULL) {
           current->next = nodeToDelete->next;
           delete nodeToDelete;
       }
   }
  
   // additional function to clean up all nodes (for no memory leaks)
   void cleanup() {
       IntNode* current = head;
       while (current != NULL) {
           IntNode* next = current->next;
           delete current;
           current = next;
       }
       head = NULL;
   }
  
   // additional function to display the list
   void display() {
       IntNode* current = head;
       cout << "List: ";
       while (current != NULL) {
           cout << current->data << " ";
           current = current->next;
       }
       cout << endl;
   }
};


// item 3
/*
   Implement an IntStack struct that internally uses IntNode and has functions
   for push, pop, and current size. You should NOT expose the IntNode struct to the user.
   (i.e., the push and pop functions should work exclusively with primitive integers).
   Your main function should demonstrate the 3 stack functions that you implemented.
   Notes:
       ● You do not actually need to (re)use IntList for this part – but if you do reuse
       IntList, do not expose the IntList to the user.
       ● Your program should not crash or exhibit undefined behavior if you try to pop an
   empty stack. (It is up to you how to handle it.)
*/


struct IntStack {
   IntNode* top;
   int count;
  
   void init() {
       top = NULL;
       count = 0;
   }
  
   // PUSH an integer onto the stack
   void push(int value) {
       IntNode* newNode = new IntNode;
       newNode->data = value;
       newNode->next = top;
       top = newNode;
       count++;
   }
  
   // POP an integer from the stack
   int pop() {
       if (top == NULL) {
           cerr << "Error: Cannot pop from empty stack. Returning 0." << endl;
           return 0;
       }
      
       IntNode* temp = top;
       int value = top->data;
       top = top->next;
       delete temp;
       count--;
       return value;
   }
  
   // get CURRENT SIZE of the stack
   int size() {
       return count;
   }
  
   // using the additional function
   void cleanup() {
       while (top != NULL) {
           IntNode* temp = top;
           top = top->next;
           delete temp;
       }
       count = 0;
   }
  
   // display stack
   void display() {
       IntNode* current = top;
       cout << "Stack (top to bottom): ";
       while (current != NULL) {
           cout << current->data << " ";
           current = current->next;
       }
       cout << endl;
   }
};


// main function
int main() {
   cout << "\nIntList Demonstration" << endl;
 
   IntList list;
   list.init();
  
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
   IntNode* current = list.getFirst();
   //cout << "Traversal: ";
   while (current != NULL) {
       cout << current->data << " ";
       current = list.getNext(current);
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
   list.cleanup();
  
   cout << "\nIntStack Demonstration" << endl;
  
   IntStack stack;
   stack.init();
  
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
   while (stack.size() > 0) {
       cout << "Popped: " << stack.pop() << endl;
   }
   cout << "Stack size: " << stack.size() << endl;
  
   // try to pop from empty stack
   cout << "\n5. Attempting to pop from empty stack" << endl;
   stack.pop();
  
   // clean up for memory leaks
   stack.cleanup();
  
   cout << "\nEnd" << endl;
   return 0;
}
