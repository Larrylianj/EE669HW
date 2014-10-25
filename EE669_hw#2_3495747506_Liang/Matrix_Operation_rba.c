//
//  Matrix_Operation_rba.c
//  RemoveBlockingArtifact
//
//  Created by Siyang Liang on 2/25/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "Matrix_Operation_rba.h"
#include "const_def_rba.h"

DoubleMatrix * two_D_dct(UncharMatrix * m)
{
    double coeff_row = 0.0;
    double coeff_col = 0.0;
    double partial_sum = 0.0;
    
    DoubleMatrix * c = (DoubleMatrix *)malloc(sizeof(DoubleMatrix));
    
    if (c == NULL) {
        printf("Requesting coefficient matrix memory failed!\n");
        exit(1);
    }
    
    for (int i = 0; i < UNIT_SIZE; i ++) {
        for (int j = 0; j < UNIT_SIZE; j ++) {
            
            coeff_row = 1.0;
            coeff_col = 1.0;
            
            if (i == 0) {
                coeff_row = 1.0/sqrt(2.0);
            }
            if (j == 0) {
                coeff_col = 1.0/sqrt(2.0);
            }
            
            partial_sum = 0.0;
            for (int x = 0; x < UNIT_SIZE; x ++) {
                for (int y = 0; y < UNIT_SIZE; y ++) {
                    partial_sum += m->data[x][y] * cos((2*x + 1)*i*M_PI/(2*UNIT_SIZE)) * cos((2*y + 1)*j*M_PI/(2*UNIT_SIZE));
                }
            }
            c->data[i][j] = 1/sqrt(2.0*UNIT_SIZE) * coeff_row * coeff_col * partial_sum;
            //            printf("dct[%d][%d] = %f\n",i,j,m->data[i][j]);
        }
    }
    
    return c;
}
// transfer to certain quantization matrix according to the quantization factor
UncharMatrix * transferQuantizationMatrix(UncharMatrix * origin, int factor)
{
    if (factor > 100 || factor < 1) {
        printf("Quantization factor must be in the range of 1~100\n");
        exit(1);
    }
    
    double coeff = 1.0;
    if (factor > QUANTIZATION_FACTOR_STANDARD) {
        coeff = (2*QUANTIZATION_FACTOR_STANDARD - factor)/(double)QUANTIZATION_FACTOR_STANDARD;
    }else{
        coeff = QUANTIZATION_FACTOR_STANDARD/(double)factor;
    }
    
    UncharMatrix * target = (UncharMatrix *)malloc(sizeof(UncharMatrix));
    if (target == NULL) {
        printf("Requsting target matrix memory failed!\n");
        exit(1);
    }
    for (int i = 0; i < UNIT_SIZE; i ++) {
        for (int j = 0; j < UNIT_SIZE; j ++) {
            target->data[i][j] = coeff*origin->data[i][j];
        }
    }
    return target;
}
/************************************************** only for debug *****************************************************/
// print the matrix
void print_MatrixDouble(DoubleMatrix * c)
{
    for (int i = 0; i < UNIT_SIZE; i ++) {
        for (int j = 0; j < UNIT_SIZE; j ++) {
            printf("%3.2f ",c->data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void print_MatrixUnchar(UncharMatrix * s)
{
    for (int i = 0; i < UNIT_SIZE; i ++) {
        for (int j = 0; j < UNIT_SIZE; j ++) {
            printf("%d ",s->data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void print_MatrixChar(CharMatrix * s)
{
    for (int i = 0; i < UNIT_SIZE; i ++) {
        for (int j = 0; j < UNIT_SIZE; j ++) {
            printf("%d ",s->data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
/****************************************************** the end ********************************************************/
