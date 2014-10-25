/************************** Start of bwt_ibwt.c *************************
 *
 * This utility file contains BWT pre-process function and IBWT function
 *
 * Basically you just use
 * void BWT_preprocess(FILE * input_handle, FILE * output_handle)
 * and
 * void IBWT_preprocess(FILE * input_handle, FILE * output_handle)
 *
 *
 * Created by Siyang Liang on 2/6/14.
 *  Copyright (c) 2014 SiyangLiang. All rights reserved.
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include "filehandle.h"
#include "const_def.h"
#include "bwt_and_ibwt.h"
#define BUFFER_LENGTH 40

void BWT_preprocess(FILE * input_handle, FILE * output_handle)
{
    
    char in_buf[BUFFER_LENGTH];
    char out_buf[BUFFER_LENGTH + 1];
    
    while(fread(in_buf, BYTE_SIZE, BUFFER_LENGTH * SYMBOL_SIZE, input_handle)){
        
        //        printf("%s\n",in_buf);
        
        bwt(in_buf, out_buf);
        //        printf("%s\n",out_buf);
        
        fwrite(out_buf, BYTE_SIZE, (BUFFER_LENGTH + 1) * SYMBOL_SIZE, output_handle);
        
    }
    
}


void IBWT_preprocess(FILE * input_handle, FILE * output_handle)
{
    
    char in_buf[BUFFER_LENGTH + 1];
    char out_buf[BUFFER_LENGTH];
    
    while(fread(in_buf, BYTE_SIZE, (BUFFER_LENGTH + 1) * SYMBOL_SIZE, input_handle)){
        
        ibwt(in_buf, out_buf);
        
        fwrite(out_buf, BYTE_SIZE, BUFFER_LENGTH * SYMBOL_SIZE, output_handle);
        
    }
    
}

void bwt(char * input_str, char * output_str)
{
    
    int index = 1;
    
    char matrix[BUFFER_LENGTH][BUFFER_LENGTH];
    
    char shiftString[BUFFER_LENGTH];
    
    for (int i = 0; i < BUFFER_LENGTH; i ++) {
        for (int j = 0; j < BUFFER_LENGTH; j ++) {
            
            matrix[i][j] = input_str[(i+j)%BUFFER_LENGTH];
            
        }
    }
    
    for (int i = BUFFER_LENGTH - 1; i > 0 ; i --) {
        
        for (int j = 0; j < i; j ++) {
            
            int u = 0;
            
            while (matrix[j][u] == matrix[j+1][u] && u < BUFFER_LENGTH) {
                
                u ++;
                
            }
            
            if (matrix[j][u] > matrix[j+1][u]) {
                
                char temp[BUFFER_LENGTH];
                
                strncpy(temp, matrix[j], BUFFER_LENGTH);
                
                strncpy(matrix[j], matrix[j+1], BUFFER_LENGTH);
                
                strncpy(matrix[j+1], temp, BUFFER_LENGTH);
                
            }
            
        }
        
    }
    
    //    for (int i = 0; i < BUFFER_LENGTH; i ++) {
    //
    //        for (int j = 0; j < BUFFER_LENGTH; j ++) {
    //
    //            printf("%c ", matrix[i][j]);
    //
    //        }
    //        printf("\n");
    //    }
    
    for (int i = 0; i < BUFFER_LENGTH; i ++) {
        
        output_str[i + 1] = matrix[i][BUFFER_LENGTH-1];
        
    }
    
    //    printf("\n");
    //    for (int i = 0; i < BUFFER_LENGTH; i ++) {
    //
    //        printf("%c ",output_str[i]);
    //
    //    }
    //    printf("\n");
    
    for (int i = 0; i < BUFFER_LENGTH - 1; i ++) {
        
        shiftString[i] = input_str[i+1];
        
    }
    shiftString[BUFFER_LENGTH-1] = input_str[0];
    
    //    printf("\n");
    //    for (int i = 0; i < BUFFER_LENGTH; i ++) {
    //
    //        printf("%c ",shiftString[i]);
    //
    //    }
    //    printf("\n");
    
    for (int i = 0; i < BUFFER_LENGTH; i ++) {
        
        if (strncmp(matrix[i], shiftString, BUFFER_LENGTH) == 0) {
            
            index = i;
            
        }
        
    }
    
    output_str[0] = index;
    
    
    //    printf("index = %d\n",index);
    
}

void ibwt(char * input_str, char * output_str)
{
    
    int index = input_str[0];
    int char_num_counter = 0;
    
    char input_seq[BUFFER_LENGTH];
    
    for (int i = 0; i < BUFFER_LENGTH; i ++) {
        
        input_seq[i] = input_str[i + 1];
        
    }
    
    char firstColumn[BUFFER_LENGTH];
    
    strncpy(firstColumn, input_seq, BUFFER_LENGTH);
    
    for (int i = BUFFER_LENGTH - 1; i > 1 ; i --) {
        
        for (int j = 0; j < i; j ++) {
            
            if (firstColumn[j] > firstColumn[j + 1]) {
                
                char temp = firstColumn[j];
                
                firstColumn[j] = firstColumn[j + 1];
                
                firstColumn[j + 1] = temp;
                
            }
        }
    }
    
    output_str[0] = input_seq[index];
    
    for (int j = 1; j < BUFFER_LENGTH; j++) {
        
        char_num_counter = 1;
        
        for (int i = 0; i < index; i ++) {
            
            if (firstColumn[i] == firstColumn[index]) {
                
                char_num_counter ++;
                
            }
            
        }
        
        output_str[j] = firstColumn[index];
        
        for (int i = 0; i < BUFFER_LENGTH; i ++) {
            
            if (input_seq[i] == output_str[j]) {
                
                char_num_counter --;
                
            }
            if (char_num_counter == 0) {
                
                index = i;
                
                break;
            }
            
        }
        
    }
    
}
