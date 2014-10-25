/************************** Start of node_manipulation.c *************************
 *
 * This utility file contains all of the Node manipulation needed to impement
 * Shannon-Fano Coding and Huffman Coding including the Coding Algorithms 
 * themselves.
 *
 * Basically you just use 
 * void Shannon_Fano_encode(FILE * input_handle, FILE * read_handle, FILE * output_handle)
 * and
 * void Huffman_encode(FILE * input_handle, FILE * read_handle, FILE * output_handle)
 *
 *
 * Created by Siyang Liang on 2/6/14.
 *  Copyright (c) 2014 SiyangLiang. All rights reserved.
 *
 *
 */

#include <stdio.h>
#include "node_manipulation.h"
#include "const_def.h"

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

/******************************************** Node manipulation ********************************************************/

// Initialize the Nodes
Node * initializeNode(Node * startNode)
{
    
    for (int i = 0; i < SYMBOL_NUM; i ++)
    {
        
        startNode->count = 0;// initialize the count for each symbol to be 0
        startNode->symbol = i;// initialize the symbol to be the corresponding index
        Bit * bitnode = (Bit *)malloc(sizeof(Bit));
        bitnode->bitValue = '2';
        startNode->bit = bitnode;// initialize the bit, differ from 0,1
        startNode->nextNode = NULL;
        startNode->leftChild = NULL;
        startNode->rightChild = NULL;
        startNode->father = NULL;
        startNode ++;
        
    }
    
    return startNode - SYMBOL_NUM;
    
}


// Read the file, create a Node for each symbol
Node * readFileIntoNodes(FILE * fp, Node * startNode)
{
    
    int buf = 0;
    
    while (fread(&buf, BYTE_SIZE, SYMBOL_SIZE, fp)) {// read one symbol from the file each time
        
        for (int i = 0; i < SYMBOL_NUM; i ++)
        {
            
            if (buf == (startNode+i)->symbol)
            {
                
                (startNode+i)->count ++;
                
            }
            
        }
        
        
    }
    
    return startNode;
}


// Create a Node for symbol with count times appeareance
Node * createNode(int count, int symbol)
{
    
    Node newNode;
    Node * new = NULL;
    newNode.count = count;
    newNode.symbol = symbol;
    new = & newNode;
    
    return new;
}


// Create linked list for the Nodes
Node * createLinkedList(Node * startNode)
{
    
    Node * currentNode = NULL;
    Node * nextNode = NULL;
    
    while (startNode->count == 0 && startNode->symbol < SYMBOL_NUM - 1) {
        
        startNode ++;
        
    }
    
    currentNode = startNode;
    
    while (currentNode->symbol < SYMBOL_NUM - 1) {
        
        nextNode = currentNode + 1;
        
        while (nextNode->count == 0 && nextNode->symbol < SYMBOL_NUM - 1) {
            
            nextNode ++;
            
        }
        
        if(nextNode->count != 0){
            
            currentNode->nextNode = nextNode;
            currentNode = nextNode;
            
        }else{
            
            break;
            
        }
        
    }
    
    currentNode->nextNode = NULL;
    
    return startNode;
    
}


// Find the Node with minimum count in the linked list
Node * findMinCountInLinkedList(Node * startNode)
{
    
    int minCount = startNode->count;
    
    Node * minNode = startNode;
    
    for(Node * node = startNode; node != NULL; node = node->nextNode)
    {
        
        if (node->count < minCount) {
            
            minCount = node->count;
            minNode = node;
        }
        
    }
    
    return minNode;
    
}


// Delete the given Node in the linked list
Node * deleteNodeFromLinkedList(Node * startNode, Node * givenNode)
{
    
    if (startNode == givenNode) { //if to delete the startNode, change the startNode to its nextNode
        
        startNode = startNode->nextNode;
        
    }else{ // otherwise to delete a Node requires
        
        Node * currentNode = NULL;
        Node * parentNode = NULL;
        
        currentNode = startNode;
        
        while (currentNode->nextNode->nextNode != NULL) {
            
            parentNode = currentNode;
            currentNode = currentNode->nextNode;
            
            if (currentNode == givenNode) {
                
                parentNode->nextNode = currentNode->nextNode;
            }
        }
        
        if (currentNode->nextNode == givenNode) { // if to delete the last Node, point currentNode to NULL
            
            currentNode->nextNode = NULL;
            
        }
        
    }
    
    return startNode;
    
}


