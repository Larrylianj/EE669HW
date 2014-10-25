//
//  main.c
//  Deblocking
//
//  Created by Siyang Liang on 3/1/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "const_def_rba.h"
#include "Matrix_Operation_rba.h"

void filter(UncharMatrix * p1, UncharMatrix * p2, int VorH);
unsigned char smooth_pixel(unsigned char * a, int index);
unsigned char max_unchar(unsigned char  * a);
unsigned char min_unchar(unsigned char  * a);
double min_double_abs(double  * a);
unsigned char limit(unsigned char a);
double clip(double x, char p, char q);

int main(int argc, const char * argv[])
{
    
    // file pointer
	FILE *file;
	int BytesPerPixel = 3; 	// 1: gray image, 3: color image
	int Size = 256;        		// default size
	
	// image data array
	unsigned char Imagedata[Size][Size][BytesPerPixel];
	
	if(argc < 3){
		printf("usage: program_name input_image.raw output_image.raw [BytesPerPixel = 3] [Size = 256]\n");
		exit(1);
	}
	if(argc >= 4 ){
		BytesPerPixel = atoi(argv[3]);
	}
	if(argc >= 5 ){
		Size = atoi(argv[4]);
	}
	
	if(BytesPerPixel > 3 || Size > 256){
		printf("This code assumes BytesPerPixel is less or equal to 3, Size is less or equal to 256\n Violating this assumption may lead to wrong result or seg fault\n");
	}
    
	// read image "ride.raw" into image data matrix
	if (!(file=fopen(argv[1],"rb")))
	{
		printf("Cannot open file1: %s\n", argv[1]);
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);
    
    // Partitioning, split the image into blocks
    int block_num = Size/UNIT_SIZE;
    UncharMatrix block[block_num][block_num][BytesPerPixel]; // create block array(3D)
    UncharMatrix * blockPointer[block_num][block_num][BytesPerPixel];
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                blockPointer[i][j][u] = &block[i][j][u];
            }
        }
    }
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                
                for (int ii = 0; ii < UNIT_SIZE; ii ++) {
                    for (int jj = 0; jj < UNIT_SIZE; jj ++) {
                        
                        blockPointer[i][j][u]->data[ii][jj] = Imagedata[ii + i*UNIT_SIZE][jj + j * UNIT_SIZE][u];
                        
                    }
                }
                
            }
        }
    }
    
    // Apply filter to each intra-block
    for (int i = 0; i < block_num - 1; i ++) {
        for (int j = 0; j < block_num - 1; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                
                filter(blockPointer[i][j][u], blockPointer[i + 1][j][u], 1);
                filter(blockPointer[i][j][u], blockPointer[i][j + 1][u], 0);
                
            }
        }
    }
    // Apply filter to last column and last row
    for (int i = 0; i < block_num - 1; i ++) {
        for (int u = 0; u < BytesPerPixel; u ++) {
            
            filter(blockPointer[i][block_num - 1][u], blockPointer[i + 1][block_num - 1][u], 1);
            filter(blockPointer[block_num - 1][i][u], blockPointer[block_num - 1][i + 1][u], 0);
            
        }
    }
    
    // Combine blocks into an image
    char ImageOutput[Size][Size][BytesPerPixel];
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                
                for (int ii = 0; ii < UNIT_SIZE; ii ++) {
                    for (int jj = 0; jj < UNIT_SIZE; jj ++) {
                        // In this problem, only deal with the grey scale image
                        ImageOutput[ii + i * UNIT_SIZE][jj + j * UNIT_SIZE][u] = blockPointer[i][j][u]->data[ii][jj];
                    }
                }
                
            }
        }
    }
    
	// write image data to output file
	if (!(file=fopen(argv[2],"wb")))
	{
		printf("Cannot open file2: %s\n", argv[2]);
		exit(1);
	}
	fwrite(ImageOutput, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);
    
}


