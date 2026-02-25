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
 * Based on Russian Peasant Algorithm
 */

void evaluate(int x){
    bool isNegative = false;
    int remaining = x;
    if (remaining < 0){
        remaining = -remaining;
        isNegative = true;
    }
    int count = 0;
    IntList order;
    IntNode* currentNode;
    order.init();

    while(remaining != 0){
        //cout << "Remaining: " << remaining;
        if(remaining % 2 == 0){
            //cout << " CROSS OUT" << endl;
            if(order.head == NULL){
                currentNode = order.createFirst(0);
            }else{
                currentNode = order.insertAfter(currentNode, 0);
            }
        }else{
            if(order.head == NULL){
                currentNode = order.createFirst(1);
            }else{
                currentNode = order.insertAfter(currentNode, 1);
            }
            //cout << endl;
        }
        remaining = remaining / 2;
        count++;
    }
    int highestPower = count-1;
    //cout << "Highest power for 2 applicable is: " << highestPower << endl;
    //order.display();
    bool binaryForm[count];
    currentNode = order.getFirst();

    for(int i = highestPower; i >= 0; i--){
        binaryForm[i] = (currentNode->data == 1 ? true : false);
        currentNode = order.getNext(currentNode);
    }
    order.cleanup();
    delete currentNode;
    currentNode = NULL;

    //cout << "Binary form of inputed number: ";
    //for(bool i : binaryForm){ cout << (i ? 1 : 0);}

    cout << "	movl	$0, %edx" << endl;
    for(int i = 0; i < count; i++){
        if(binaryForm[i]){
            if(i == count-1){
                cout << "	addl	%eax, %edx" << endl;
            }else{
                cout << "	sall	$" << highestPower-i << ", %eax" << endl;
                cout << "	addl	%eax, %edx" << endl;
                for(int j = i+1; j < count; j++){
                    if(binaryForm[j]){
                        cout << "	sarl	$" << highestPower-i << ", %eax" << endl;
                        break;
                    }
                }
            }
        }
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