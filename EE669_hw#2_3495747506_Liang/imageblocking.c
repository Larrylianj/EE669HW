//
//  main.c
//  ImageBlocking
//
//  Created by Siyang Liang on 2/22/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "Matrix_ImageBlocking.h"


int main(int argc, const char * argv[])
{

    // file pointer
	FILE *file;
	int BytesPerPixel = 1; 	    // default is grey scale image
	int length = 256*3;
    int width = 256;          // default size
    int block_size = 8;        // default size
	
	// image data array
	unsigned char Imagedata[length][width][BytesPerPixel];
	
	if(argc < 3){
		printf("usage: program_name training_data output_vector BytePerPixel Legnth Width block_size\n");
		exit(1);
	}
    if(argc >= 4 ){
		BytesPerPixel = atoi(argv[3]);
	}
	if(argc >= 5 ){
		length = atoi(argv[4]);
	}
    if(argc >= 6 ){
		width = atoi(argv[5]);
	}
    if(argc >= 7 ){
		block_size = atoi(argv[6]);
        printf("block size = %d\n",block_size);
	}
	
	if(BytesPerPixel > 3){
		printf("This code assumes BytesPerPixel is less or equal to 3\n Violating this assumption may lead to wrong result or seg fault\n");
	}
    
	// read image into image data matrix
	if (!(file=fopen(argv[1],"rb")))
	{
		printf("Cannot open file1: %s\n", argv[1]);
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), length*width*BytesPerPixel, file);
	fclose(file);
    
    // Image blocking
    int col = length/block_size;
    int row = width/block_size;
    SymbolMatrix block[col][row][BytesPerPixel];
    for (int i = 0; i < col; i ++) {
        for (int j = 0; j < row; j ++) {
            
            for (int ii = 0; ii < block_size; ii ++) {
                for (int jj = 0; jj < block_size; jj ++) {
                    for (int u = 0; u < BytesPerPixel; u ++) {
                        // In this problem, only deal with the grey scale image
                        block[i][j][u].data[ii][jj] = Imagedata[ii + i*block_size][jj + j * block_size][u];
                    }
                }
            }
            
        }
    }
//    print_MatrixS(&block[0][0][0]);
    
    // Convert blocks into vectors
    unsigned char VectorSet[col*row*BytesPerPixel];
    long k = 0;
    for (int i = 0; i < col; i ++) {
        for (int j = 0; j < row; j ++) {
            
            for (int ii = 0; ii < block_size; ii ++) {
                for (int jj = 0; jj < block_size; jj ++) {
                    for (int u = 0; u < BytesPerPixel; u ++) {
                        
                        VectorSet[k] = block[i][j][u].data[ii][jj];
                        k ++;
                    }
                }
            }
            
        }
    }
    
    
    // write image data to target file
	if (!(file=fopen(argv[2],"wb")))
	{
		printf("Cannot open file2: %s\n", argv[2]);
		exit(1);
	}
	fwrite(VectorSet, sizeof(unsigned char), length*width*BytesPerPixel, file);
	fclose(file);
}

