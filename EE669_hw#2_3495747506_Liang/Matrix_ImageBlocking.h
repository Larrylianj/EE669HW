//
//  Matrix_ImageBlocking.h
//  DCT_Quantization_JPEG
//
//  Created by Siyang Liang on 2/22/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#ifndef DCT_Quantization_JPEG_Matrix_ImageBlocking_h
#define DCT_Quantization_JPEG_Matrix_ImageBlocking_h

#include "const_ImageBlocking.h"

typedef struct SymbolMatrix{
    
    unsigned char data[UNIT_SIZE][UNIT_SIZE];
    
}SymbolMatrix;
typedef struct CoeffMatrix{
    
    double coeff[UNIT_SIZE][UNIT_SIZE];
    
}CoeffMatrix;

CoeffMatrix * two_D_dct(SymbolMatrix * m);
SymbolMatrix * transferQuantizationMatrix(SymbolMatrix * origin, int factor);
void print_MatrixC(CoeffMatrix * c);
void print_MatrixS(SymbolMatrix * s);

#endif
