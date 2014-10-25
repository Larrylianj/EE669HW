//
//  Adaptive_Huffman.c
//  Adaptive_Huffman_Encoder
//
//  Created by Siyang Liang on 2/7/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#include <stdio.h>
#include "errhand.h"
#include "bitio.h"
#include "const_def.h"
#include "Adaptive_Huffman.h"


/******************************************** Stack manipulation *******************************************************/
// Initialize the stack
void initializeStack(Stack * s)
{
    
    s->startbit = NULL;
    s->length = 0;
    
}

// Push a Bit into the stack
void Push(Stack * s, Bit * b)
{
    
    if (s->length == 0) {
        
        s->startbit = b;
        
    }else if (s->length == 1){
        
        s->startbit->nextBit = b;
        
    }else{
        
        int length = s->length;
        Bit * temp_bit = s->startbit;
        
        while (length != 1) {
            
            temp_bit = temp_bit->nextBit;
            length --;
            
        }
        
        temp_bit->nextBit = b;
        
    }
    
    s->length ++;
    
}

// Pop a Bit out of the stack
Bit * Pop(Stack * s)
{
    
    Bit * bitnode = NULL;
    
    if (s->length == 0) {
        
        printf("The stack is empty!\n");
        
    }else if (s->length == 1){
        
        bitnode = s->startbit;
        s->startbit = NULL;
        s->length --;
        
    }else{
        
        int length = s->length;
        Bit * currentBit = s->startbit;
        Bit * parentBit = currentBit;
        
        while (length != 1) {
            
            parentBit = currentBit;
            currentBit = currentBit->nextBit;
            length --;
            
        }
        
        bitnode = currentBit;
        parentBit->nextBit = NULL;
        s->length --;
        
    }
    
    return bitnode;
    
}
/******************************************** End of stack manipulation ************************************************/

/******************************************** Queue manipulation *******************************************************/
// Initialize a queue
void initializeQueue(Queue * q)
{
    
    q->length = 0;
    q->startNode = NULL;
    
}


// Push a Node into the queue
void push(Queue * q, Node * node)
{
    
    if (q->length == 0) {
        
        q->startNode = node;
        
    }else{
        
        int length = q->length;
        Node * currentNode = q->startNode;
        Node * parentNode = NULL;
        
        while (length != 0) {
            
            parentNode = currentNode;
            currentNode = currentNode->nextNode;
            length --;
            
        }
        
        parentNode->nextNode = node;
        
    }
    
    q->length ++;
    
}


// Pop a Node out of the queue
Node * pop(Queue * q)
{
    
    Node * targetNode = NULL;
    
    if (q->length == 0) {
        
        printf("The queue is empty!\n");
        
    }else if ( q->length == 1){
        
        targetNode = q->startNode;
        
        q->startNode = NULL;
        q->length --;
        
    }else{
        
        targetNode = q->startNode;
        
        q->startNode = q->startNode->nextNode;
        q->length --;
        
    }
    
    return targetNode;
    
}
/******************************************** End of queue manipulation ************************************************/

/******************************************** Node manipulation ********************************************************/
// Initialize a Node, assign count, symbol and order to it
void initializeNode(Node * node, int count, int symbol, int order)
{
    
    node->count = count;
    node->symbol =symbol;
    node->order = order;
    node->nextNode = NULL;
    node->parent = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;
    
}


// Find the node by a given symbol, (pre-order search), if not found, will return NULL
Node * findNodeBySymbol(Node * root, int symbol)
{
    
    Node * targetNode = NULL;
    
    if (root->leftChild != NULL) {
        
        targetNode = findNodeBySymbol(root->leftChild, symbol);
    }
    
    if (root->symbol == symbol) {
        
        return root;
        
    }
    
    if (targetNode == NULL && root->rightChild != NULL) {
        
        targetNode = findNodeBySymbol(root->rightChild, symbol);
        
    }
    
    return targetNode;
    
}


