//
//  Matrix_ImageBlocking.c
//  DCT_Quantization_JPEG
//
//  Created by Siyang Liang on 2/22/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "const_ImageBlocking.h"
#include "Matrix_ImageBlocking.h"


/************************************************** only for debug *****************************************************/
// print the matrix
void print_MatrixC(CoeffMatrix * c)
{
    for (int i = 0; i < UNIT_SIZE; i ++) {
        for (int j = 0; j < UNIT_SIZE; j ++) {
            printf("%3.2f ",c->coeff[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void print_MatrixS(SymbolMatrix * s)
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