// Calculate the sum of the counts of the linked list
int sumOfCount(Node * startNode)
{
    int sum = 0;
    
    for (Node * node = startNode; node != NULL; node = node->nextNode)
    {
        
        sum += node->count;
        
    }
    
    return sum;
    
}

// Calculate the number of Nodes of the linked list
int numberOfNodes(Node * startNode)
{
    
    int num = 0;
    
    for (Node * node = startNode; node != NULL; node = node->nextNode)
    {
        
        num ++;
        
    }
    
    return num;
    
}


// Compute the entropy for the current file and print the frequency for each symbol
void computeEntropy(Node * startNode)
{
    
    double entropy = 0;
    double sum = sumOfCount(startNode);
    
    for (Node * node = startNode; node != NULL; node = node->nextNode)
    {
        
        printf("Symbol %c appears %d times, corresponding frequency is %f.\n", node->symbol, node->count, node->count/sum);
        entropy += node->count/sum * log2(sum/node->count);
        
    }
    
    printf("The entropy of this file is %f bits/symbol\n", entropy);
    
}


// Combine two Nodes into one Node, sum the counts
Node * combineTwoNode(Node * leaf_a, Node * leaf_b)
{
    
    Node * fatherNode= (Node *)malloc(sizeof(Node));
    
    Bit * bitnode = (Bit *)malloc(sizeof(Bit));
    bitnode->bitValue = '2';
    fatherNode->bit = bitnode;// initialize the bit, differ from 0,1
    fatherNode->symbol = SYMBOL_NUM + 1; // To differ fathre Nodes from leaf Nodes
    fatherNode->leftChild = leaf_a;
    fatherNode->rightChild = leaf_b;
    fatherNode->count = leaf_a->count + leaf_b->count;
    
    leaf_a->father = fatherNode;
    leaf_b->father = fatherNode;
    
    return fatherNode;
    
}


// Build the Huffuman Tree (core algorithm)
Node * buildTree(Node * startNode)
{
    
    while (numberOfNodes(startNode) > 2) {// do combination until two Nodes left
        
        // find the minimum two Nodes
        Node * min1 = findMinCountInLinkedList(startNode);
        startNode = deleteNodeFromLinkedList(startNode, min1);
        
        Node * min2 = findMinCountInLinkedList(startNode);
        startNode = deleteNodeFromLinkedList(startNode, min2);
        
        Node * father = (Node *)malloc(sizeof(Node));
        
        if (father != NULL) {
            
            father = combineTwoNode(min1, min2);
            
            // after combine them, add the father Node to the linked list
            father->nextNode = startNode;
            startNode = father;
            
        }else{
            
            printf("Error requesting storage!\n");
            
        }
        
    }
    
    return combineTwoNode(startNode, startNode->nextNode);
    
}


// Assign 0,1 to Nodes according to the generating tree
void assignBitToNode(Node * root)
{
    
    if (root->leftChild != NULL && root-> rightChild != NULL) {
        
        root->leftChild->bit->bitValue = '0';
        root->rightChild->bit->bitValue = '1';
        
        assignBitToNode(root->leftChild);
        assignBitToNode(root->rightChild);
        
    }
    
}


// Find the node by a given symbol, (pre-order search)
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


// Output dictionary
void outputDic(Node * root, FILE * fp)
{
    
    if (root != NULL) {
        
        fwrite(&root->symbol, BYTE_SIZE, SYMBOL_SIZE, fp);
        outputDic(root->leftChild, fp);
        outputDic(root->rightChild, fp);
        
    }
}