// Given a certain count, find the Node with the maximum order
Node * findNodeWithMaxOrder(Node * root, Node * changedNode)
{
    
    // given a count, the Node with maximum order should be at the upper most and right most
    // so I apply BFS (right child first) on the tree, which requires me to introduce queue
    
    // Define a queue for use
    Queue * queue = (Queue *)malloc(sizeof(Queue));
    
    // Initialize the queue
    initializeQueue(queue);
    
    Node * currentNode = NULL;
    Node * targetNode = changedNode;
    
    int maxOrder = changedNode->order;
    
    push(queue, root);
    
    // BFS on the tree to find the first Node with matched count, which is of maximum order
    while (queue->length != 0) {
        
        currentNode = pop(queue);
        
        if (currentNode->count == changedNode->count) {
            
            if (currentNode->order > maxOrder) {
                
                maxOrder = currentNode->order;
                
                targetNode = currentNode;
            }
            
        }
        if (currentNode->rightChild != NULL) {
            
            push(queue, currentNode->rightChild);
            
        }
        if (currentNode->leftChild != NULL) {
            
            push(queue, currentNode->leftChild);
            
        }
        
    }
    
    return targetNode;
    
}


//Swap two Nodes and their order, including the offsprings
void swapNodes(Node * a, Node * b, Node * root)
{
    
    if ((a != root) && (b != root) && (a != b->parent) && (b != a->parent) && (a!= b)) {// the root should never be swapped
        
        if (a->parent == b->parent) {// if a and b are sibling, change their parent's leftchild and rightchild pointer
            
            if (a == a->parent->leftChild) {
                
                a->parent->rightChild = a;
                
                a->parent->leftChild = b;
            }
            
        }else{// otherwise swap everything
            
            // first swap their parent Node's children pointer
            if (a == a->parent->leftChild) {
                
                a->parent->leftChild = b;
                
                
            }else{
                
                a->parent->rightChild = b;
                
            }
            
            if (b == b->parent->leftChild) {
                
                b->parent->leftChild = a;
                
            }else{
                
                b->parent->rightChild = a;
                
            }
            
        }
        
        // then swap parent Node
        Node * temp_parent = a->parent;
        
        a->parent = b->parent;
        
        b->parent = temp_parent;
        
        //finally swap the order
        int temp_order = a->order;
        
        a->order = b->order;
        
        b->order = temp_order;
        
    }
    
}


// Update the entire tree, keep swaping Nodes accordingly unitl it reaches the root
Node * updateTree(Node * root, Node * changedNode)
{
    
    Node * maxOrderNode = (Node *)malloc(sizeof(Node));
    if (maxOrderNode == NULL) {
        printf("Error requesting storage!");
    }
    
    if (changedNode != NULL) {
        
        while (changedNode != root) {
            
            maxOrderNode = findNodeWithMaxOrder(root, changedNode);
            
            if (changedNode != maxOrderNode) {
                
                swapNodes(changedNode, maxOrderNode, root);
                
            }
            
            changedNode->count ++;
            
            changedNode = changedNode->parent;
            
        }
        
        changedNode->count ++; // final add the count of root Node
        
    }
    
    return  root;
    
}

/******************************************** End of Node manipulation *************************************************/

