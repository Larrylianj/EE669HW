//
//  node_manipulation.h
//
//  Created by Siyang Liang on 2/6/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#ifndef HW1_node_manipulation_h
#define HW1_node_manipulation_h

typedef struct Bit{
    
    char bitValue;
    
    struct Bit * nextBit;
    
}Bit;

typedef struct Node{
    
    unsigned int count; // stands for the times of appearance
    int symbol; // stands for the integer format of the symbol
    Bit * bit; // stands for 0,1
    
    struct Node * nextNode; // this pointer is for linked list
    
    struct Node * father;
    struct Node * leftChild;
    struct Node * rightChild;
    
}Node;

typedef struct Stack{ // define a stack for ouput bit streams
    
    Bit * startbit;
    
    int length;
    
}Stack;


void initializeStack(Stack * s);
void Push(Stack * s, Bit * b);
Bit * Pop(Stack * s);

Node * readFileIntoNodes(FILE * fp, Node * startNode);
Node * initializeNode(Node * startNode);
Node * createNode(int count,int symbol);
Node * createLinkedList(Node * startNode);

Node * findMaxCountInLinkedList(Node * startNode);
Node * deleteMaxNodeFromLinkedList(Node * startNode);
Node * sortLinkedList(Node * startNode);

void computeEntropy(Node * startNode);

int sumOfCount(Node * startNode);
Node * combineTwoNode(Node * leaf_a,Node * leaf_b);
Node * partition(Node * startNode);
void assignBitToNode(Node * root);
int foundNode(Node * givenNode, int symbol);
Node * findNodeBySymbol(Node * startNode, int symbol);
int numberOfNodes(Node * startNode);
Node * findMinCountInLinkedList(Node * startNode);
Node * deleteNodeFromLinkedList(Node * startNode, Node * givenNode);
Node * buildTree(Node * startNode);
void outputDic(Node * root, FILE * fp);
void outputBitsToFile(FILE * input_fp, FILE * output_fp, Node * root);

void Shannon_Fano_encode(FILE * input_handle, FILE * read_handle, FILE * output_handle);

void Huffman_encode(FILE * input_handle, FILE * read_handle, FILE * output_handle);

void printLinkedList(Node * startNode);
void traversal(Node * root);
void findBitsForNode(Node * givenNode, Node * root);
Node * addNodeToLinkedList(Node * startNode, Node * newNode);
Node * copyOfLinkedList(Node * startNode);

#endif
