/************************** Start of run_length_code.c *************************
 *
 * This utility file contains Basic Scheme and Modified Scheme of Run-Length Code
 *
 *
 * Created by Siyang Liang on 2/6/14.
 *  Copyright (c) 2014 SiyangLiang. All rights reserved.
 *
 *
 */

#include <stdio.h>
#include "const_def.h"


// Read each char and directly coded into output file 
void Basic_RLC_encode(FILE * input_fp, FILE * output_fp)
{
    
    unsigned char buf = 0x00;
    unsigned char previous_char = 0x00;
    unsigned char count = 0x00;
    
    fread(&buf, BYTE_SIZE, SYMBOL_SIZE, input_fp);// read the first symbol to give the loop a reference
    
    while (!feof(input_fp)) {
        
        previous_char = buf;
        
        count = 0x00;
        
        do {
            
            count ++;
            
            fread(&buf, BYTE_SIZE, SYMBOL_SIZE, input_fp);
            
        }while (previous_char == buf && (!feof(input_fp)));
        
        fwrite(&count, BYTE_SIZE, SYMBOL_SIZE, output_fp);
        
        fwrite(&previous_char, BYTE_SIZE, SYMBOL_SIZE, output_fp);
        
    }
    
    printf("Compress Successfully!\n");
    
}


// Read two char each time and directly output the symbol with certain #
void Basic_RLC_decode(FILE * input_fp, FILE * output_fp)
{
    
    unsigned char symbol = 0x00;
    unsigned char count = 0x00;
    
    while (!feof(input_fp)) {
        
        fread(&count, BYTE_SIZE, SYMBOL_SIZE, input_fp);// read the first symbol to give the loop a reference
        
        fread(&symbol, BYTE_SIZE, SYMBOL_SIZE, input_fp);// read the first symbol to give the loop a reference
        
        fwrite(&symbol, BYTE_SIZE, SYMBOL_SIZE, output_fp);
        
        for (int i = 1; i < count; i++) {
            
            fwrite(&symbol, BYTE_SIZE, SYMBOL_SIZE, output_fp);
        }
        
    }
    
    printf("Decompress Successfully!\n");
    
}


// Read each char and decide whether it is greater than 0x80, output accordingly
void Modified_RLC_encode(FILE * input_fp, FILE * output_fp)
{
    
    unsigned char buf = 0x00;
    unsigned char previous_char = 0x00;
    unsigned char count = 0x00;
    
    fread(&buf, BYTE_SIZE, SYMBOL_SIZE, input_fp);// read the first symbol to give the loop a reference
    
    while (!feof(input_fp)) {
        
        previous_char = buf;
        
        if (previous_char > 0x80) {
            
            count = 0x80;
            
            do {
                
                count ++;
                
                fread(&buf, BYTE_SIZE, SYMBOL_SIZE, input_fp);
                
            }while (previous_char == buf && (!feof(input_fp)));
            
            fwrite(&count, BYTE_SIZE, SYMBOL_SIZE, output_fp);
            
            fwrite(&previous_char, BYTE_SIZE, SYMBOL_SIZE, output_fp);
            
        }else{
            
            count = 0x00;
            
            do {
                
                count ++;
                
                fread(&buf, BYTE_SIZE, SYMBOL_SIZE, input_fp);
                
            }while (previous_char == buf && (!feof(input_fp)));
            
            if (count == 0x01) {
                
                fwrite(&previous_char, BYTE_SIZE, SYMBOL_SIZE, output_fp);
                
            }else{
                
                count += 0x80;
                
                fwrite(&count, BYTE_SIZE, SYMBOL_SIZE, output_fp);
                
                fwrite(&previous_char, BYTE_SIZE, SYMBOL_SIZE, output_fp);
                
            }
            
        }
        
    }
    
    printf("Compress Successfully!\n");
    
}


// Read one char each time and decide whether it is greater than 0x80, interpret accordingly
void Modified_RLC_decode(FILE * input_fp, FILE * output_fp)
{
    
    unsigned char symbol = 0x00;
    unsigned char count = 0x00;
    
    while (!feof(input_fp)) {
        
        fread(&count, BYTE_SIZE, SYMBOL_SIZE, input_fp);
        
        if (count > 0x80) {
            
            fread(&symbol, BYTE_SIZE, SYMBOL_SIZE, input_fp);
            
            count -= 0x80;
            
            for (int i = 0; i < count; i ++) {
                
                fwrite(&symbol, BYTE_SIZE, SYMBOL_SIZE, output_fp);
                
            }
            
        }else{ // otherwise, count < 0x80, which means count is a single symbol
            
            symbol = count;
            
            fwrite(&symbol, BYTE_SIZE, SYMBOL_SIZE, output_fp);
            
        }
        
    }
    
    printf("Decompress Successfully!\n");
    
}