// Apply filter on block p1 and p2 (0 for horizontal, 1 for vertical)
void filter(UncharMatrix * p1, UncharMatrix * p2, int VorH)
{
    
    // filter buffer
    unsigned char max_v,min_v;
    double kernel_coeff[KERNEL_LENGTH];
    unsigned char vector[BUFFER_LENGTH];
    int sum_diff;
    unsigned char dist_DM;
    double kernel_new_DM;
    long int temp_sum;
    long int b_coeff[] = {1,1,2,2,4,2,2,1,1};
    
    for (int i = 0; i < UNIT_SIZE; i ++) {
        
        // read the boundary pixel values into buffer
        if (VorH == 0) {
            for (int j = UNIT_SIZE/2; j < UNIT_SIZE + 1; j ++) {
                vector[j - UNIT_SIZE/2] = p1->data[i][j + 1];
                vector[j + 1] = p2->data[i][j - UNIT_SIZE/2];
            }
        }else{
            for (int j = UNIT_SIZE/2; j < UNIT_SIZE + 1; j ++) {
                vector[j - UNIT_SIZE/2] = p1->data[j + 1][i];
                vector[j + 1] = p2->data[j - UNIT_SIZE/2][i];
            }
        }
        
        // find max and min
        max_v = max_unchar(vector);
        min_v = min_unchar(vector);
        
        // assign kernel values
        for (int j = 0; j < KERNEL_LENGTH; j ++) {
            kernel_coeff[j] = (C1*vector[2*j + 1] - C2*vector[2*j + 2] + C2*vector[2*j + 3] - C1*vector[2*j +4])/C3;
        }
        
        // calculate the sum of differnece, representing the flatness measure
        sum_diff = 0;
        for (int j = 0; j < BUFFER_LENGTH - 1; j ++) {
            if (abs(vector[j] - vector[j + 1]) <= THRESHOLD1) {
                sum_diff += 1;
            }
        }
        
        // discriminant based on Flatness of the region
        if (sum_diff >= THRESHOLD2) {
            if (abs(max_v - min_v) < 2*QP_FACTOR) {
                for (int j = 1; j < BUFFER_LENGTH - 1; j ++) {
                    temp_sum = 0;
                    for (int u = 0; u < BUFFER_LENGTH - 1; u ++) {
                        temp_sum += b_coeff[u] * (int)smooth_pixel(vector, j + u - 4);
                    }
                    vector[j] = temp_sum/SMOOTH_DIVISOR;
                }
            }
        }else{
            if (abs(kernel_coeff[1]) < QP_FACTOR) {
                if (kernel_coeff[1] != 0.0 && kernel_coeff[1] != -0.0) {
                    kernel_new_DM = kernel_coeff[1]*(min_double_abs(kernel_coeff))/abs(kernel_coeff[1]);
                }else{
                    kernel_new_DM = 0;
                }
                if (vector[4] >= vector[5]) {
                    dist_DM = clip(C2/C3*(kernel_new_DM - kernel_coeff[1]), 0, (vector[4] - vector[5])/2);
                }else{
                    dist_DM = clip(C2/C3*(kernel_new_DM - kernel_coeff[1]), 0, -(vector[5] - vector[4])/2);
                }
                vector[4] = limit((unsigned char)(vector[4] - dist_DM));
                vector[5] = limit((unsigned char)(vector[5] + dist_DM));
            }
        }
        
        // put the pixel values back
        if (VorH == 0) {
            for (int j = UNIT_SIZE/2; j < UNIT_SIZE + 1; j ++) {
                p1->data[i][j + 1] = vector[j - UNIT_SIZE/2];
                p2->data[i][j - UNIT_SIZE/2] = vector[j + 1];
            }
        }else{
            for (int j = UNIT_SIZE/2; j < UNIT_SIZE + 1; j ++) {
                p1->data[j + 1][i] = vector[j - UNIT_SIZE/2];
                p2->data[j - UNIT_SIZE/2][i] = vector[j + 1];
            }
        }
    }
    
}

unsigned char smooth_pixel(unsigned char * a, int index)
{
    unsigned char smooth_pixel[BUFFER_LENGTH];
    unsigned char smooth_result = 0;
    if (abs(a[1] - a[0])<QP_FACTOR) {
        smooth_pixel[0] = a[0];
    }else{
        smooth_pixel[0] = a[1];
    }
    if (abs(a[BUFFER_LENGTH - 1] - a[BUFFER_LENGTH - 2])<QP_FACTOR) {
        smooth_pixel[BUFFER_LENGTH - 1] = a[BUFFER_LENGTH - 1];
    }else{
        smooth_pixel[BUFFER_LENGTH - 1] = a[BUFFER_LENGTH - 2];
    }
    for (int i = 1; i < BUFFER_LENGTH - 1; i ++) {
        smooth_pixel[i] = a[i];
    }
    
    if (index < 1) {
        smooth_result = smooth_pixel[0];
    }else if (index > BUFFER_LENGTH - 2){
        smooth_result = smooth_pixel[BUFFER_LENGTH - 1];
    }else{
        smooth_result = smooth_pixel[index];
    }
    
    return smooth_result;

}

double clip(double x, char p, char q)
{
    char temp;
    if (p > q) {
        temp = q;
        q = p;
        p = temp;
    }
    if (x < p) {
        return p;
    }else if (x > q){
        return q;
    }else{
        return x;
    }
}

unsigned char max_unchar(unsigned char  * a)
{
    unsigned char maximum = a[0];
    for (int i = 0; i < BUFFER_LENGTH; i ++) {
        if (a[i] > maximum) {
            maximum = a[i];
        }
    }
    return maximum;
}

unsigned char min_unchar(unsigned char  * a)
{
    unsigned char minimum = a[0];
    for (int i = 0; i < BUFFER_LENGTH; i ++) {
        if (a[i] < minimum) {
            minimum = a[i];
        }
    }
    return minimum;
}

double min_double_abs(double  * a)
{
    unsigned char minimum = abs(a[0]);
    for (int i = 0; i < KERNEL_LENGTH; i ++) {
        if (abs(a[i]) < minimum) {
            minimum = a[i];
        }
    }
    return minimum;
}

unsigned char limit(unsigned char a)
{
    if (a > 255) {
        return 255;
    }else{
        return a;
    }
}

