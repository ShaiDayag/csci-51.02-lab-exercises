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

    // Applying Russian's Peasant Algorithm to generate binary form of x
    while(remaining != 0){
        if(remaining % 2 == 0){
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
        }
        remaining = remaining / 2;
        bitCount++; // adds to the count on how many bits were involved
    }
    int highestPower = bitCount-1;  // highest power of two involved in reaching sum x with powers of 2 only
    bool binaryForm[bitCount];
    currentNode = order.getFirst();

    // reverses the order of bits and sends the result to a bool array to get proper binary form of x
    // true = 1, false = 0
    for(int i = highestPower; i >= 0; i--){
        binaryForm[i] = (currentNode->data == 1 ? true : false);
        currentNode = order.getNext(currentNode);
    } 

    // free up storage
    order.cleanup(); // delete nodes
    currentNode = NULL;

    cout << "	movl	$0, %edx" << endl;  // ensures that our product register no longer equals to "i" (as in array index for IntArray's elements, used in function getElement and accessed through such register)
    bool all_ones_after = false;            // a special boolean that can allow a shortcut for the multiplication procedure when applicable

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

                                                                // chosen element (currently in %eax) multiplied by x, stored in %edx [this part is what we need to change]
    evaluate(x);                                                // before running evaluate %eax holds our element and %edx initially still holds the value of i. 
                                                                // After running evaluate, %edx holds the product.

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

    // Additional info that's probably relevant to the s file
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