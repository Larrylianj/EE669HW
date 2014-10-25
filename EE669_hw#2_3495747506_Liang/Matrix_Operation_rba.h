//
//  Matrix_Operation_rba.h
//  RemoveBlockingArtifact
//
//  Created by Siyang Liang on 2/25/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#ifndef RemoveBlockingArtifact_Matrix_Operation_rba_h
#define RemoveBlockingArtifact_Matrix_Operation_rba_h

#include"const_def_rba.h"

typedef struct CharMatrix{
    
     char data[UNIT_SIZE][UNIT_SIZE];
    
}CharMatrix;
typedef struct UncharMatrix{
    
    unsigned char data[UNIT_SIZE][UNIT_SIZE];
    
}UncharMatrix;
typedef struct DoubleMatrix{
    
    double data[UNIT_SIZE][UNIT_SIZE];
    
}DoubleMatrix;

void print_MatrixUnchar(UncharMatrix * s);
void print_MatrixDouble(DoubleMatrix * c);
void print_MatrixChar(CharMatrix * s);

#endif
