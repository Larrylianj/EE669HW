//
//  const_def_DCT_Quantization.h
//  DCT_Quantization_JPEG
//
//  Created by Siyang Liang on 2/20/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#ifndef DCT_Quantization_JPEG_const_def_DCT_Quantization_h
#define DCT_Quantization_JPEG_const_def_DCT_Quantization_h

#define UNIT_SIZE 8
#define QUANTIZATION_FACTOR_STANDARD 50
#define QUANTIZATION_MATRIX char Q_50[UNIT_SIZE][UNIT_SIZE] = {{16,11,10,16,24,40,51,61},{12,12,14,19,26,58,60,5},{14,13,16,24,40,57,59,56},{14,17,22,29,51,87,80,62},{18,22,37,56,68,109,103,77},{24,35,55,64,81,104,113,92},{49,64,78,87,103,121,120,101},{72,92,95,98,112,100,103,99}}

#endif