// Output bitstreams to file
void outputBitsToFile(FILE * input_fp, FILE * output_fp, Node * root)
{
    
    int buf = 0;
    int bit_used = 0;// bit used counter
    unsigned char bits = 0x00;
    
    // Create a stack for output bit stream
    Stack * stack = (Stack *)malloc(sizeof(Stack));
    initializeStack(stack);
    
    Node * node = NULL;
    
    Bit * bitnode =NULL;
    
    while (!feof(input_fp)) {
        
        fread(&buf, BYTE_SIZE, SYMBOL_SIZE, input_fp);
        
        node = findNodeBySymbol(root, buf);
        
        do {
            
            Push(stack, node->bit);// push the all node's father into the stack form bottom up
            
            node = node->father;
            
        } while (node != root);
        
        while (stack->length > 0) {
            
            bit_used ++;
            
            bitnode = Pop(stack);// pop the father Node one by one
            
            if (bitnode->bitValue == '1') {
                
                bits = bits | 0x01;
                
            }else{
                
                bits = bits | 0x00;
                
            }
            
            if (bit_used%BYTE_OF_BIT == 0) {// if the counter reach a size of char, print it to file
                
                fwrite(&bits, BYTE_SIZE, SYMBOL_SIZE, output_fp);
                
                bits = 0x00;// reset bit stream buffer
                bit_used = 0;// reset counter
                
            }
            
            bits <<= 1;
            
        }
        
    }
    
    printf("Compress file successfully!\n");
    
}

// Find the Node with maximum count in the linked list
Node * findMaxCountInLinkedList(Node * startNode)
{
    
    int maxCount = 0;//initialize the max count value
    Node * maxNode = NULL;
    
    for(Node * node = startNode; node != NULL; node = node->nextNode)
    {
        
        if (node->count > maxCount) {
            
            maxCount = node->count;
            maxNode = node;
        }
        
    }
    
    return maxNode;
    
}


// Delete the Node with maximum count in the linked list
Node * deleteMaxNodeFromLinkedList(Node * startNode)
{
    int symbolFlag;
    int maxCount = 0;
    Node * currentNode = NULL;
    Node * parentNode = NULL;
    
    for (Node * node = startNode; node != NULL; node = node->nextNode)
    {
        
        if (node->count > maxCount) {
            
            symbolFlag = node->symbol; // find the node with maximum Count, flag its symbol
            maxCount = node->count;
            
        }
        
    }
    
    
    if (startNode->symbol == symbolFlag) { //if to delete the startNode, change the startNode to its nextNode
        
        startNode = startNode->nextNode;
        
    }else{ // otherwise to delete a Node requires
        
        currentNode = startNode;
        
        while (currentNode->nextNode->nextNode != NULL) {
            
            parentNode = currentNode;
            currentNode = currentNode->nextNode;
            
            if (currentNode->symbol == symbolFlag) {
                
                parentNode->nextNode = currentNode->nextNode;
            }
        }
        
        if (currentNode->nextNode->symbol == symbolFlag) { // if to delete the last Node, point currentNode to NULL
            
            currentNode->nextNode = NULL;
        }
        
    }
    
    return startNode;
}


// Sort the counts of symbol for the linked list
Node * sortLinkedList(Node * startNode)
{
    
    Node * newStartNode = NULL;
    Node * currentNode = NULL;
    
    newStartNode = findMaxCountInLinkedList(startNode);
    startNode = deleteMaxNodeFromLinkedList(startNode);
    
    currentNode = newStartNode;
    
    do {
        
        Node * maxNode = NULL;
        
        maxNode = findMaxCountInLinkedList(startNode);
        startNode = deleteMaxNodeFromLinkedList(startNode);
        
        currentNode->nextNode = maxNode;
        
        currentNode = currentNode->nextNode;
        
    } while (startNode != NULL);
    
    return newStartNode;
    
}


// Partition the Nodes to create the tree(Core of Shannon Fano Coding)
Node * partition(Node * startNode)
{
    
    if (startNode->nextNode == NULL){ // if there is only one Node in the linked list, return itself
        
        return startNode;
        
    }else{ // implementation of Shannon Fano Algorithm
        
        Node * currentNode = NULL;
        Node * parentNode = NULL;
        
        currentNode = startNode;
        
        int partialSum = 0;
        
        do {
            
            parentNode = currentNode;
            
            partialSum += currentNode->count;
            
            if ((partialSum + currentNode->nextNode->count) >= sumOfCount(startNode)/2) {
                
                currentNode = currentNode->nextNode;
                parentNode->nextNode = NULL;
                
                break;
                
            }
            
            currentNode = currentNode->nextNode;
            
        } while (currentNode != NULL);
        
        return combineTwoNode(partition(startNode), partition(currentNode));
        
    }
    
}

/******************************************** End of Node manipulation *************************************************/