/******************************************** Encoding function ********************************************************/
// Do adaptive Huffman Encoding
void Adaptive_Huffman_encode(BIT_FILE * input_fp, BIT_FILE * output_fp)
{
    
    unsigned long bits = 0;
    
    // Find the size of the file
    fseek(input_fp->file,0,SEEK_SET);
    fseek(input_fp->file,0,SEEK_END);
    long file_size = ftell(input_fp->file);
    fseek(input_fp->file,0,SEEK_SET); // point the file handle back to the begining of the file
    long size_count = 0;
    
    // Create a stack for output bit stream
    Stack * stack = (Stack *)malloc(sizeof(Stack));
    initializeStack(stack);
    
    Node * NYT = (Node *)malloc(sizeof(Node));// claim NYT Node
    if (NYT == NULL) {
        printf("Error requesting storage!");
    }
    
    Node * Node_temp = NULL;// will be used to temporarily storing a given Node
    
    Bit * bitNode = NULL;// will be used to temporarily storing the poped Bit
    
    initializeNode(NYT, 0, SYMBOL_SIZE + 1, ORDER_MAX);
    
    Node * root = NYT;// initialize the tree as one Node
    
    // begin the core encoding algorithm
    while (size_count < file_size) {
        
        bits = InputBits(input_fp, BYTE_OF_BIT);// read one symbol from the file
        
        size_count ++;
        
        //        if (input_fp->file == EOF) {
        //            return;
        //        }
        
        Node * targetNode = (Node *)malloc(sizeof(Node));// this Node's count will be upgrade
        if (targetNode == NULL) {
            printf("Error requesting storage!");
        }
        
        // update the tree acccordingly
        if (findNodeBySymbol(root, (int)bits) == NULL) {
            
            /***********************************  output bits  **********************************/
            // if it is a new symbol,  first write down the path to NYT
            Node_temp = NYT;
            
            while (Node_temp != root) {// Push each parent Node into stack
                
                if (Node_temp->parent->rightChild == Node_temp) {
                    
                    Bit * bitNode = (Bit *)malloc(sizeof(Bit));
                    bitNode->bitValue = '1';
                    Push(stack, bitNode);
                    
                }else{
                    
                    Bit * bitNode = (Bit *)malloc(sizeof(Bit));
                    bitNode->bitValue = '0';
                    Push(stack, bitNode);
                    
                }
                
                Node_temp = Node_temp->parent;
                
            }
            
            while (stack->length > 0) {// Pick out each Node from the stack
                
                bitNode = Pop(stack);// pop the father Node one by one
                
                if (bitNode->bitValue == '1') {
                    
                    OutputBit(output_fp, 1);
                    
                }else{
                    
                    OutputBit(output_fp, 0);
                    
                }
                
            }
            
            // second write the symbol to file
            OutputBits(output_fp, bits, BYTE_OF_BIT);
            /************************************  end output bits *******************************/
            
            /******************************  update the position of NYT  *************************/
            Node * newNode = (Node *)malloc(sizeof(Node));
            if (newNode == NULL) {
                printf("Error requesting storage!");
            }
            
            Node * newNYT = (Node *)malloc(sizeof(Node));
            if (newNYT == NULL) {
                printf("Error requesting storage!");
            }
            
            // put the new Node and new NYT as children of previous NYT, then rename new NYT
            initializeNode(newNode, 1, (int)bits, NYT->order - 1);
            
            initializeNode(newNYT, 0, SYMBOL_SIZE + 1, NYT->order - 2);
            
            // old NYT give birth to new NYT and new Node
            NYT->count = newNode->count + newNYT->count;
            NYT->leftChild = newNYT;
            NYT->rightChild = newNode;
            newNYT->parent = NYT;
            newNode->parent = NYT;
            
            if (root == NYT) {
                
                targetNode = NULL; // for the first symbol of the file, no will-be updated Node
                
            }else{
                
                targetNode = NYT->parent; // for new symbol case, the will-be updated Node is the parent of old NYT
                
            }
            
            NYT = newNYT;
            
            /***********************************  end the update  ********************************/
            
        }else{
            
            /***********************************  output bits  **********************************/
            // for an existing symbol, just write down the path from root to it
            targetNode = findNodeBySymbol(root, (int)bits); // for existing symbol, the will-be updated Node is itself
            
            Node_temp = targetNode;
            
            while (Node_temp != root) {// Push each parent Node in stack
                
                if (Node_temp->parent->rightChild == Node_temp) {// if go through right child, push 1
                    
                    Bit * bitNode = (Bit *)malloc(sizeof(Bit));
                    bitNode->bitValue = '1';
                    Push(stack, bitNode);
                    
                }else{
                    
                    Bit * bitNode = (Bit *)malloc(sizeof(Bit));// if go through left child, push 0
                    bitNode->bitValue = '0';
                    Push(stack, bitNode);
                    
                }
                
                Node_temp = Node_temp->parent;
                
            }
            
            while (stack->length > 0) {
                
                bitNode = Pop(stack);// pop the father Node one by one
                
                if (bitNode->bitValue == '1') {
                    
                    OutputBit(output_fp, 1);
                    
                }else{
                    
                    OutputBit(output_fp, 0);
                    
                }
                
            }
            
            /************************************  end output bits *******************************/
            
        }
        
        // afterwards update the count of the entire tree
        root = updateTree(root, targetNode);
        
    }
    
    printf("Adaptive Huffman Encoding Successfully!\n");
    
}
/******************************************* End of encoding function **************************************************/
/******************************************** Decoding function ********************************************************/
// Do adaptive Huffman Encoding
void Adaptive_Huffman_decode(BIT_FILE * input_fp, BIT_FILE * output_fp)
{
    
    /******************** Initialization ******************************************/
    int bit = 0; // carry one bit
    int bits = 0; // carry bits
    
    // Find the size of the file
    fseek(input_fp->file,0,SEEK_SET);
    fseek(input_fp->file,0,SEEK_END);
    long file_size = BYTE_OF_BIT * ftell(input_fp->file);
    fseek(input_fp->file,0,SEEK_SET);
    long size_count = BYTE_OF_BIT; // initialize 1 byte to avoid output the end of the file
    
    Node * NYT = (Node *)malloc(sizeof(Node));// claim NYT Node
    if (NYT == NULL) {
        printf("Error requesting storage!");
    }
    
    initializeNode(NYT, 0, SYMBOL_SIZE + 1, ORDER_MAX);
    
    Node * Node_temp = NULL;// will be used to temporarily storing a given Node
    
    Node * targetNode = NULL; // will be the Node added to the tree or being updated count
    
    /************************** End of Initialization ********************************/
    
    /***************************** Node manipulation *********************************/
    Node * root = NYT;// initialize the tree as one Node
    
    while (size_count < file_size) {
        
        Node_temp = root;
        
        while (Node_temp->leftChild != NULL) {// Check if it is a leaf Node, if not, do the following
            
            bit = InputBit(input_fp);
            
            size_count ++;
            
            if (bit == 1) {// if the next bit is 1, move down right
                
                Node_temp = Node_temp->rightChild;
                
            }else{ // otherwise, move down left
                
                Node_temp = Node_temp->leftChild;
                
            }
            
        }
        
        if (Node_temp == NYT) { // if the leaf is NYT, means it is a new symbol
            
            bits = (int)InputBits(input_fp, BYTE_OF_BIT);
            
            size_count += BYTE_OF_BIT;
            
            if (bits != EOF) {
                
                OutputBits(output_fp, bits, BYTE_OF_BIT);
                
            }
            
            /*************************** Update NYT ***************************/
            Node * newNode = (Node *)malloc(sizeof(Node));
            if (newNode == NULL) {
                printf("Error creating new Node\n");
            }
            
            Node * newNYT = (Node *)malloc(sizeof(Node));
            if (newNYT == NULL) {
                printf("Error requesting storage!");
            }
            
            initializeNode(newNode, 1, bits, NYT->order - 1);
            initializeNode(newNYT, 0, SYMBOL_SIZE + 1, NYT->order - 2);
            
            // old NYT give birth to new NYT and new Node
            NYT->count = newNode->count + newNYT->count;
            NYT->leftChild = newNYT;
            NYT->rightChild = newNode;
            newNYT->parent = NYT;
            newNode->parent = NYT;
            
            if (root == NYT) {
                
                targetNode = NULL; // for the first symbol of the file, no will-be updated Node
                
            }else{
                
                targetNode = NYT->parent; // for new symbol case, the will-be updated Node is the parent of old NYT
                
            }
            
            NYT = newNYT;
            
            /************************* End of update NYT **********************/
            
        }else{ // If the leaf Node is a symbol, write to file directly
            
            OutputBits(output_fp, Node_temp->symbol, BYTE_OF_BIT);
            
            targetNode = Node_temp; // for existing symbol, the will-be updated Node is itself
            
        }
        /************************************ End of Node manipulation ********************************/
        root = updateTree(root, targetNode); // update the tree
        
    }
    
    printf("Decompress successfully!\n");
    
}
/******************************************* End of decoding function **************************************************/

/********************************************** only used for debuging use *********************************************/


// Traversal the tree
void traversal(Node * root)
{
    
    if (root != NULL) {
        
        traversal(root->leftChild);
        printf("%c with %d times and order %d\n",root->symbol,root->count, root->order);
        traversal(root->rightChild);
        
    }
}


/******************************************************* The end *******************************************************/
