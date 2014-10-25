//
//  Adaptive_Huffman.h
//  Adaptive_Huffman_Encoder
//
//  Created by Siyang Liang on 2/7/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#include <stdio.h>

#ifndef Adaptive_Huffman_Encoder_Adaptive_Huffman_h
#define Adaptive_Huffman_Encoder_Adaptive_Huffman_h

typedef struct Bit{
    
    char bitValue;
    
    struct Bit * nextBit;
    
}Bit;

// Define Node struct to do the tree construction
typedef struct Node{
    
    unsigned int count; // stands for the times of appearance
    int symbol; // stands for the integer format of the symbol
    int order;// the position in the tree
    
    struct Node * nextNode;// only use for queue manipulation
    
    struct Node * parent;
    struct Node * leftChild;
    struct Node * rightChild;
    
}Node;

// Define a stack for ouput bit streams
typedef struct Stack{
    Bit * startbit;
    
    int length;
    
}Stack;

// Define Queue to do the BFS of a tree
typedef struct Queue{
    
    Node * startNode;
    
    int length;
    
}Queue;

void initializeStack(Stack * s);
void Push(Stack * s, Bit * b);
Bit * Pop(Stack * s);

void initializeQueue(Queue * q);
void push(Queue * q, Node * node);
Node * pop(Queue * q);

void initializeNode(Node * node, int count, int symbol, int order);
Node * findNodeBySymbol(Node * root, int symbol);
Node * findNodeWithMaxOrder(Node * root, Node * changedNode);
void swapNodes(Node * a, Node * b, Node * root);
Node * updateTree(Node * root, Node * changedNode);

void Adaptive_Huffman_encode(BIT_FILE * input_fp, BIT_FILE * output_fp);
void Adaptive_Huffman_decode(BIT_FILE * input_fp, BIT_FILE * output_fp);

void traversal(Node * root);

#endif
