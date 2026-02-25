#include "IntArray.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
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

int main(int argc, char *argv[]){
    int x = atoi(argv[1]);
    cout << "	.file	\"multiplyBy" << x << ".cpp\"" << endl;
    cout << "	.text" << endl;
    // Interpretation of IntArray's getSize function (the registers used here are important when this function is called by multiplyByX)
    cout << "	.section	.text._ZN8IntArray7getSizeEv,\"axG\",@progbits,_ZN8IntArray7getSizeEv,comdat" << endl;
    cout << "	.align 2" << endl;
    cout << "	.weak	_ZN8IntArray7getSizeEv" << endl;
    cout << "	.type	_ZN8IntArray7getSizeEv, @function" << endl;
    cout << "_ZN8IntArray7getSizeEv:" << endl;
    cout << ".LFB20:" << endl;
    cout << "	.cfi_startproc" << endl;
    cout << "	endbr64" << endl;
    cout << "	pushq	%rbp" << endl;
    cout << "	.cfi_def_cfa_offset 16" << endl;
    cout << "	.cfi_offset 6, -16" << endl;
    cout << "	movq	%rsp, %rbp" << endl;
    cout << "	.cfi_def_cfa_register 6" << endl;
    cout << "	movq	%rdi, -8(%rbp)" << endl;                // send IntArray into this function's stackframe address: base pointer - 8
    cout << "	movq	-8(%rbp), %rax" << endl;                // send IntArray into %rax
    cout << "	movl	(%rax), %eax" << endl;                  // get size by moving the first 32 bits of %rax into %eax (which is also the last 32 bits of %rax)
    cout << "	popq	%rbp" << endl;                          // with the register's values changed, end function go back to the point after where it was called
    cout << "	.cfi_def_cfa 7, 8" << endl;
    cout << "	ret" << endl;
    cout << "	.cfi_endproc" << endl;
    cout << ".LFE20:" << endl;
    cout << "	.size	_ZN8IntArray7getSizeEv, .-_ZN8IntArray7getSizeEv" << endl;
    // Interpretation of IntArray's getElement function (the registers used here are important when this function is called by multiplyByX)
    cout << "	.section	.text._ZN8IntArray10getElementEi,\"axG\",@progbits,_ZN8IntArray10getElementEi,comdat" << endl;
    cout << "	.align 2" << endl;
    cout << "	.weak	_ZN8IntArray10getElementEi" << endl;
    cout << "	.type	_ZN8IntArray10getElementEi, @function" << endl;
    cout << "_ZN8IntArray10getElementEi:" << endl;
    cout << ".LFB22:" << endl;
    cout << "	.cfi_startproc" << endl;
    cout << "	endbr64" << endl;
    cout << "	pushq	%rbp" << endl;
    cout << "	.cfi_def_cfa_offset 16" << endl;
    cout << "	.cfi_offset 6, -16" << endl;
    cout << "	movq	%rsp, %rbp" << endl;
    cout << "	.cfi_def_cfa_register 6" << endl;
    cout << "	movq	%rdi, -8(%rbp)" << endl;                // IntArray sent to address base pointer - 8 
    cout << "	movl	%esi, -12(%rbp)" << endl;               // current value of i to address base pointer - 12
    cout << "	movq	-8(%rbp), %rax" << endl;                // IntArray sent to %rax
    cout << "	movq	8(%rax), %rax" << endl;                 // elements of IntArray sent to %rax
    cout << "	movl	-12(%rbp), %edx" << endl;               // current value of i sent to %edx
    cout << "	movslq	%edx, %rdx" << endl;                    // with sign-extend long to quad, move current value of i to %rdx
    cout << "	salq	$2, %rdx" << endl;                      // i * 4 = %rdx
    cout << "	addq	%rdx, %rax" << endl;                    // elements of IntArray + i * 4 (this allows us to move along the array) = %rax
    cout << "	movl	(%rax), %eax" << endl;                  // first 32 bits of %rax is sent to %eax (now holds our element of choosing based on i's value)
    cout << "	popq	%rbp" << endl;                          // with the register's values changed, end function go back to the point after where it was called
    cout << "	.cfi_def_cfa 7, 8" << endl;
    cout << "	ret" << endl;
    cout << "	.cfi_endproc" << endl;
    cout << ".LFE22:" << endl;
    cout << "	.size	_ZN8IntArray10getElementEi, .-_ZN8IntArray10getElementEi" << endl;
    // Interpretation of IntArray's setElement function (the registers used here are important when this function is called by multiplyByX)
    cout << "	.section	.text._ZN8IntArray10setElementEii,\"axG\",@progbits,_ZN8IntArray10setElementEii,comdat" << endl;
    cout << "	.align 2" << endl;
    cout << "	.weak	_ZN8IntArray10setElementEii" << endl;
    cout << "	.type	_ZN8IntArray10setElementEii, @function" << endl;
    cout << "_ZN8IntArray10setElementEii:" << endl;
    cout << ".LFB23:" << endl;
    cout << "	.cfi_startproc" << endl;
    cout << "	endbr64" << endl;
    cout << "	pushq	%rbp" << endl;
    cout << "	.cfi_def_cfa_offset 16" << endl;
    cout << "	.cfi_offset 6, -16" << endl;
    cout << "	movq	%rsp, %rbp" << endl;
    cout << "	.cfi_def_cfa_register 6" << endl;
    cout << "	movq	%rdi, -8(%rbp)" << endl;                // IntArray to address base pointer - 8
    cout << "	movl	%esi, -12(%rbp)" << endl;               // current value of i to address base pointer - 12
    cout << "	movl	%edx, -16(%rbp)" << endl;               // the product calculated to address base pointer - 16
    cout << "	movq	-8(%rbp), %rax" << endl;                // IntArray sent to %rax
    cout << "	movq	8(%rax), %rax" << endl;                 // elements of IntArray sent to %rax
    cout << "	movl	-12(%rbp), %edx" << endl;               // current value of i sent to %edx
    cout << "	movslq	%edx, %rdx" << endl;                    // with sign-extend long to quad, move current value of i to %rdx
    cout << "	salq	$2, %rdx" << endl;                      // i * 4 = %rdx
    cout << "	addq	%rax, %rdx" << endl;                    // i * 4 + elements of IntArray (this allows us to move along the array) = %rdx
    cout << "	movl	-16(%rbp), %eax" << endl;               // the product calculated is moved into %eax
    cout << "	movl	%eax, (%rdx)" << endl;                  // replace the element found at elements[i] with the product
    cout << "	nop" << endl;                                   // idle
    cout << "	popq	%rbp" << endl;                          // with the register's values changed, end function go back to the point after where it was called
    cout << "	.cfi_def_cfa 7, 8" << endl;
    cout << "	ret" << endl;
    cout << "	.cfi_endproc" << endl;
    cout << ".LFE23:" << endl;
    cout << "	.size	_ZN8IntArray10setElementEii, .-_ZN8IntArray10setElementEii" << endl;
    cout << "	.text" << endl;
    // Interpretation of multiplyByX function
    cout << "	.globl	_Z12multiplyBy" << x << "P8IntArray" << endl;
    cout << "	.type	_Z12multiplyBy" << x << "P8IntArray, @function" << endl;
    cout << "_Z12multiplyBy" << x << "P8IntArray:" << endl;
    cout << ".LFB24:" << endl;
    cout << "	.cfi_startproc" << endl;
    cout << "	endbr64" << endl;
    cout << "	pushq	%rbp" << endl;
    cout << "	.cfi_def_cfa_offset 16" << endl;
    cout << "	.cfi_offset 6, -16" << endl;
    cout << "	movq	%rsp, %rbp" << endl;
    cout << "	.cfi_def_cfa_register 6" << endl;
    cout << "	pushq	%rbx" << endl;
    cout << "	subq	$40, %rsp" << endl;
    cout << "	.cfi_offset 3, -24" << endl;
    cout << "	movq	%rdi, -40(%rbp)" << endl;               // send the IntArray into the stackframe address base pointer - 40
    cout << "	movl	$0, -20(%rbp)" << endl;                 // initialize i as 0, with stackframe address as base pointer - 20
    cout << "	jmp	.L7" << endl;                               // jump to L7 (always)
    cout << ".L8:" << endl;
    cout << "	movq	-40(%rbp), %rbx" << endl;               // send IntArray to %rbx
    cout << "	movl	-20(%rbp), %edx" << endl;               // send current value of i to %edx (which is the last 32 bits of %rdx)
    cout << "	movq	-40(%rbp), %rax" << endl;               // send IntArray to %rax
    cout << "	movl	%edx, %esi" << endl;                    // move current value of i to %esi
    cout << "	movq	%rax, %rdi" << endl;                    // send IntArray to %rdi
    cout << "	call	_ZN8IntArray10getElementEi" << endl;    // refer to getElement function call

    //This area is most likely going to need to change for the sake of optimization
    evaluate(x);      // chosen element (currently in %eax) multiplied by x, stored in %edx [this part is what we need to change]
    // right now, %eax holds our element, %edx initially still holds the value of i. At the end, %edx must hold our product.

    cout << "	movl	-20(%rbp), %eax" << endl;               // send current value of i to %eax
    cout << "	movl	%eax, %esi" << endl;                    // send current value of i to %esi
    cout << "	movq	%rbx, %rdi" << endl;                    // send IntArray to %rdi
    cout << "	call	_ZN8IntArray10setElementEii" << endl;   // refer to setElement function call
    cout << "	addl	$1, -20(%rbp)" << endl;                 // i++
    cout << ".L7:" << endl;
    cout << "	movq	-40(%rbp), %rax" << endl;               // move the IntArray into %rax
    cout << "	movq	%rax, %rdi" << endl;                    // move the IntArray into $rdi
    cout << "	call	_ZN8IntArray7getSizeEv" << endl;        // refer to getSize function call
    cout << "	cmpl	%eax, -20(%rbp)" << endl;               // with %eax (lower 32 bits of %rax) containing the size of IntArray, the value of i and size is compared
    cout << "	setl	%al" << endl;                           // results of the comparison are stored in %al
    cout << "	testb	%al, %al" << endl;                      // Confirms if %al is 1 or 0 via %al AND %al
    cout << "	jne	.L8" << endl;                               // If %al is not equal to 0, then %al is 1 and that means i < size, which leads us to jump to L8. Otherwise, this function is basically done
    cout << "	nop" << endl;                                   // idle
    cout << "	nop" << endl;                                   // idle
    cout << "	movq	-8(%rbp), %rbx" << endl;                // ???
    cout << "	leave" << endl;
    cout << "	.cfi_def_cfa 7, 8" << endl;
    cout << "	ret" << endl;
    cout << "	.cfi_endproc" << endl;
    cout << ".LFE24:" << endl;
    cout << "	.size	_Z12multiplyBy" << x << "P8IntArray, .-_Z12multiplyBy" << x << "P8IntArray" << endl;
    cout << "	.ident	\"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0\"" << endl;
    cout << "	.section	.note.GNU-stack,\"\",@progbits" << endl;
    cout << "	.section	.note.gnu.property,\"a\"" << endl;
    cout << "	.align 8" << endl;
    cout << "	.long	1f - 0f" << endl;
    cout << "	.long	4f - 1f" << endl;
    cout << "	.long	5" << endl;
    cout << "0:" << endl;
    cout << "	.string	\"GNU\"" << endl;
    cout << "1:" << endl;
    cout << "	.align 8" << endl;
    cout << "	.long	0xc0000002" << endl;
    cout << "	.long	3f - 2f" << endl;
    cout << "2:" << endl;
    cout << "	.long	0x3" << endl;
    cout << "3:" << endl;
    cout << "	.align 8" << endl;
    cout << "4:" << endl;
    return 0;
}