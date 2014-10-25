//
//  bwt_and_ibwt.h
//
//  Created by Siyang Liang on 2/6/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//
#include <stdio.h>

#ifndef BWT_bwt_and_ibwt_h
#define BWT_bwt_and_ibwt_h

void BWT_preprocess(FILE * input_handle, FILE * output_handle);
void IBWT_preprocess(FILE * input_handle, FILE * output_handle);
void bwt(char * input_str, char * output_str);
void ibwt(char * input_str, char * output_str);

#endif