/************************************************ Shanno-Fano Encoding *************************************************/
void Shannon_Fano_encode(FILE * input_handle, FILE * read_handle, FILE * output_handle)
// input_handle and read_handle should pointed to the same file
{
    
    Node nodeTable[SYMBOL_NUM];//Create Node array, ready for note down the input
    Node * startNode = nodeTable;// will be the start Node of the linked list
    Node * root = NULL;// will be the root Node of the generating tree
    
    // Initialize the symbol table
    startNode = initializeNode(startNode);
    
    // Read file by symbol, create Node for each symbol
    startNode = readFileIntoNodes(input_handle, startNode);
    
    // Create linked list for non-zero appearing symbol Nodes
    startNode = createLinkedList(startNode);// after this step, delete all zero-appearence symbols
    
    // Sort the counts in order for the linked list
    startNode = sortLinkedList(startNode);
    
    // Compute the entropy of the current file and print it
    computeEntropy(startNode);
    
    // Create generating tree for the Nodes
    root = partition(startNode);
    assignBitToNode(root);
    
    // Output Dictionary to file
    outputDic(root, output_handle);
    
    // Generate bitstream for each symbol
    outputBitsToFile(read_handle, output_handle, root);
    
}
/************************************************** End of Encoding ****************************************************/

/**************************************************** Huffman Encoding *************************************************/
void Huffman_encode(FILE * input_handle, FILE * read_handle, FILE * output_handle)
// input_handle and read_handle should pointed to the same file
{
    
    Node nodeTable[SYMBOL_NUM];//Create Node array, ready for note down the input
    Node * startNode = nodeTable;// will be the start Node of the linked list
    Node * root = NULL;// will be the root Node of the generating tree
    
    // Initialize the symbol table
    startNode = initializeNode(nodeTable);
    
    // Read file by symbol, create Node for each symbol
    startNode = readFileIntoNodes(input_handle, startNode);
    
    // Create linked list for non-zero appearing symbol Nodes
    startNode = createLinkedList(startNode);// after this step, delete all zero-appearence symbols
    
    // Create generating tree for the Nodes
    root = buildTree(startNode);
    assignBitToNode(root);
    
    // Output Dictionary to file
    outputDic(root, output_handle);
    
    // Generate bitstream for each symbol
    outputBitsToFile(read_handle, output_handle, root);
    
}
/********************************************* End of Huffman Encoding *************************************************/

/********************************************** only used for debuging use *********************************************/


// Print the linkedlist
void printLinkedList(Node * startNode)
{
    
    for (Node * node = startNode; node != NULL; node = node->nextNode)
    {
        
        printf("Symbol %d appears %d times\n", node->symbol, node->count);
        
    }
    
}


// Traversal the tree
void traversal(Node * root)
{
    
    if (root != NULL) {
        
        printf("%d with %d times\n",root->symbol,root->count);
        traversal(root->leftChild);
        traversal(root->rightChild);
        
    }
}


// Generate the bit stream for the given Node
void findBitsForNode(Node * givenNode, Node * root)
{
    
    for (Node * node = givenNode; node != root; node = node->father)
    {
        
        printf("%c", node->bit->bitValue);
        
    }
    
}


// Add Node to a linked list, will be used in copyOfLinkedList() function
Node * addNodeToLinkedList(Node * startNode, Node * newNode)
{
    
    if (startNode == NULL) {
        
        startNode = newNode;
        
    }else{
        
        Node * node = startNode;
        
        while (node->nextNode != NULL) {
            
            node = node->nextNode;
            
        }
        
        node->nextNode = newNode;
        
    }
    
    return startNode;
    
}


// Make a copy of the linked list (for decoding)
Node * copyOfLinkedList(Node * startNode)
{
    
    Node * newStartNode = NULL;
    
    for (Node * node = startNode; node != NULL; node = node->nextNode) {
        
        Node * newNode = (Node *)malloc(sizeof(Node));
        
        if (newNode == NULL) {
            
            printf("Can not copy the linked list!\n");
            
        }
        
        newNode->bit = node->bit;
        newNode->symbol = node->symbol;
        newNode->count = node->count;
        
        newStartNode = addNodeToLinkedList(newStartNode, newNode);
        
    }
    
    return newStartNode;
    
}

/****************************************************** The end ********************************************************/