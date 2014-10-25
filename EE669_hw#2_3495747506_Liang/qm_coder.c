//
//  main.c
//  QM_coder
//
//  Created by Siyang Liang on 2/13/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#include <stdio.h>
#include "bitio.h"
#include "errhand.h"

#define STATE_SIZE 46
#define BYTE_OF_BIT 8
#define BIT_DIVISOR 0x80
#define HALF_BOUND 0X8000
#define UPPER_BOUND 0xFFFF

typedef struct State{
    
    int number;
    unsigned int Qe_hex;
    double Qe_dec;
    
    unsigned int increase;
    unsigned int decrease;
    
}State;

void QM_encode(BIT_FILE * input_handle, FILE * transition_table, BIT_FILE * output_handle);
void readTableFromFile(FILE * fp, State * table);


int main(int argc, const char * argv[])
{
    if (argc != 4) {
        printf("There should be 3 parameters! The format is:\n");
        printf("program_name input_file transition_table output_file\n");
        exit(1);
    }
    
    BIT_FILE * input_handle = NULL;
    FILE * transition_table = NULL;
    BIT_FILE * output_handle = NULL;
    
    if (!(input_handle = OpenInputBitFile(argv[1])))
	{
		printf("Cannot open file1: %s\n", argv[1]);
		exit(1);
	}
    if (!(transition_table = fopen(argv[2],"rb")))
	{
		printf("Cannot open file2: %s\n", argv[2]);
		exit(1);
	}
    if (!(output_handle = OpenOutputBitFile(argv[3])))
	{
		printf("Cannot open file1: %s\n", argv[3]);
		exit(1);
	}
    
    QM_encode(input_handle, transition_table, output_handle);
    
    CloseInputBitFile(input_handle);
    fclose(transition_table);
    CloseOutputBitFile(output_handle);
    
    return 0;
    
}

// QM encode
void QM_encode(BIT_FILE * input_handle, FILE * transition_table, BIT_FILE * output_handle)
{
    
    unsigned char bit;
    
    // define QM coder variables
    int current_state = 0;
    unsigned int A = 0x10000;
    unsigned short C = 0x0000;
    unsigned int Qe;
    int mps = 0;
    int carry = 0; // 0 = false, 1 = true
    
    State * state_table = (State *)malloc(STATE_SIZE * sizeof(State));
    
    readTableFromFile(transition_table, state_table);
    
    Qe = state_table[current_state].Qe_hex;
    
    // Find the size of the file
    fseek(input_handle->file,0,SEEK_SET);
    fseek(input_handle->file,0,SEEK_END);
    long file_size = BYTE_OF_BIT * ftell(input_handle->file);
    printf("file size = %d\n",file_size);
    fseek(input_handle->file,0,SEEK_SET);
    long size_count = 0; // initialize 1 byte to avoid output the end of the file
    
    while (size_count < file_size) {
        
        bit = InputBit(input_handle);
        
        // encode bit
        if (bit == mps) { // receiving MPS
            
            printf("RECIEVE mps\n");
            
            A -= Qe;
            
            if (A < HALF_BOUND) {
                
                if (A < Qe) {
                    
                    if (C + A > UPPER_BOUND) {
                        
                        carry = 1;
                        
                    }
                    
                    C += A;
                    A = Qe;
                    
                }
                
                current_state += state_table[current_state].increase;
                Qe = state_table[current_state].Qe_hex;
                
                // renormalization
                if (carry == 1) {
                    
                    OutputBit(output_handle, 1);
                    printf("Carry = 1 output 1\n");
                    carry = 0;
                    
                }
                while (A < HALF_BOUND) {
                    
                    A <<= 1;
                    
                    if (C > HALF_BOUND) { // output the MSB of C
                        
                        printf("C 's msb = 1 ouput 1\n");
                        OutputBit(output_handle, 1);
                        
                    }else{
                        
                        printf("C 's msb = 0 ouput 0\n");
                        OutputBit(output_handle, 0);
                        
                    }
                    
                    C <<= 1;
                    
                }
                
            }
            
            printf(" current state = %d, mps = %d, A =%x, C =%x, Qe = %x\n",current_state,mps, A,C,Qe);
            
        }else{ // receiving LPS
            
            printf("RECIEVE lps\n");
            
            A -= Qe;
            
            if (A >= Qe) {
                
                if (C + A > UPPER_BOUND) {
                    
                    carry = 1;
                    
                }
                
                C += A;
                A = Qe;
                
            }
            
            if (state_table[current_state].decrease == 0) {
                
                mps = 1 - mps; // reverse the MPS
                
            }
            
            current_state -= state_table[current_state].decrease;
            Qe = state_table[current_state].Qe_hex;
            
            // renormalization
            if (carry == 1) {
                
                OutputBit(output_handle, 1);
                carry = 0;
                
            }
            while (A < HALF_BOUND) {
                
                A <<= 1;
                
                if (C > HALF_BOUND) { // output the MSB of C
                    
                    OutputBit(output_handle, 1);
                    
                }else{
                    
                    OutputBit(output_handle , 0);
                    
                }
                
                C <<= 1;
                
            }
            
            printf(" current state = %d, A =%x, C =%x, Qe = %x\n",current_state, A,C,Qe);
            
        }
        
        size_count ++;
        
    }
    
}

// Read the transition table file and put the table into strings
void readTableFromFile(FILE * fp, State * table)
{
    
    unsigned char increase, decrease;
    
    for (int i = 0; i < STATE_SIZE; i ++) {
        
        fscanf(fp,"%d %x %lf %c %c\n",&table[i].number,&table[i].Qe_hex,&table[i].Qe_dec,&increase,&decrease);
        
        if (increase == '1') {
            
            table[i].increase = 1;
            
        }else{
            
            table[i].increase = 0;
            
        }
        
        if (decrease == '1') {
            
            table[i].decrease = 1;
            
        }else if(decrease == '2'){
            
            table[i].decrease = 2;
            
        }else{
            
            table[i].decrease = 0; // use 0 in place of S
            
        }
        
    }
    
}


